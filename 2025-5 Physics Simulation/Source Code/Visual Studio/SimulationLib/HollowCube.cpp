#include "staticPch.h"
#include "HollowCube.h"
#include "Intersect.h"

using namespace DirectX;

bool HollowCube::testIntersect(const Capsule* other) const {
	return Intersect::testIntersect(*other, *this);
}
bool HollowCube::testIntersect(const HollowCube* other) const {
	return Intersect::testIntersect(*this, *other);
}
bool HollowCube::testIntersect(const Plane* other) const {
	return Intersect::testIntersect(*this, *other);
}
bool HollowCube::testIntersect(const Sphere* other) const {
	return Intersect::testIntersect(*this, *other);
}
bool HollowCube::testIntersect(const PhysicsObject* other) const {
	return other->testIntersect(this);
}

bool HollowCube::isInside(const XMVECTOR& point) const {
	if (signedPointDistance(point) >= 0)
		return false;
	return true;
}

// Signed distance from closest face
float HollowCube::signedPointDistance(const XMVECTOR& point) const {
	float closestDist;
	closestPlane(point, closestDist);
	return closestDist;
}

// This is a fairly hot function. If I had time I'd make plane into a light construct like LineSegment and Line, instead of a full physicsObject
Plane HollowCube::closestPlane(const XMVECTOR& point, float& closestDist) const {
	const XMVECTOR myPos = getPosition();
	const XMVECTOR myRotation = getRotation();

	// Cuts down calculations by half
	const bool topIsCloser = XMVectorGetY(point) > XMVectorGetY(myPos);
	const bool rightIsCloser = XMVectorGetX(point) > XMVectorGetX(myPos);
	const bool backIsCloser = XMVectorGetZ(point) > XMVectorGetZ(myPos);

	std::unique_ptr<Plane> closestPlane;
	{
		if (topIsCloser)
			closestPlane = std::make_unique<Plane>(faceToPlane(top));
		else
			closestPlane = std::make_unique<Plane>(faceToPlane(bottom));
		closestDist = closestPlane->signedPointDistance(point);
	}
	{
		std::unique_ptr<Plane> newPlane;
		if (rightIsCloser)
			newPlane = std::make_unique<Plane>(faceToPlane(right));
		else
			newPlane = std::make_unique<Plane>(faceToPlane(left));
		float newDist = newPlane->signedPointDistance(point);

		if (abs(newDist) < abs(closestDist)) {
			closestDist = newDist;
			closestPlane = std::move(newPlane);
		}
	}
	{
		std::unique_ptr<Plane> newPlane;
		if (backIsCloser)
			newPlane = std::make_unique<Plane>(faceToPlane(back));
		else
			newPlane = std::make_unique<Plane>(faceToPlane(front));
		float newDist = newPlane->signedPointDistance(point);

		if (abs(newDist) < abs(closestDist)) {
			closestDist = newDist;
			closestPlane = std::move(newPlane);
		}
	}
	return *closestPlane;
}

Plane HollowCube::faceToPlane(Face face) const {
	XMVECTOR rotationQuat;
	XMVECTOR positionOffset;
	switch (face) {
	case top:
		rotationQuat = XMQuaternionIdentity();
		positionOffset = XMVectorSet(0, _sideLength / 2.0f, 0, 0);
		break;
	case bottom:
		rotationQuat = XMQuaternionRotationNormal(XMVectorSet(1, 0, 0, 0), XM_PI);
		positionOffset = XMVectorSet(0, -_sideLength / 2.0f, 0, 0);
		break;
	case left:
		rotationQuat = XMQuaternionRotationNormal(XMVectorSet(0, 0, 1, 0), XM_PIDIV2);
		positionOffset = XMVectorSet(-_sideLength / 2.0f, 0, 0, 0);
		break;
	case right:
		rotationQuat = XMQuaternionRotationNormal(XMVectorSet(0, 0, 1, 0), -XM_PIDIV2);
		positionOffset = XMVectorSet(_sideLength / 2.0f, 0, 0, 0);
		break;
	case front:
		rotationQuat = XMQuaternionRotationNormal(XMVectorSet(1, 0, 0, 0), -XM_PIDIV2);
		positionOffset = XMVectorSet(0, 0, -_sideLength / 2.0f, 0);
		break;
	case back:
		rotationQuat = XMQuaternionRotationNormal(XMVectorSet(1, 0, 0, 0), XM_PIDIV2);
		positionOffset = XMVectorSet(0, 0, _sideLength / 2.0f, 0);
		break;
	default:
#ifdef _DEBUG
		throw ShouldNotRunException();
#endif
		return Plane();
		break;
	}
	const XMVECTOR trueRotation = XMQuaternionMultiply(getRotation(), rotationQuat);
	PhysicsObject::constructParams params;
	params._rotation = trueRotation;
	params._position = positionOffset;
	return Plane(params);
}