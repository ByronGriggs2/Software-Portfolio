#include "staticPch.h"
#include "LineSegment.h"

using namespace DirectX;

LineSegment::LineSegment(const DirectX::XMVECTOR& p1, const DirectX::XMVECTOR& p2) : _orientation(shapeHelpers::convertToTransformation(p1, p2 - p1)), _p2(p2) {};

float LineSegment::pointDistance(XMVECTOR point) const {
	XMVECTOR projectedPoint = closestPointOnLineSegment(point);
	//This line is in projectedPoint but the value is thrown away so inefficient omg
	XMVECTOR projection = XMVectorSubtract(projectedPoint, point);
	return XMVectorGetX(XMVector3Length(projection));
}

XMVECTOR LineSegment::closestPointOnLineSegment(const XMVECTOR& point) const {
	const XMVECTOR p1 = getPosition();
	const XMVECTOR direction = getDirection();

	const XMVECTOR hypotenuse = XMVectorSubtract(point, p1);
	const float AdjLength = XMVectorGetX(XMVector3Dot(hypotenuse, direction));
	//projection of point is before start point
	if (AdjLength < 0) {
		return p1;
	}
	//projection of point is after end point
	else if (AdjLength > XMVectorGetX(XMVector3Length(XMVectorSubtract(_p2, p1)))) {
		return _p2;
	}
	//projection is on line segment
	else {
		XMVECTOR projectedPoint = p1;
		projectedPoint += direction * AdjLength;
		return projectedPoint;
	}
}
XMVECTOR LineSegment::closestPointOnLineSegment(const XMVECTOR& point, bool& onLine) const {
	const XMVECTOR p1 = getPosition();
	const XMVECTOR direction = getDirection();

	const XMVECTOR hypotenuse = XMVectorSubtract(point, p1);
	const float AdjLength = XMVectorGetX(XMVector3Dot(hypotenuse, direction));
	//projection of point is before start point
	if (AdjLength < 0) {
		onLine = false;
		return p1;
	}
	//projection of point is after end point
	else if (AdjLength > XMVectorGetX(XMVector3Length(XMVectorSubtract(_p2, p1)))) {
		onLine = false;
		return _p2;
	}
	//projection is on line segment
	else {
		onLine = true;
		XMVECTOR projectedPoint = p1;
		projectedPoint += direction * AdjLength;
		return projectedPoint;
	}
}

// Treats line segment as line
XMVECTOR LineSegment::closestPointOnLineSegmentUnbounded(const XMVECTOR& point) const {
	const XMVECTOR p1 = getPosition();
	const XMVECTOR direction = getDirection();

	const XMVECTOR hypotenuse = XMVectorSubtract(point, p1);
	const float AdjLength = XMVectorGetX(XMVector3Dot(hypotenuse, direction));
	XMVECTOR projectedPoint = p1;
	projectedPoint += direction * AdjLength;
	return projectedPoint;
}
XMVECTOR LineSegment::closestPointOnLineSegmentUnbounded(const XMVECTOR& point, bool&) const {
	const XMVECTOR p1 = getPosition();
	const XMVECTOR direction = getDirection();

	const XMVECTOR hypotenuse = XMVectorSubtract(point, p1);
	const float AdjLength = XMVectorGetX(XMVector3Dot(hypotenuse, direction));
	XMVECTOR projectedPoint = p1;
	projectedPoint += direction * AdjLength;
	return projectedPoint;
}

XMVECTOR LineSegment::closestPointToOtherLineSegment(const LineSegment& other, bool& onSegment) const {
	const XMVECTOR closestApproach = closestApproachToOtherLineSegment(other);
	const XMVECTOR closestOnSegment = closestPointOnLineSegment(closestApproach, onSegment);
	return closestOnSegment;
}

XMVECTOR LineSegment::closestApproachToOtherLineSegment(const LineSegment& other) const {
	const XMVECTOR otherDir = other.getDirection();
	const XMVECTOR otherStart = other.getPosition();
	const XMVECTOR myDir = getDirection();
	const XMVECTOR myStart = getPosition();

	const XMVECTOR U1CrossU2 = XMVector3Cross(otherDir, myDir);
	const XMVECTOR BADiff = otherStart - myStart;

	const XMVECTOR numerator = XMVector3Dot(XMVector3Cross(BADiff, otherDir), U1CrossU2);
	const float denominator = XMVectorGetX(XMVector3Length(U1CrossU2));

	return myStart + myDir * (numerator / denominator);
}
