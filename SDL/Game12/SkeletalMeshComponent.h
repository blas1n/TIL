#pragma once

#include "MeshComponent.h"
#include "MatrixPalette.h"

class SkeletalMeshComponent : public MeshComponent {
public:
	SkeletalMeshComponent(Actor* inOwner);

	void Update(float deltaTime) override;
	void Draw(Shader* shader) override;
	
	float PlayAnimation(const class Animation* anim, float playRate = 1.0f);

	inline void SetSkeleton(const class Skeleton* inSkeleton) { skeleton = inSkeleton; }
	
protected:
	void ComputeMatrixPalette();

	MatrixPalette palette;

	const Skeleton* skeleton;
	const Animation* animation;

	float animPlayRate;
	float animTime;
};

