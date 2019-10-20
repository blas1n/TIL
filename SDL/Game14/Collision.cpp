#include "Collision.h"
#include <algorithm>
#include <array>

LineSegment::LineSegment(const Vector3& inStart, const Vector3& inEnd)
	: start(inStart), end(inEnd) {}

Vector3 LineSegment::PointOnSegment(const float t) const {
	return start + (end - start) * t;
}

float LineSegment::MinDistanceSquared(const Vector3& point) const {
	const auto ab = end - start;
	const auto ac = point - start;
	const auto bc = point - end;

	if (Vector3::Dot(ab, ac) < 0.0f)
		return ac.LengthSquared();
	if (Vector3::Dot(-ab, bc) < 0.0f)
		return bc.LengthSquared();

	const auto scalar = Vector3::Dot(ac, ab) / ab.LengthSquared();
	return (ac - scalar * ab).LengthSquared();
}

float LineSegment::MinDistanceSquared(const LineSegment& lhs, const LineSegment& rhs) {
	const auto u = lhs.end - lhs.start;
	const auto v = rhs.end - rhs.start;
	const auto w = lhs.start - rhs.start;

	const auto a = Vector3::Dot(u, u);
	const auto b = Vector3::Dot(u, v);
	const auto c = Vector3::Dot(v, v);
	const auto d = Vector3::Dot(u, w);
	const auto e = Vector3::Dot(v, w);
	const auto D = a * c - b * b;

	auto sN = 0.0f, sD = D;
	auto tN = 0.0f, tD = D;

	if (Math::NearZero(D)) {
		sN = 0.0f;
		sD = 1.0f;
		tN = e;
		tD = c;
	}
	else {
		sN = (b * e - c * d);
		tN = (a * e - b * d);

		if (sN < 0.0f) {
			sN = 0.0f;
			tN = e;
			tD = c;
		}
		else if (sN > sD) {
			sN = sD;
			tN = e + b;
			tD = c;
		}
	}

	if (tN < 0.0f) {
		tN = 0.0f;

		if (d > 0.0f)
			sN = 0.0f;
		else if (-d > a)
			sN = sD;
		else {
			sN = -d;
			sD = a;
		}
	}
	else if (tN > tD) {
		tN = tD;
		const auto bMinusD = b - d;
		if (bMinusD < 0.0f)
			sN = 0.0f;
		else if (bMinusD > a)
			sN = sD;
		else {
			sN = bMinusD;
			sD = a;
		}
	}

	const auto sc = (Math::NearZero(sN) ? 0.0f : sN / sD);
	const auto tc = (Math::NearZero(tN) ? 0.0f : tN / tD);

	const auto dP = w + (sc * u) - (tc * v);
	return dP.LengthSquared();
}

Plane::Plane(const Vector3& inNormal, const float inD)
	: normal(inNormal), d(inD) {}

Plane::Plane(const Vector3& a, const Vector3& b, const Vector3& c) {
	const auto ab = b - a;
	const auto ac = c - a;

	normal = Vector3::Cross(ab, ac);
	normal.Normalized();

	d = -Vector3::Dot(a, normal);
}

float Plane::SignedDistance(const Vector3& point) const {
	return Vector3::Dot(point, normal) - d;
}

Sphere::Sphere(const Vector3& inCenter, const float inRadius)
	: center(inCenter), radius(inRadius) {}

bool Sphere::Contains(const Vector3& point) const {
	const auto distSq = (center - point).LengthSquared();
	return distSq <= (radius * radius);
}

AABB::AABB(const Vector3& inMin, const Vector3& inMax)
	: min(inMin), max(inMax) {}

void AABB::UpdateMinMax(const Vector3& point) {
	min.x = Math::Min(min.x, point.x);
	min.y = Math::Min(min.y, point.y);
	min.z = Math::Min(min.z, point.z);

	max.x = Math::Max(max.x, point.x);
	max.y = Math::Max(max.y, point.y);
	max.z = Math::Max(max.z, point.z);
}

void AABB::Rotate(const Quaternion& rotation) {
	Vector3 points[8]{
		min,
		Vector3{ max.x, min.y, min.z },
		Vector3{ min.x, max.y, min.z },
		Vector3{ min.x, min.y, max.z },
		Vector3{ min.x, max.y, max.z },
		Vector3{ max.x, min.y, max.z },
		Vector3{ max.x, max.y, min.z },
		max
	};

	auto p = Vector3::Transform(points[0], rotation);
	min = max = p;

	for (auto i = 1; i < 8; ++i) {
		p = Vector3::Transform(points[i], rotation);
		UpdateMinMax(p);
	}
}

bool AABB::Contains(const Vector3& point) const
{
	return
		point.x >= min.x &&
		point.y >= min.y &&
		point.z >= min.z &&
		point.x <= max.x &&
		point.y <= max.y &&
		point.z <= max.z;
}

float AABB::MinDistanceSquared(const Vector3& point) const {
	auto v = Vector3::Zero;
	v.x = Math::Max(min.x - point.x, v.x);
	v.x = Math::Max(point.x - max.x, v.x);

	v.y = Math::Max(min.y - point.y, v.y);
	v.y = Math::Max(point.y - max.y, v.y);

	v.z = Math::Max(min.z - point.z, v.z);
	v.z = Math::Max(point.z - max.z, v.z);
	return v.LengthSquared();
}

OBB::OBB(const Vector3& inCenter, const Quaternion& inRotation, const Vector3& inExtents)
	: center(inCenter), rotation(inRotation), extents(inExtents) {}

Capsule::Capsule(const Vector3& start, const Vector3& end, float inRadius)
	: segment(start, end), radius(inRadius) {}

Vector3 Capsule::PointOnSegment(const float t) const {
	return segment.PointOnSegment(t);
}

bool Capsule::Contains(const Vector3& point) const {
	const auto distSq = segment.MinDistanceSquared(point);
	return distSq <= (radius * radius);
}

bool ConvexPolygon::Contains(const Vector2& point) const {
	auto sum = 0.0f;
	Vector2 a, b;

	for (size_t i = 0; i < vertices.size() - 1; i++) {
		a = vertices[i] - point;
		a.Normalized();

		b = vertices[i + 1] - point;
		b.Normalized();

		sum += Math::Acos(Vector2::Dot(a, b));
	}

	a = vertices.back() - point;
	a.Normalized();

	b = vertices.front() - point;
	b.Normalized();

	sum += Math::Acos(Vector2::Dot(a, b));
	return Math::NearZero(sum - Math::Pi * 2.0f);
}

ConvexPolygon::ConvexPolygon(const std::vector<Vector2>& inVertices) 
	: vertices(inVertices) {}

ConvexPolygon::ConvexPolygon(std::vector<Vector2>&& inVertices)
	: vertices(std::move(inVertices)) {}

bool Intersect(const Sphere& a, const Sphere& b) {
	const auto distSq = (a.center - b.center).LengthSquared();
	const auto sumRadii = a.radius + b.radius;
	return distSq <= (sumRadii * sumRadii);
}

bool Intersect(const AABB& a, const AABB& b) {
	return
		a.max.x >= b.min.x &&
		a.max.y >= b.min.y &&
		a.max.z >= b.min.z &&
		a.min.x <= b.max.x &&
		a.min.y <= b.max.y &&
		a.min.z <= b.max.z;
}

bool Intersect(const Capsule& a, const Capsule& b) {
	const auto distSq =
		LineSegment::MinDistanceSquared(a.segment, b.segment);
	float sumRadii = a.radius + b.radius;
	return distSq <= (sumRadii * sumRadii);
}

bool Intersect(const Sphere& s, const AABB& box) {
	const auto distSq = box.MinDistanceSquared(s.center);
	return distSq <= (s.radius * s.radius);
}

bool Intersect(const LineSegment& l, const Sphere& s, float& outT) {
	const auto x = l.start - s.center;
	const auto y = l.end - l.start;

	const auto a = y.LengthSquared();
	const auto b = 2.0f * Vector3::Dot(x, y);
	const auto c = x.LengthSquared() - s.radius * s.radius;

	float disc = b * b - 4.0f * a * c;
	if (disc < 0.0f)
		return false;

	disc = Math::Sqrt(disc);

	const auto tMin = (-b - disc) / (2.0f * a);
	const auto tMax = (-b + disc) / (2.0f * a);
	if (tMin >= 0.0f && tMin <= 1.0f) {
		outT = tMin;
		return true;
	}
	else if (tMax >= 0.0f && tMax <= 1.0f) {
		outT = tMax;
		return true;
	}
	else
		return false;
}

bool Intersect(const LineSegment& l, const Plane& p, float& outT) {
	const auto denom = Vector3::Dot(l.end - l.start, p.normal);

	if (Math::NearZero(denom))
		return (Math::NearZero(Vector3::Dot(l.start, p.normal) - p.d));
	
	const auto numer = -Vector3::Dot(l.start, p.normal) - p.d;
	outT = numer / denom;
	return (outT >= 0.0f && outT <= 1.0f);
}

bool TestSidePlane(const float start, const float end, const float negd,
	const Vector3& norm, std::vector<std::pair<float, Vector3>>& out) {

	const auto denom = end - start;
	if (Math::NearZero(denom))
		return false;

	const auto numer = negd - start;
	const auto t = numer / denom;

	if (t >= 0.0f && t <= 1.0f) {
		out.emplace_back(t, norm);
		return true;
	}

	return false;
}

bool Intersect(const LineSegment& l, const AABB& b, float& outT, Vector3& outNorm) {
	std::vector<std::pair<float, Vector3>> tValues;
	TestSidePlane(l.start.x, l.end.x, b.min.x, -Vector3::UnitX, tValues);
	TestSidePlane(l.start.x, l.end.x, b.max.x, Vector3::UnitX, tValues);
	TestSidePlane(l.start.y, l.end.y, b.min.y, -Vector3::UnitY, tValues);
	TestSidePlane(l.start.y, l.end.y, b.max.y, Vector3::UnitY, tValues);
	TestSidePlane(l.start.z, l.end.z, b.min.z, -Vector3::UnitZ, tValues);
	TestSidePlane(l.start.z, l.end.z, b.max.z, Vector3::UnitZ, tValues);

	std::sort(tValues.begin(), tValues.end(), [](const auto& lhs, const auto& rhs) {
		return lhs.first < rhs.first;
	});

	for (const auto& t : tValues) {
		const auto point = l.PointOnSegment(t.first);
		if (b.Contains(point)) {
			outT = t.first;
			outNorm = t.second;
			return true;
		}
	}

	return false;
}

bool SweptSphere(const Sphere& P0, const Sphere& P1, const Sphere& Q0, const Sphere& Q1, float& outT) {
	const auto x = P0.center - P0.center;
	const auto y = (P1.center - P0.center) - (Q1.center - Q0.center);

	const auto a = y.LengthSquared();
	const auto b = 2.0f * Vector3::Dot(x, y);
	const auto sumRadius = P0.radius + Q0.radius;
	const auto c = x.LengthSquared() - sumRadius * sumRadius;

	const auto disc = b * b - 4.0f * a * c;
	if (disc < 0.0f) return false;

	outT = (-b - disc) / (2.0f * a);
	return outT >= 0.0f && outT <= 1.0f;
}