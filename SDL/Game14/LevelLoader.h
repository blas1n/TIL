#pragma once

#include <string>
#include <functional>
#include <unordered_map>
#include <rapidjson/document.h>
#include "Component.h"
#include "Math.h"

using ActorFunc = std::function<class Actor*(class Game*, const rapidjson::Value&)>;
using ComponentFunc = std::function<Component*(Actor*, const rapidjson::Value&)>;

class LevelLoader {
public:
	static bool LoadLevel(Game* game, const std::string fileName);
	static void SaveLevel(Game* game, const std::string& fileName);
	static bool LoadJSON(const std::string fileName, rapidjson::Document& outDoc);

private:
	static void LoadGlobalProperties(Game* game, const rapidjson::Value& inObject);
	static void LoadActors(Game* game, const rapidjson::Value& inArray);
	static void LoadComponents(class Actor* actor, const rapidjson::Value& inArray);

	static void SaveGlobalProperties(rapidjson::Document::AllocatorType& alloc, 
		Game* game, rapidjson::Value& inObject);
	static void SaveActors(rapidjson::Document::AllocatorType& alloc, 
		Game* game, rapidjson::Value& inArray);
	static void SaveComponents(rapidjson::Document::AllocatorType& alloc, 
		const Actor* actor, rapidjson::Value& inArray);

	static std::unordered_map<std::string, ActorFunc> ActorFactoryMap;
	static std::unordered_map<std::string, std::pair<Component::TypeId, ComponentFunc>> ComponentFactoryMap;
};