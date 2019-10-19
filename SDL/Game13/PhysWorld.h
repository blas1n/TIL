#pragma once

#include <vector>
#include <algorithm>
#include "Math.h"
#include "Collision.h"
#include "BoxComponent.h"

class PhysWorld {
public:
	PhysWorld(class Game* inGame);

	struct CollisionInfo {
		CollisionInfo() = default;

		Vector3 point;
		Vector3 normal;
		BoxComponent* box;
		class Actor* actor;
	};

	bool SegmentCast(const LineSegment& l, CollisionInfo& outColl);

	template <class Func>
	void TestPairwise(Func&& f);

	template <class Func>
	void TestSweepAndPrune(Func&& f);

	void AddBox(BoxComponent* box);
	void RemoveBox(BoxComponent* box);

private:
	Game* game;
	std::vector<BoxComponent*> boxes;
};

template <class Func>
void PhysWorld::TestPairwise(Func&& f) {
	for (const auto a : boxes) {
		for (const auto b : boxes) {
			if (Intersect(a->GetWorldBox(), b->GetWorldBox()))
				f(a->GetOwner(), b->GetOwner());
		}
	}
}

template <class Func>
void PhysWorld::TestSweepAndPrune(Func&& f) {
	std::sort(boxes.begin(), boxes.end(), [](const auto lhs, const auto rhs) {
		return lhs->GetWorldBox().min.x < rhs->GetWorldBox().min.x;
	});

	for (const auto a : boxes) {
		const auto max = a->GetWorldBox().max.x;

		for (const auto b : boxes) {
			if (b->GetWorldBox().min.x > max
				&& Intersect(a->GetWorldBox(), b->GetWorldBox()))
				f(a->GetOwner(), b->GetOwner());
		}
	}
}