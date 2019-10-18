#include "SkeletalMeshComponent.h"
#include "Shader.h"
#include "Mesh.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"
#include "Animation.h"
#include "Skeleton.h"

SkeletalMeshComponent::SkeletalMeshComponent(Actor* inOwner)
	: MeshComponent(inOwner),
	palette(),
	skeleton(nullptr),
	animation(nullptr),
	animPlayRate(0.0f),
	animTime(0.0f) {

	inOwner->GetGame()->GetRenderer()->RemoveMeshComponent(this);
	inOwner->GetGame()->GetRenderer()->AddMeshComponent(this);
}

void SkeletalMeshComponent::Update(const float deltaTime) {
	if (animation && skeleton) {
		animTime += animPlayRate * deltaTime;
		while (animTime > animation->GetDuration())
			animTime -= animation->GetDuration();

		ComputeMatrixPalette();
	}
}

void SkeletalMeshComponent::Draw(Shader* shader) {
	if (!mesh) return;
	
	shader->SetUniformValue("uWorldTransform", owner->GetWorldTransform());
	shader->SetUniformValue("uMatrixPalette", &palette[0], MAX_SKELETON_BONES);
	shader->SetUniformValue("uSpecularPower", mesh->GetSpecularPower());

	const auto t = mesh->GetTexture(textureIndex);
	if (t) t->SetActive();

	const auto va = mesh->GetVertexArray();
	va->SetActive();

	glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
}

float SkeletalMeshComponent::PlayAnimation(const Animation* anim, const float playRate /*= 1.0f*/) {
	animation = anim;
	animPlayRate = playRate;
	animTime = 0.0f;

	if (!animation) return 0.0f;

	ComputeMatrixPalette();
	return animation->GetDuration();
}

void SkeletalMeshComponent::ComputeMatrixPalette() {
	const auto& globalInvBindPoses = skeleton->GetGlobalInvBindPoses();
	const auto currentPoses = animation->GetGlobalPoseAtTime(skeleton, animTime);

	for (size_t i = 0; i < skeleton->GetNumBones(); ++i)
		palette[i] = globalInvBindPoses[i] * currentPoses[i];
}