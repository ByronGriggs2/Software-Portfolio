#include "staticPch.h"
#include "Plane.h"
#include "Line.h"
#include "LineSegment.h"
#include "Intersect.h"

using namespace DirectX;

bool Plane::testIntersect(const Capsule* other) const {
	return Intersect::testIntersect(*other, *this);
}
bool Plane::testIntersect(const Plane* other) const {
	return Intersect::testIntersect(*this, *other);
}
bool Plane::testIntersect(const Sphere* other) const {
	return Intersect::testIntersect(*this, *other);
}
bool Plane::testIntersect(const PhysicsObject* other) const {
	return other->testIntersect(this);
}

float Plane::pointDistance(const XMVECTOR& point) const {
	const XMVECTOR myPosition = getPosition();
	const XMVECTOR myNormal = getDirection();

	const Line normalLine(myPosition, myNormal);
	const XMVECTOR closestPointOnNormal = normalLine.closestPointOnLine(point);
	const XMVECTOR normalLineSegment = XMVectorSubtract(closestPointOnNormal, myPosition);
	return XMVectorGetX(XMVector3Length(normalLineSegment));
}
float Plane::signedPointDistance(const XMVECTOR& point) const {
	const XMVECTOR myPosition = getPosition();
	const XMVECTOR myNormal = getDirection();

	const Line normalLine(myPosition, myNormal);
	const XMVECTOR closestPointOnNormal = normalLine.closestPointOnLine(point);
	const XMVECTOR normalLineSegment = XMVectorSubtract(closestPointOnNormal, myPosition);
	//the normal line segment will either be in the same direction as the normal or opposite
	//if opposite, the dot product ~= -1
	//if parallel, the dot product ~= 1
	if (XMVectorGetX(XMVector3Dot(normalLineSegment, myNormal)) < 0) {
		return -1.0f * XMVectorGetX(XMVector3Length(normalLineSegment));
	}
	return 1.0f * XMVectorGetX(XMVector3Length(normalLineSegment));
}

XMVECTOR Plane::closestPointOnPlane(const XMVECTOR& point) const {
	const XMVECTOR myPosition = getPosition();
	const XMVECTOR myNormal = getDirection();

	const Line normalLine(myPosition, myNormal);
	const XMVECTOR closestPointOnNormal = normalLine.closestPointOnLine(point);
	const XMVECTOR normalLineSegment = XMVectorSubtract(closestPointOnNormal, myPosition);
	return XMVectorSubtract(point, normalLineSegment);
}

