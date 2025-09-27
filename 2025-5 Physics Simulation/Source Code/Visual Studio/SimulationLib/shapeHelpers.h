#pragma once
#include <DirectXMath.h>
#include <cmath>
#include "definitions.h"

namespace shapeHelpers {
	enum ObjType {
		capsule,
		cylinder,
		hollowCube,
		plane, 
		sphere,
		physObj
	};

	inline float distance(const DirectX::XMVECTOR& p1, const DirectX::XMVECTOR& p2) {
		const DirectX::XMVECTOR difference = DirectX::XMVectorSubtract(p1, p2);
		return DirectX::XMVectorGetX(DirectX::XMVector3Length(difference));
	}

	inline bool vec3Equal(const DirectX::XMVECTOR& a, const DirectX::XMVECTOR& b) {
		DirectX::XMFLOAT3 temp1;
		DirectX::XMFLOAT3 temp2;
		XMStoreFloat3(&temp1, a);
		XMStoreFloat3(&temp2, b);
		return (abs(temp1.x - temp2.x) <= EPSILON) && (abs(temp1.y - temp2.y) <= EPSILON) &&
			(abs(temp1.z - temp2.z) <= EPSILON);
	}

	//direction is relative to (0,1,0)
	inline DirectX::XMMATRIX convertToTransformation(const DirectX::XMVECTOR& position, const DirectX::XMVECTOR& direction) {
		const DirectX::XMVECTOR normA = DirectX::XMVectorSet(0, 1, 0, 0);
		const DirectX::XMVECTOR normB = DirectX::XMVector3Normalize(direction);

		DirectX::XMVECTOR rotationAxis = DirectX::XMVector3Cross(normA, normB);
		if (DirectX::XMVector3Equal(rotationAxis, DirectX::XMVectorZero())) {
			const float yVal = DirectX::XMVectorGetY(normB);
			//parallel
			if (abs(yVal - 1) < EPSILON) {
				return DirectX::XMMatrixTranslationFromVector(position);
			}
			//opposite
			else {
				rotationAxis = DirectX::XMVectorSet(1, 0, 0, 0);
			}
		}
		const float angle = acosf(DirectX::XMVectorGetX(DirectX::XMVector3Dot(normA, normB)));
		const DirectX::XMVECTOR rotationQuaternion =  DirectX::XMQuaternionRotationAxis(rotationAxis, angle);
		const DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationQuaternion(rotationQuaternion);

		const DirectX::XMMATRIX positionMatrix = DirectX::XMMatrixTranslationFromVector(position);

		return XMMatrixMultiply(rotationMatrix, positionMatrix);
	}
	inline DirectX::XMVECTOR directionToQuaternion(const DirectX::XMVECTOR& direction) {
		const DirectX::XMVECTOR normA = DirectX::XMVectorSet(0, 1, 0, 0);
		const DirectX::XMVECTOR normB =  DirectX::XMVector3Normalize(direction);
		const DirectX::XMVECTOR rotationAxis =  DirectX::XMVector3Cross(normA, normB);
		if (DirectX::XMVector3Equal(rotationAxis, DirectX::XMVectorZero())) {
			const float yVal = DirectX::XMVectorGetY(normB);
			//parallel
			if (abs(yVal - 1) < EPSILON) {
				return  DirectX::XMQuaternionIdentity();
			}
			//opposite
			else {
				return  DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(1, 0, 0, 0), DirectX::XM_PI);
			}
		}
		const float angle = acosf(DirectX::XMVectorGetX(DirectX::XMVector3Dot(normA, normB)));
		return  DirectX::XMQuaternionRotationAxis(rotationAxis, angle);
	}
}