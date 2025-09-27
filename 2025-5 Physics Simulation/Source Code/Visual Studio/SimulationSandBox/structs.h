#pragma once

#include <atlbase.h>
#include <d3d11.h>

namespace helpers {
	enum IntegrationType {
		euler,
		semiEuler,
		runge_kutta
	};

	struct alignas(16) SimpleVertex {
		DirectX::XMVECTOR Pos;
		DirectX::XMVECTOR Norm;
		DirectX::XMVECTOR Tex;
	};

	struct RenderedObjectStruct {
		int _sphere = 0;
		int _capsule = 0;
		int _hollowCube = 0;

		void reset() {
			_sphere = 0;
			_capsule = 0;
			_hollowCube = 0;
		}
		RenderedObjectStruct& operator +=(const RenderedObjectStruct& other) {
			_sphere += other._sphere;
			_capsule += other._capsule;
			_hollowCube += other._hollowCube;
			return *this;
		}
	};

	struct CapsuleInit {
		float _radius = 1;
		float _height = 1;
		int _tesselation = 4;
	};

	struct HollowCubeInit {
		float _sideLength = 1;
	};

	struct PlaneInit {
	};

	struct SphereInit {
		float _radius = -1;
		int _tesselation = 4;
	};

	struct SpecialInit {
		CapsuleInit _capsule;
		HollowCubeInit _hollowCube;
		PlaneInit _plane;
		SphereInit _sphere;
	};
}