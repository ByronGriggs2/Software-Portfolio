#include "staticPch.h"
#include "Sphere.h"
#include "Intersect.h"

using namespace DirectX;
using namespace shapeHelpers;

bool Sphere::testIntersect(const Capsule* other) const {
	return Intersect::testIntersect(*other, *this);
}
bool Sphere::testIntersect(const Plane* other) const {
	return Intersect::testIntersect(*other, *this);
}
bool Sphere::testIntersect(const Sphere* other) const {
	return Intersect::testIntersect(*this, *other);
}
bool Sphere::testIntersect(const HollowCube* other) const {
	return Intersect::testIntersect(*other, *this);
}
bool Sphere::testIntersect(const PhysicsObject* other) const {
	return other->testIntersect(this);
}

Sphere::Sphere(const PhysicsObject::constructParams& params, float radius) : PhysicsObject(params), _radius(radius) {
	_myType = sphere;
	_simpleInertia = true;
	float inertia = 2.0f / 5.0f * getMass() * radius * radius;
	_inertia = XMMatrixSet(inertia, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	_inverseInertia = XMMatrixSet(1.0f / inertia, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}

Sphere::Sphere(float radius) : PhysicsObject(), _radius(radius) {
	_myType = sphere;
	_simpleInertia = true;
	float inertia = 2.0f / 5.0f * 1.0f * radius * radius;
	_inertia = XMMatrixSet(inertia, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	_inverseInertia = XMMatrixSet(1.0f / inertia, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}