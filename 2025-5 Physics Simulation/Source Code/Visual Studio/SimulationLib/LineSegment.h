#pragma once

#include <DirectXMath.h>

class LineSegment final{
	// _p2 and orientation must be changed together. Making this const so I read this comment when I try to change it
	const DirectX::XMMATRIX _orientation;
	const DirectX::XMVECTOR _p2;
public:
	LineSegment(const DirectX::XMVECTOR& p1, const DirectX::XMVECTOR& p2);
	LineSegment() : LineSegment(DirectX::XMVectorSet(0, 0, 0, 0), DirectX::XMVectorSet(1, 0, 0, 0)) {}

	float pointDistance(DirectX::XMVECTOR point) const;
	DirectX::XMVECTOR closestPointOnLineSegment(const DirectX::XMVECTOR& point) const;
	DirectX::XMVECTOR closestPointOnLineSegment(const DirectX::XMVECTOR& point, bool& onLine) const;
	DirectX::XMVECTOR closestPointOnLineSegmentUnbounded(const DirectX::XMVECTOR& point) const;
	DirectX::XMVECTOR closestPointOnLineSegmentUnbounded(const DirectX::XMVECTOR& point, bool& onLine) const;
	DirectX::XMVECTOR closestPointToOtherLineSegment(const LineSegment& other, bool& onLine) const;
	DirectX::XMVECTOR closestApproachToOtherLineSegment(const LineSegment& other) const;

	const DirectX::XMVECTOR& getPosition() const { return _orientation.r[3]; }
	DirectX::XMVECTOR getDirection() const { return XMVector3TransformNormal(DirectX::XMVectorSet(0, 1, 0, 0), _orientation); }
	const DirectX::XMVECTOR& getP2() const{ return _p2; }
};

