#include "PointLightComponent.h"
#include "Shader.h"
#include "Game.h"
#include "Renderer.h"
#include "Actor.h"
#include "VertexArray.h"
#include "Mesh.h"

PointLightComponent::PointLightComponent(Actor* inOwner)
	: Component(inOwner), diffuseColor(), innerRadius(0.0f), outerRadius(0.0f) {

	owner->GetGame()->GetRenderer()->AddPointLight(this);
}

PointLightComponent::~PointLightComponent() {
	owner->GetGame()->GetRenderer()->RemovePointLight(this);
}

void PointLightComponent::Draw(Shader* shader, Mesh* mesh) {
	const auto scale = Matrix4::CreateScale(owner->GetScale() * outerRadius / mesh->GetRadius());
	const auto trans = Matrix4::CreateTranslation(owner->GetPosition());

	shader->SetUniformValue("uWorldTransform", scale * trans);

	shader->SetUniformValue("uPointLight.worldPos", owner->GetPosition());
	shader->SetUniformValue("uPointLight.diffuseColor", diffuseColor);
	shader->SetUniformValue("uPointLight.innerRadius", innerRadius);
	shader->SetUniformValue("uPointLight.outerRadius", outerRadius);

	glDrawElements(GL_TRIANGLES, mesh->GetVertexArray()->GetNumIndices(),
		GL_UNSIGNED_INT, nullptr);
}