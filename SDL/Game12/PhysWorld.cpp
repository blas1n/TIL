#include "PhysWorld.h"

PhysWorld::PhysWorld(Game* inGame)
	: game(inGame),
	boxes() {}

bool PhysWorld::SegmentCast(const LineSegment& l, CollisionInfo& outColl) {
	auto collided = false;
	auto closestT = Math::Inf;
	Vector3 norm;

	for (const auto box : boxes) {
		auto t = 0.0f;
		if (Intersect(l, box->GetWorldBox(), t, norm)) {
			if (t < closestT) {
				outColl.point = l.PointOnSegment(t);
				outColl.normal = norm;
				outColl.box = box;
				outColl.actor = box->GetOwner();
				collided = true;
			}
		}
	}

	return collided;
}

void PhysWorld::AddBox(BoxComponent* box) {
	boxes.emplace_back(box);
}

void PhysWorld::RemoveBox(BoxComponent* box) {
	auto iter = std::find(boxes.begin(), boxes.end(), box);
	if (iter != boxes.end()) {
		std::iter_swap(iter, boxes.end() - 1);
		boxes.pop_back();
	}
}