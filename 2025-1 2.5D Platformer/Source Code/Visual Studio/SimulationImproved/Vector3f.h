//Only used by GenerateObject.h, to make outputting text to file and converting to and from XMVECTOR a bit cleaner

#pragma once

#include <iostream>
#include <fstream>
#include <DirectXMath.h>
using namespace DirectX;

class Vector3f final {
	float x, y, z;
public:
	Vector3f(float a, float b, float c) : x(a), y(b), z(c) {
	}
	Vector3f() = default;
	explicit Vector3f(const XMVECTOR& v) {
		XMFLOAT3 temp;
		XMStoreFloat3(&temp, v);
		x = temp.x; y = temp.y; z = temp.z;
	}

	void print(std::ofstream& ofs) const{
		ofs << x << " " << y << " " << z;
	}
	XMVECTOR directX() const {
		return XMVectorSet(x, y, z, 0.0);
	}
	void add(float a, float b, float c) {
		x += a;
		y += b;
		z += c;
	}
};