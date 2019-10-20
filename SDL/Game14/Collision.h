#pragma once

#include <vector>
#include "Math.h"

struct LineSegment {
	LineSegment(const Vector3& inStart, const Vector3& inEnd);

	Vector3 PointOnSegment(float t) const;
	float MinDistanceSquared(const Vector3& point) const;
	static float MinDistanceSquared(const LineSegment& lhs, const LineSegment& rhs);

	Vector3 start;
	Vector3 end;
};

struct Plane {
	Plane(const Vector3& inNormal, float inD);
	Plane(const Vector3& a, const Vector3& b, const Vector3& c);

	float SignedDistance(const Vector3& point) const;

	Vector3 normal;
	float d;
};

struct Sphere {
	Sphere(const Vector3& inCenter, float inRadius);

	bool Contains(const Vector3& point) const;

	Vector3 center;
	float radius;
};

struct AABB {
	AABB(const Vector3& inMin, const Vector3& inMax);

	void UpdateMinMax(const Vector3& point);
	
	void Rotate(const Quaternion& rotation);
	bool Contains(const Vector3& point) const;
	float MinDistanceSquared(const Vector3& point) const;

	Vector3 min;
	Vector3 max;
};

struct OBB {
	OBB(const Vector3& inCenter, const Quaternion& inRotation, const Vector3& inExtents);

	Vector3 center;
	Quaternion rotation;
	Vector3 extents;
};

struct Capsule {
	Capsule(const Vector3& start, const Vector3& end, float inRadius);

	Vector3 PointOnSegment(float t) const;
	bool Contains(const Vector3& point) const;

	LineSegment segment;
	float radius;
};

struct ConvexPolygon {
	ConvexPolygon(const std::vector<Vector2>& inVertices);
	ConvexPolygon(std::vector<Vector2>&& inVertices);
	
	bool Contains(const Vector2& point) const;

	std::vector<Vector2> vertices;
};

bool Intersect(const Sphere& a, const Sphere& b);
bool Intersect(const AABB& a, const AABB& b);
bool Intersect(const Capsule& a, const Capsule& b);
bool Intersect(const Sphere& s, const AABB& box);

bool Intersect(const LineSegment& l, const Sphere& s, float& outT);
bool Intersect(const LineSegment& l, const Plane& p, float& outT);
bool Intersect(const LineSegment& l, const AABB& b, float& outT, Vector3& outNorm);

bool SweptSphere(const Sphere& P0, const Sphere& P1, const Sphere& Q0, const Sphere& Q1, float& outT);