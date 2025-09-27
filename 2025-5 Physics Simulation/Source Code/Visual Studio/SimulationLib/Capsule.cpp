#include "staticPch.h"
#include "Capsule.h"
#include "Intersect.h"
#include "LineSegment.h"

using namespace DirectX;

LineSegment Capsule::getAxis() const {
	const DirectX::XMVECTOR cylPos = getPosition();
	const DirectX::XMVECTOR cylDir = getDirection();
	return LineSegment(cylPos - cylDir * _height / 2.0, cylPos + cylDir * _height / 2.0);
}

bool Capsule::testIntersect(const Capsule* other) const {
	return Intersect::testIntersect(*this, *other);
}
bool Capsule::testIntersect(const HollowCube* other) const {
	return Intersect::testIntersect(*this, *other);
}
bool Capsule::testIntersect(const Plane* other) const {
	return Intersect::testIntersect(*this, *other);
}
bool Capsule::testIntersect(const Sphere* other) const {
	return Intersect::testIntersect(*this, *other);
}

bool Capsule::testIntersect(const PhysicsObject* other) const {
	return other->testIntersect(this);
}