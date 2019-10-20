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
#include "JsonHelper.h"

SkeletalMeshComponent::SkeletalMeshComponent(Actor* inOwner)
	: MeshComponent(inOwner, true),
	palette(),
	skeleton(nullptr),
	animation(nullptr),
	animPlayRate(0.0f),
	animTime(0.0f) {}

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

void SkeletalMeshComponent::LoadProperties(const rapidjson::Value& inObj) {
	MeshComponent::LoadProperties(inObj);

	std::string skelFile;
	if (JsonHelper::GetString(inObj, "skelFile", skelFile))
		SetSkeleton(owner->GetGame()->GetRenderer()->GetSkeleton(skelFile));

	std::string animFile;
	if (JsonHelper::GetString(inObj, "animFile", animFile))
		PlayAnimation(owner->GetGame()->GetRenderer()->GetAnimation(animFile));

	JsonHelper::GetFloat(inObj, "animPlayRate", animPlayRate);
	JsonHelper::GetFloat(inObj, "animTime", animTime);
}

void SkeletalMeshComponent::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const {
	MeshComponent::SaveProperties(alloc, inObj);

	if (skeleton)
		JsonHelper::AddString(alloc, inObj, "skelFile", skeleton->GetFileName());

	if (animation)
		JsonHelper::AddString(alloc, inObj, "animFile", animation->GetFileName());

	JsonHelper::AddFloat(alloc, inObj, "animPlayRate", animPlayRate);
	JsonHelper::AddFloat(alloc, inObj, "animTime", animTime);
}

void SkeletalMeshComponent::ComputeMatrixPalette() {
	const auto& globalInvBindPoses = skeleton->GetGlobalInvBindPoses();
	const auto currentPoses = animation->GetGlobalPoseAtTime(skeleton, animTime);

	for (size_t i = 0; i < skeleton->GetNumBones(); ++i)
		palette[i] = globalInvBindPoses[i] * currentPoses[i];
}