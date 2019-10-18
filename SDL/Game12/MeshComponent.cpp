#include "MeshComponent.h"
#include "Shader.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"

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
	if (mesh) {
		shader->SetUniformValue("uWorldTransform", owner->GetWorldTransform());
		shader->SetUniformValue("uSpecularPower", mesh->GetSpecularPower());

		auto t = mesh->GetTexture(textureIndex);
		if (t) t->SetActive();

		auto vertexArray = mesh->GetVertexArray();
		vertexArray->SetActive();

		glDrawElements(GL_TRIANGLES, vertexArray->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}