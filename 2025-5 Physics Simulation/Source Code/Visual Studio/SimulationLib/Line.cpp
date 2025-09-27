#include "staticPch.h"
#include "Line.h"

using namespace DirectX;

Line::Line(const XMVECTOR& point, const XMVECTOR& pointOrDirection, constructionType con) {
	XMVECTOR direction;
	if (con == pointDirection) {

		direction = pointOrDirection;
	}
	else {
		direction = XMVector3Normalize(point - pointOrDirection);
	}
	_orientation = shapeHelpers::convertToTransformation(point, direction);
}

float Line::pointDistance(const XMVECTOR& point) const {
	const XMVECTOR projectedPoint = closestPointOnLine(point);
	const XMVECTOR projection = XMVectorSubtract(projectedPoint, point);
	return XMVectorGetX(XMVector3Length(projection));
}

XMVECTOR Line::closestPointOnLine(const XMVECTOR& point) const {
	const XMVECTOR myPosition = getPosition();
	const XMVECTOR myDirection = getDirection();

	const XMVECTOR hypotenuse = XMVectorSubtract(point, myPosition);
	const float AdjLength = XMVectorGetX(XMVector3Dot(hypotenuse, myDirection));
	const XMVECTOR projectedPoint = myPosition + myDirection * AdjLength;
	return projectedPoint;
}