#pragma once

#include "LineSegment.h"
#include "PhysicsObject.h"
#include <memory>

class Capsule : public PhysicsObject
{
	float _height = 1.0f;
	float _radius = 1.0f;
public:
	Capsule(const PhysicsObject::constructParams& params, float height, float radius) : PhysicsObject(params), _height(height), _radius(radius) { _myType = shapeHelpers::capsule; }
	Capsule(float height, float radius) : PhysicsObject(), _height(height), _radius(radius) { _myType = shapeHelpers::capsule; }
	Capsule() : PhysicsObject() { _myType = shapeHelpers::capsule; }

	bool testIntersect(const Capsule* other) const override;
	bool testIntersect(const HollowCube* other) const override;
	bool testIntersect(const Plane* other) const override;
	bool testIntersect(const Sphere* other) const override;
	bool testIntersect(const PhysicsObject* other) const override;
	std::unique_ptr<PhysicsObject> clone() const override { return std::make_unique<Capsule>(*this); }

	LineSegment getAxis() const;

	void setHeight(float height) { _height = height; }
	void setRadius(float radius) { _radius = radius; }
	float getHeight() const { return _height; }
	float getRadius() const { return _radius; }
};

