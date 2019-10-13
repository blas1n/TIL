#include "SpriteComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Shader.h"
#include "Texture.h"

SpriteComponent::SpriteComponent(Actor* inOwner, const int inDrawOrder)
	: Component(inOwner),
	texture(nullptr),
	drawOrder(inDrawOrder),
	width(0),
	height(0) {

	owner->GetGame()->AddSprite(this);
}

SpriteComponent::~SpriteComponent() {
	owner->GetGame()->RemoveSprite(this);
}

void SpriteComponent::Draw(Shader* shader) {
	if (texture) {
		const auto imageScale = Matrix4::CreateScale(
			static_cast<float>(width),
			static_cast<float>(height),
			1.0f
		);

		const auto transform = imageScale * owner->GetWorldTransform();
		shader->SetMatrixUniform("uWorldTransform", transform);
		texture->SetActive();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}
}

void SpriteComponent::SetTexture(Texture* inTexture) {
	texture = inTexture;
	width = texture->GetWidth();
	height = texture->GetHeight();
}
