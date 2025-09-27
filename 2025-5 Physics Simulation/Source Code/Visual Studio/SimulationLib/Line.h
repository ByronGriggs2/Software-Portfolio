#pragma once

#include <DirectXMath.h>

class Line final {
public:
	enum constructionType {
		pointDirection,
		pointPoint
	};
private:
	DirectX::XMMATRIX _orientation;
public:
	Line(const DirectX::XMVECTOR& point, const DirectX::XMVECTOR& pointOrDirection, constructionType con);
	Line(const DirectX::XMVECTOR& point, const DirectX::XMVECTOR& direction) : Line(point, direction, pointDirection) {}
	Line() : Line(DirectX::XMVectorSet(0, 0, 0, 0), DirectX::XMVectorSet(1, 0, 0, 0), pointPoint) {}
	
	float pointDistance(const DirectX::XMVECTOR& point) const;
	DirectX::XMVECTOR closestPointOnLine(const DirectX::XMVECTOR& point) const;

	const DirectX::XMVECTOR& getPosition() const { return _orientation.r[3]; }
	DirectX::XMVECTOR getDirection() const { return DirectX::XMVector3TransformNormal(DirectX::XMVectorSet(0, 1, 0, 0), _orientation); }
};

