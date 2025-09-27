#pragma once

#include "PhysicsObject.h"
#include <memory>

class Sphere final : public PhysicsObject
{
	float _radius = 1.0f;
public:
	Sphere(const PhysicsObject::constructParams& params, float radius);
	Sphere(float radius);
	Sphere() : Sphere(1.0f) {}

	bool testIntersect(const Capsule* other) const override;
	bool testIntersect(const Plane* other) const override;
	bool testIntersect(const Sphere* other) const override;
	bool testIntersect(const HollowCube* other) const override;
	bool testIntersect(const PhysicsObject* other) const override;
	std::unique_ptr<PhysicsObject> clone() const override { return std::make_unique<Sphere>(*this); }

	void setRadius(float radius) { _radius = radius; }
	float getRadius() const { return _radius; }
};

