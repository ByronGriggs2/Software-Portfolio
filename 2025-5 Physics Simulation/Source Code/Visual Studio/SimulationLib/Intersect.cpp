#include "staticPch.h"
#include "Intersect.h"
#include "LineSegment.h"
#include "Capsule.h"
#include "HollowCube.h"
#include "Plane.h"
#include "Sphere.h"

using namespace DirectX;
using namespace shapeHelpers;

bool Intersect::testIntersect(const Capsule& c1, const Capsule& c2) {
	const float c1Radius = c1.getRadius();
	const LineSegment c1Axis = c1.getAxis();
	const XMVECTOR c1P1 = c1Axis.getPosition();
	const XMVECTOR c1P2 = c1Axis.getP2();
	const float c2Radius = c2.getRadius();
	const LineSegment c2Axis = c2.getAxis();
	const XMVECTOR c2P1 = c2Axis.getPosition();
	const XMVECTOR c2P2 = c2Axis.getPosition();

	const XMVECTOR closestC1P1 = c2Axis.closestPointOnLineSegment(c1P1);
	if (distance(closestC1P1, c1P1) <= c1Radius + c2Radius)
		return true;
	const XMVECTOR closestC1P2 = c2Axis.closestPointOnLineSegment(c1P2);
	if (distance(closestC1P2, c1P2) <= c1Radius + c2Radius)
		return true;
	return false;
}

bool Intersect::testIntersect(const Capsule& cap, const HollowCube& cube) {
	const XMVECTOR capPosOld = cap.getPrevPosition();
	const XMVECTOR capPos = cap.getPosition();
	const float capHeight = cap.getHeight();
	const float capRadius = cap.getRadius();
	const XMVECTOR capDirOld = cap.getPrevDirection();
	const XMVECTOR capDir = cap.getDirection();

	const XMVECTOR P1Old = capPosOld + capDirOld * (-capHeight / 2.0f);
	const XMVECTOR P1New = capPos + capDir * (-capHeight / 2.0f);
	const float P1DistOld = cube.signedPointDistance(P1Old);
	const float P1DistNew = cube.signedPointDistance(P1New);
	if (abs(P1DistNew) < capRadius)
		return true;

	if (P1DistOld * P1DistNew < 0)
		return true;

	const XMVECTOR P2Old = capPosOld + capDirOld * capHeight / 2.0f;
	const XMVECTOR P2New = capPos + capDir * capHeight / 2.0f;
	const float P2DistOld = cube.signedPointDistance(P2Old);
	const float P2DistNew = cube.signedPointDistance(P2New);
	if (abs(P2DistNew) < capRadius)
		return true;

	if (P2DistOld * P2DistNew < 0)
		return true;

	float throwaway;
	{
		const Plane temp = cube.closestPlane(P1New, throwaway);
		if (cap.testIntersect(&temp))
			return true;
	}
	{
		const Plane temp = cube.closestPlane(P2New, throwaway);
		if (cap.testIntersect(&temp))
			return true;
	}
	return false;
}

bool Intersect::testIntersect(const Capsule& cap, const Plane& plane) {
	const float capHeight = cap.getHeight();
	const XMMATRIX capOrientation = cap.getOrientation();
	const XMVECTOR capP1 = XMVector3Transform(XMVectorSet(0, capHeight / 2.0f, 0, 0), capOrientation);
	const XMVECTOR capP2 = XMVector3Transform(XMVectorSet(0, -capHeight / 2.0f, 0, 0), capOrientation);
	const float distance1 = plane.signedPointDistance(capP1);
	const float distance2 = plane.signedPointDistance(capP2);

	//through plane
	if (distance1 * distance2 < 0) {
		return true;
	}

	const float closerDist = abs(distance1) < abs(distance2) ? abs(distance1) : abs(distance2);
	return closerDist <= cap.getRadius();
}

bool Intersect::testIntersect(const Capsule& cap, const Sphere& sphere) {
	const float capHeight = cap.getHeight();
	const XMMATRIX capOrientation = cap.getOrientation();
	const XMVECTOR spherePos = sphere.getPosition();

	const XMVECTOR capP1 = XMVector3Transform(XMVectorSet(0, capHeight / 2.0f, 0, 0), capOrientation);
	const XMVECTOR capP2 = XMVector3Transform(XMVectorSet(0, -capHeight / 2.0f, 0, 0), capOrientation);
	const LineSegment capAxis(capP1, capP2);
	const XMVECTOR closestPoint = capAxis.closestPointOnLineSegment(spherePos);
	return distance(closestPoint, spherePos) <= cap.getRadius() + sphere.getRadius();
}

bool Intersect::testIntersect(const HollowCube& c1, const HollowCube& c2) {
#ifdef _DEBUG
	throw NotImplementedException();
#endif
	return false;
}

bool Intersect::testIntersect(const HollowCube& cube, const Plane& plane) {
#ifdef _DEBUG
	throw NotImplementedException();
#endif
	return false;
}

bool Intersect::testIntersect(const HollowCube& cube, const Sphere& sphere) {
	const float sphereRadius = sphere.getRadius();

	const XMVECTOR& spherePosNew = sphere.getPosition();
	const float newDist = cube.signedPointDistance(spherePosNew);
	if (abs(newDist) <= sphereRadius)
		return true;

	const XMVECTOR& spherePosOld = sphere.getPrevPosition();
	const float oldDist = cube.signedPointDistance(spherePosOld);
	
	return newDist * oldDist < 0;
}

bool Intersect::testIntersect(const Plane& p1, const Plane& p2) {
	const XMVECTOR norm1 = p1.getDirection();
	const XMVECTOR norm2 = p2.getDirection();

	return !vec3Equal(norm1, norm2);
}

bool Intersect::testIntersect(const Plane& plane, const Sphere& sphere) {
	return plane.pointDistance(sphere.getPosition()) <= sphere.getRadius();
}

bool Intersect::testIntersect(const Sphere& s1, const Sphere& s2) {
	float distance = shapeHelpers::distance(s1.getPosition(), s2.getPosition());
	return distance <= s1.getRadius() + s2.getRadius();
}


