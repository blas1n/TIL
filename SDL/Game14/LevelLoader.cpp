#include "LevelLoader.h"
#include <fstream>
#include <vector>
#include <SDL/SDL.h>
#include "Game.h"
#include "Renderer.h"
#include "JsonHelper.h"
#include "Actor.h"
#include "FollowActor.h"
#include "PlaneActor.h"
#include "TargetActor.h"
#include "AudioComponent.h"
#include "BoxComponent.h"
#include "CameraComponent.h"
#include "FollowCamera.h"
#include "MeshComponent.h"
#include "MoveComponent.h"
#include "SkeletalMeshComponent.h"
#include "SpriteComponent.h"
#include "MirrorCamera.h"
#include "PointLightComponent.h"
#include "TargetComponent.h"
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>

constexpr int LevelVersion = 1;

std::unordered_map<std::string, ActorFunc> LevelLoader::ActorFactoryMap{
	{ "Actor", &Actor::Create<Actor> },
	{ "FollowActor", &Actor::Create<FollowActor> },
	{ "PlaneActor", &Actor::Create<PlaneActor> },
	{ "TargetActor", &Actor::Create<TargetActor> },
};

std::unordered_map<std::string, std::pair<Component::TypeId, ComponentFunc>> LevelLoader::ComponentFactoryMap {
	{ "AudioComponent", { Component::TypeId::TAudioComponent, &Component::Create<AudioComponent> } },
	{ "BoxComponent", { Component::TypeId::TBoxComponent, &Component::Create<BoxComponent> } },
	{ "CameraComponent", { Component::TypeId::TCameraComponent, &Component::Create<CameraComponent> } },
	{ "FollowCamera", { Component::TypeId::TFollowCamera, &Component::Create<FollowCamera> } },
	{ "MeshComponent", { Component::TypeId::TMeshComponent, &Component::Create<MeshComponent> } },
	{ "MoveComponent", { Component::TypeId::TMoveComponent, &Component::Create<MoveComponent> } },
	{ "SkeletalMeshComponent", { Component::TypeId::TSkeletalMeshComponent, &Component::Create<SkeletalMeshComponent> } },
	{ "SpriteComponent", { Component::TypeId::TSpriteComponent, &Component::Create<SpriteComponent> } },
	{ "MirrorCamera", { Component::TypeId::TMirrorCamera, &Component::Create<MirrorCamera> } },
	{ "PointLightComponent", { Component::TypeId::TPointLightComponent, &Component::Create<PointLightComponent> } },
	{ "TargetComponent", { Component::TypeId::TTargetComponent, &Component::Create<TargetComponent> } },
};

bool LevelLoader::LoadLevel(Game* game, const std::string fileName) {
	rapidjson::Document doc;
	if (!LoadJSON(fileName, doc)) {
		SDL_Log("Failed to load level %s", fileName.c_str());
		return false;
	}

	auto version = 0;
	if (!JsonHelper::GetInt(doc, "version", version) || version != LevelVersion) {
		SDL_Log("Incorrect level file version for %s", fileName.c_str());
		return false;
	}

	const auto& globals = doc["globalProperties"];
	if (globals.IsObject())
		LoadGlobalProperties(game, globals);

	const auto& actors = doc["actors"];
	if (actors.IsArray())
		LoadActors(game, actors);

	return true;
}

void LevelLoader::SaveLevel(Game* game, const std::string& fileName) {
	rapidjson::Document doc;
	doc.SetObject();

	JsonHelper::AddInt(doc.GetAllocator(), doc, "version", LevelVersion);

	rapidjson::Value globals{ rapidjson::kObjectType };
	SaveGlobalProperties(doc.GetAllocator(), game, globals);
	doc.AddMember("globalProperties", globals, doc.GetAllocator());

	rapidjson::Value actors{ rapidjson::kArrayType };
	SaveActors(doc.GetAllocator(), game, actors);
	doc.AddMember("actors", actors, doc.GetAllocator());

	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer{ buffer };
	doc.Accept(writer);

	std::ofstream outFile{ fileName, std::ios::trunc };
	if (outFile.is_open())
		outFile << buffer.GetString();
}

bool LevelLoader::LoadJSON(const std::string fileName, rapidjson::Document& outDoc) {
	std::ifstream file{ fileName, std::ios::in | std::ios::binary | std::ios::ate };
	if (!file.is_open()) {
		SDL_Log("File %s not found", fileName.c_str());
		return false;
	}

	const auto fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	std::vector<char> bytes(static_cast<size_t>(fileSize) + 1);
	file.read(bytes.data(), static_cast<size_t>(fileSize));
	outDoc.Parse(bytes.data());

	if (!outDoc.IsObject()) {
		SDL_Log("File %s is not valid JSON", fileName.c_str());
		return false;
	}

	return true;
}

void LevelLoader::LoadGlobalProperties(Game* game, const rapidjson::Value& inObject) {
	Vector3 ambient;
	if (JsonHelper::GetVector3(inObject, "ambientLight", ambient))
		game->GetRenderer()->SetAmbientLight(ambient);

	const auto& dirObj = inObject["directionalLight"];
	if (dirObj.IsObject()) {
		auto& light = game->GetRenderer()->GetDirectionalLight();

		JsonHelper::GetVector3(dirObj, "direction", light.direction);
		JsonHelper::GetVector3(dirObj, "diffuseColor", light.diffuseColor);
		JsonHelper::GetVector3(dirObj, "specularColor", light.specularColor);
	}
}

void LevelLoader::LoadActors(Game* game, const rapidjson::Value& inArray) {
	for (rapidjson::SizeType i = 0; i < inArray.Size(); ++i) {
		const auto& actorObj = inArray[i];
		if (!actorObj.IsObject()) continue;

		std::string type;
		if (!JsonHelper::GetString(actorObj, "type", type))
			continue;
		
		const auto iter = ActorFactoryMap.find(type);
		if (iter == ActorFactoryMap.end()) {
			SDL_Log("Unknown actor type %s", type.c_str());
			continue;
		}

		const auto actor = iter->second(game, actorObj["properties"]);
		if (actorObj.HasMember("components")) {
			const auto& components = actorObj["components"];
			if (components.IsArray())
				LoadComponents(actor, components);
		}
	}
}

void LevelLoader::LoadComponents(Actor* actor, const rapidjson::Value& inArray) {
	for (rapidjson::SizeType i = 0; i < inArray.Size(); ++i) {
		const auto& compObj = inArray[i];
		if (!compObj.IsObject()) continue;
		
		std::string type;
		if (!JsonHelper::GetString(compObj, "type", type))
			continue;

		auto iter = ComponentFactoryMap.find(type);
		if (iter == ComponentFactoryMap.end()) {
			SDL_Log("Unknown component type %s", type.c_str());
			continue;
		}

		const auto tid = static_cast<Component::TypeId>(iter->second.first);
		const auto comp = actor->GetComponentOfType(tid);
		if (comp)
			comp->LoadProperties(compObj["properties"]);
		else
			iter->second.second(actor, compObj["properties"]);
	}
}

void LevelLoader::SaveGlobalProperties(rapidjson::Document::AllocatorType& alloc, 
	Game* game, rapidjson::Value& inObject) {

	JsonHelper::AddVector3(alloc, inObject, "ambientLight",
		game->GetRenderer()->GetAmbientLight());

	rapidjson::Value dirObj{ rapidjson::kObjectType };
	const auto& dirLight = game->GetRenderer()->GetDirectionalLight();
	JsonHelper::AddVector3(alloc, dirObj, "direction", dirLight.direction);
	JsonHelper::AddVector3(alloc, dirObj, "diffuseColor", dirLight.diffuseColor);
	JsonHelper::AddVector3(alloc, dirObj, "specularColor", dirLight.specularColor);
	inObject.AddMember("directionalLight", dirObj, alloc);
}

void LevelLoader::SaveActors(rapidjson::Document::AllocatorType& alloc, 
	Game* game, rapidjson::Value& inArray) {

	for (const auto actor : game->GetActors()) {
		rapidjson::Value obj{ rapidjson::kObjectType };
		JsonHelper::AddString(alloc, obj, "type", Actor::TypeNames[actor->GetType()]);

		rapidjson::Value props{ rapidjson::kObjectType };
		actor->SaveProperties(alloc, props);
		obj.AddMember("properties", props, alloc);

		rapidjson::Value components{ rapidjson::kArrayType };
		SaveComponents(alloc, actor, components);
		obj.AddMember("components", components, alloc);

		inArray.PushBack(obj, alloc);
	}
}

void LevelLoader::SaveComponents(rapidjson::Document::AllocatorType& alloc, 
	const Actor* actor, rapidjson::Value& inArray) {

	for (const auto comp : actor->GetComponents()) {
		rapidjson::Value obj{ rapidjson::kObjectType };
		JsonHelper::AddString(alloc, obj, "type", Component::TypeNames[comp->GetType()]);

		rapidjson::Value props{ rapidjson::kObjectType };
		comp->SaveProperties(alloc, props);
		obj.AddMember("properties", props, alloc);

		inArray.PushBack(obj, alloc);
	}
}