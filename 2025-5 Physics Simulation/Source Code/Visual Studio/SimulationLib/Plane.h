#pragma once

#include "PhysicsObject.h"
#include <memory>

class Plane final : public PhysicsObject
{
public:
	Plane(const PhysicsObject::constructParams& params) : PhysicsObject(params) { _myType = shapeHelpers::plane; _simpleInertia = true; }
	Plane() : PhysicsObject() { _myType = shapeHelpers::plane; _simpleInertia = true; }

	bool testIntersect(const Capsule* other) const override;
	bool testIntersect(const Plane* other) const override;
	bool testIntersect(const Sphere* other) const override;
	bool testIntersect(const PhysicsObject* other) const override;
	std::unique_ptr<PhysicsObject> clone() const override { return std::make_unique<Plane>(*this); }

	float pointDistance(const DirectX::XMVECTOR& point) const;
	float signedPointDistance(const DirectX::XMVECTOR& point) const;
	DirectX::XMVECTOR closestPointOnPlane(const DirectX::XMVECTOR& point) const;
};