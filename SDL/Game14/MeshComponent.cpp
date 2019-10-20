#include "MeshComponent.h"
#include "Shader.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"
#include "JsonHelper.h"

MeshComponent::MeshComponent(Actor* owner, bool inIsSkeletal /*= false*/)
	: Component(owner),
	mesh(nullptr),
	textureIndex(0),
	visible(true),
	isSkeletal(inIsSkeletal) {

	owner->GetGame()->GetRenderer()->AddMeshComponent(this);
}

MeshComponent::~MeshComponent() {
	owner->GetGame()->GetRenderer()->RemoveMeshComponent(this);
}

void MeshComponent::Draw(Shader* shader) {
	if (!mesh) return;
	
	shader->SetUniformValue("uWorldTransform", owner->GetWorldTransform());
	shader->SetUniformValue("uSpecularPower", mesh->GetSpecularPower());

	auto t = mesh->GetTexture(textureIndex);
	if (t) t->SetActive();

	auto vertexArray = mesh->GetVertexArray();
	vertexArray->SetActive();

	glDrawElements(GL_TRIANGLES, vertexArray->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
}

void MeshComponent::LoadProperties(const rapidjson::Value& inObj) {
	Component::LoadProperties(inObj);

	std::string meshFile;
	if (JsonHelper::GetString(inObj, "meshFile", meshFile))
		SetMesh(owner->GetGame()->GetRenderer()->GetMesh(meshFile));

	int idx;
	if (JsonHelper::GetInt(inObj, "textureIndex", idx))
		textureIndex = static_cast<size_t>(idx);

	JsonHelper::GetBool(inObj, "visible", visible);
	JsonHelper::GetBool(inObj, "isSkeletal", isSkeletal);
}

void MeshComponent::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const {
	Component::SaveProperties(alloc, inObj);

	if (mesh)
		JsonHelper::AddString(alloc, inObj, "meshFile", mesh->GetFileName());

	JsonHelper::AddInt(alloc, inObj, "textureIndex", textureIndex);
	JsonHelper::AddBool(alloc, inObj, "visible", visible);
	JsonHelper::AddBool(alloc, inObj, "isSkeletal", isSkeletal);
}
