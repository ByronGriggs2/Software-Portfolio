#pragma once

#include "PhysicsObject.h"
#include <memory>
#include <DirectXMath.h>
#include "Plane.h"

class HollowCube final : public PhysicsObject
{
public:
	enum Face {
		top,
		bottom,
		left,
		right,
		front,
		back
	};
private:
	float _sideLength = 3.0f;

	Plane faceToPlane(Face) const;
public:
	HollowCube(const PhysicsObject::constructParams& params, float sideLength) : PhysicsObject(params), _sideLength(sideLength) {
		_myType = shapeHelpers::hollowCube;
		_fixed = true;
	}
	HollowCube() : PhysicsObject() {
		_myType = shapeHelpers::hollowCube;
		_fixed = true;
	}

	bool testIntersect(const Capsule* other) const override;
	bool testIntersect(const HollowCube* other) const override;
	bool testIntersect(const Plane* other) const override;
	bool testIntersect(const Sphere* other) const override;
	bool testIntersect(const PhysicsObject* other) const override;
	std::unique_ptr<PhysicsObject> clone() const override { return std::make_unique<HollowCube>(*this); }

	bool isInside(const DirectX::XMVECTOR& point) const;
	float signedPointDistance(const DirectX::XMVECTOR& point) const;
	Plane closestPlane(const DirectX::XMVECTOR& point, float& closestDist) const;

	void setSideLength(float length) { _sideLength = length; }
};

