#include "pch.h"
#include "GeometryGenerator.h"
#include "ObjectGeometry.h"

using namespace DirectX;
using namespace helpers;

void GeometryGenerator::textureTriangle(std::vector<SimpleVertex>& vertices, const std::vector<WORD>& indices) {
	auto index = [indices](int i) {
		return static_cast<int>(indices[i]);
		};

	for (int i = 0; i < indices.size()-2; i += 3) {
		vertices[index(i)].Tex = XMVectorSet(0, 1, 0, 0);
		vertices[index(i + 1)].Tex = XMVectorSet(1, 1, 0, 0);
		vertices[index(i + 2)].Tex = XMVectorSet(0.5, 0, 0, 0);
	}
}

struct info {
	int vertex;
	int texture;
	int normal;
};
ObjectGeometry GeometryGenerator::createRectangularPrism(float width, float height, float depth) {
	ObjectGeometry tempGeometry;
	std::vector<SimpleVertex> simpleVertices;
	
	XMVECTOR vertices[8];
	vertices[0] = XMVectorSet(-width / 2, height / 2, -depth / 2, 0);
	vertices[1] = XMVectorSet(width / 2, height / 2, -depth / 2, 0);
	vertices[2] = XMVectorSet(width / 2, height / 2, depth / 2, 0);
	vertices[3] = XMVectorSet(-width / 2, height / 2, depth / 2, 0);
	vertices[4] = XMVectorSet(-width / 2, -height / 2, -depth / 2, 0);
	vertices[5] = XMVectorSet(width / 2, -height / 2, -depth / 2, 0);
	vertices[6] = XMVectorSet(width / 2, -height / 2, depth / 2, 0);
	vertices[7] = XMVectorSet(-width / 2, -height / 2, depth / 2, 0);
	XMVECTOR textures[4];
	textures[0] = XMVectorSet(0, 0, 0, 0);
	textures[1] = XMVectorSet(0, 1, 0, 0);
	textures[2] = XMVectorSet(1, 1, 0, 0);
	textures[3] = XMVectorSet(1, 0, 0, 0);
	XMVECTOR normals[6];
	normals[0] = XMVectorSet(0, 1, 0, 0);
	normals[1] = XMVectorSet(0, 0, -1, 0);
	normals[2] = XMVectorSet(1, 0, 0, 0);
	normals[3] = XMVectorSet(0, 0, 1, 0);
	normals[4] = XMVectorSet(0, -1, 0, 0);
	normals[5] = XMVectorSet(0, -1, 0, 0);
	info infos[24] = {
		{0,0,0},
		{1,1,0},
		{2,2,0},
		{3,3,0},
		{0,0,1},
		{4,1,1},
		{5,2,1},
		{1,3,1},
		{1,0,2},
		{5,1,2},
		{6,2,2},
		{2,3,2},
		{2,0,3},
		{6,1,3},
		{7,2,3},
		{3,3,3},
		{3,0,4},
		{7,1,4},
		{4,2,4},
		{0,3,4},
		{7,0,5},
		{6,1,5},
		{5,2,5},
		{4,3,5}
	};
	for (auto& info : infos) {
		simpleVertices.push_back({ vertices[info.vertex], normals[info.normal], textures[info.texture] });
	}
	tempGeometry.setVertices(simpleVertices);
	
	std::vector<WORD> indices;
	for (auto i = 0; i < 6; i++) {
		indices.push_back(static_cast<WORD>(4 * i));
		indices.push_back(static_cast<WORD>(4 * i + 1));
		indices.push_back(static_cast<WORD>(4 * i + 2));

		indices.push_back(static_cast<WORD>(4 * i));
		indices.push_back(static_cast<WORD>(4 * i + 2));
		indices.push_back(static_cast<WORD>(4 * i + 3));
	}
	tempGeometry.setIndices(indices);
	return tempGeometry;
}

ObjectGeometry GeometryGenerator::createSphere(float radius, int tesselation) {
	ObjectGeometry tempGeometry;
	std::vector<SimpleVertex> simpleVertices;
	const auto PI = 3.1415926535989732364f;
	const auto STACKCOUNT = tesselation;
	const auto SLICECOUNT = tesselation;
	const float PHISTEP = PI / STACKCOUNT;
	const float THETASTEP = (2.0f * PI) / SLICECOUNT;

	// Top vertex
	simpleVertices.push_back({ XMVectorSet(0.0f, radius, 0.0f, 0),
		XMVectorSet(0, 1, 0, 0),
		XMVectorSet(-1, -1, 0, 0) });

	// Middle
	for (auto i = 1; i < STACKCOUNT; i++) {
		const float phi = i * PHISTEP;
		for (auto j = 0; j < SLICECOUNT; j++) {
			const float theta = j * THETASTEP;
			XMVECTOR temp = XMVectorSet(radius * sinf(phi) * cosf(theta), radius * cosf(phi), radius * sinf(phi) * sinf(theta), 0);
			simpleVertices.push_back({ temp,XMVector3Normalize(temp), XMVectorSet(-1,-1,0,0) });
		}
	}

	// Bottom vertex
	simpleVertices.push_back({ XMVectorSet(0.0f, -radius, 0.0f, 0),
		XMVectorSet(0, -1, 0, 0),
		XMVectorSet(-1, -1, 0, 0) });
	
	//////
	// Indices
	std::vector<WORD> indices;
	// Top ring
	for (auto j = 0; j < SLICECOUNT; j++) {
		indices.push_back(static_cast<WORD>(0));
		indices.push_back(static_cast<WORD>(1 + j));
		if (j == SLICECOUNT - 1)
			indices.push_back(static_cast<WORD>(1));
		else
			indices.push_back(static_cast<WORD>(2 + j));
	}
	// Middle indices
	for (auto i = 1; i < STACKCOUNT - 1; i++) {
		for (auto j = 0; j < SLICECOUNT; j++) {
			// Bottom heavy triangle
			indices.push_back(static_cast<WORD>(1 + j + (i - 1) * SLICECOUNT));
			indices.push_back(static_cast<WORD>(1 + j + i * SLICECOUNT));
			if (j == SLICECOUNT - 1)
				indices.push_back(static_cast<WORD>(1 + i * SLICECOUNT));
			else
				indices.push_back(static_cast<WORD>(2 + j + i * SLICECOUNT));
			// Top heavy triangle
			indices.push_back(static_cast<WORD>(1 + j + (i - 1) * SLICECOUNT));
			
			if (j == SLICECOUNT - 1) {
				indices.push_back(static_cast<WORD>(1 + i * SLICECOUNT));
				indices.push_back(static_cast<WORD>(1 + (i - 1) * SLICECOUNT));
			}
			else {
				indices.push_back(static_cast<WORD>(2 + j + i * SLICECOUNT));
				indices.push_back(static_cast<WORD>(2 + j + (i - 1) * SLICECOUNT));
			}
		}
	}
	// Bottom ring
	const int final = static_cast<int>(simpleVertices.size())-1;
	for (auto j = 0; j < SLICECOUNT; j++) {
		const int firstInRing = final - SLICECOUNT;
		indices.push_back(static_cast<WORD>(firstInRing+j));
		indices.push_back(static_cast<WORD>(final));
		if (j == SLICECOUNT - 1)
			indices.push_back(static_cast<WORD>(firstInRing));
		else
			indices.push_back(static_cast<WORD>(firstInRing + j + 1));
	}
	// Use indices to set texture coordinates
	textureTriangle(simpleVertices, indices);
	tempGeometry.setVertices(simpleVertices);
	tempGeometry.setIndices(indices);

	//TESTING
	//for (auto& vertex : simpleVertices) {
	//	float dist = shapeHelpers::distance(vertex.Pos, XMVectorZero());
	//	if (abs(dist - radius) > EPSILON)
	//		myExcept("Sphere generated too large");
	//}

	return tempGeometry;
}

ObjectGeometry GeometryGenerator::createPlane(float scale) {
	ObjectGeometry tempGeometry;
	std::vector<SimpleVertex> simpleVertices;
	std::vector<WORD> indices;

	for (auto i = 0; i < 100; i++) {
		for (auto j = 0; j < 100; j++) {
			simpleVertices.push_back({ XMVectorSet(scale * j - scale * 50, 0, scale * i - scale * 50, 0),
				XMVectorSet(0,1,0,0),
				XMVectorSet(j/99.0f,i/99.0f,0,0) });
		}
	}

	for (auto i = 0; i < 99; i++) {
		for (auto j = 0; j < 99; j++) {
			indices.push_back(static_cast<WORD>(j + 100 * i));
			indices.push_back(static_cast<WORD>(j + 100 * (i+1)));
			indices.push_back(static_cast<WORD>(1 + j + 100 * (i+1)));
			indices.push_back(static_cast<WORD>(j + 100 * i));
			indices.push_back(static_cast<WORD>(1 + j + 100 * (i + 1)));
			indices.push_back(static_cast<WORD>(1 + j + 100 * i));
		}
	}

	tempGeometry.setVertices(simpleVertices);
	tempGeometry.setIndices(indices);
	return tempGeometry;
}

ObjectGeometry GeometryGenerator::createCapsule(float height, float radius, int tesselation) {
	if (tesselation % 2 != 0)
		tesselation += 1;
	const auto PI = 3.14159265359897f;
	const auto STACKCOUNT = tesselation;
	const auto SLICECOUNT = tesselation;
	const float PHISTEP = PI / STACKCOUNT;
	const float THETASTEP = (2.0f * PI) / SLICECOUNT;

	ObjectGeometry tempGeometry;
	std::vector<SimpleVertex> simpleVertices;

	// Top vertex
	simpleVertices.push_back({ XMVectorSet(0.0f, radius + height / 2.0f, 0.0f, 0),
		XMVectorSet(0, 1, 0, 0),
		XMVectorSet(-1, -1, 0, 0) });
	// Top hemisphere
	for (auto i = 1; i < STACKCOUNT/2+1; i++) {
		const float phi = i * PHISTEP;
		for (auto j = 0; j < SLICECOUNT; j++) {
			const float theta = j * THETASTEP;
			XMVECTOR temp = XMVectorSet(radius * sinf(phi) * cosf(theta), radius * cosf(phi) + height / 2.0f, radius * sinf(phi) * sinf(theta), 0);
			//if (i == STACKCOUNT / 2) {
			//	XMFLOAT3 directionFloat;
			//	XMStoreFloat3(&directionFloat, temp);
			//	const XMVECTOR radialDirection = XMVector3Normalize(XMVectorSet(directionFloat.x, 0, directionFloat.y, 0));
			//	simpleVertices.push_back({ temp, radialDirection, XMVectorSet(-1, -1, 0, 0) });
			//}
			//else
				simpleVertices.push_back({ temp,XMVector3Normalize(temp), XMVectorSet(-1,-1,0,0) });
		}
	}
	// Bottom hemisphere
	for (auto i = STACKCOUNT/2; i < STACKCOUNT; i++) {
		const float phi = i * PHISTEP;
		for (auto j = 0; j < SLICECOUNT; j++) {
			const float theta = j * THETASTEP;
			XMVECTOR temp = XMVectorSet(radius * sinf(phi) * cosf(theta), radius * cosf(phi) - height / 2.0f, radius * sinf(phi) * sinf(theta), 0);
			//if (i == STACKCOUNT / 2) {
			//	XMFLOAT3 directionFloat;
			//	XMStoreFloat3(&directionFloat, temp);
			//	const XMVECTOR radialDirection = XMVector3Normalize(XMVectorSet(directionFloat.x, 0, directionFloat.y, 0));
			//	simpleVertices.push_back({ temp, radialDirection, XMVectorSet(-1, -1, 0, 0) });
			//}
			//else
				simpleVertices.push_back({ temp, XMVector3Normalize(temp), XMVectorSet(-1,-1,0,0) });
		}
	}
	// Bottom vertex
	simpleVertices.push_back({ XMVectorSet(0.0f, -radius - height / 2.0f, 0.0f, 0),
		XMVectorSet(0, -1, 0, 0),
		XMVectorSet(-1, -1, 0, 0) });

	///////
	// Indices
	std::vector<WORD> indices;
	// Top ring
	for (auto j = 0; j < SLICECOUNT; j++) {
		indices.push_back(static_cast<WORD>(0));
		indices.push_back(static_cast<WORD>(1 + j));
		if (j == SLICECOUNT - 1)
			indices.push_back(static_cast<WORD>(1));
		else
			indices.push_back(static_cast<WORD>(2 + j));
	}
	// Top hemisphere
	for (auto i = 1; i < STACKCOUNT/2; i++) {
		for (auto j = 0; j < SLICECOUNT; j++) {
			// Bottom heavy triangle
			indices.push_back(static_cast<WORD>(1 + j + (i - 1) * SLICECOUNT));
			indices.push_back(static_cast<WORD>(1 + j + i * SLICECOUNT));
			if (j == SLICECOUNT - 1)
				indices.push_back(static_cast<WORD>(1 + i * SLICECOUNT));
			else
				indices.push_back(static_cast<WORD>(2 + j + i * SLICECOUNT));
			// Top heavy triangle
			indices.push_back(static_cast<WORD>(1 + j + (i - 1) * SLICECOUNT));
			
			if (j == SLICECOUNT - 1) {
				indices.push_back(static_cast<WORD>(1 + i * SLICECOUNT));
				indices.push_back(static_cast<WORD>(1 + (i - 1) * SLICECOUNT));
			}
			else {
				indices.push_back(static_cast<WORD>(2 + j + i * SLICECOUNT));
				indices.push_back(static_cast<WORD>(2 + j + (i - 1) * SLICECOUNT));
			}
		}
	}
	// Middle bit
	{
		const auto offset = (STACKCOUNT / 2 - 1) * SLICECOUNT;
		for (auto i = 0; i < SLICECOUNT; i++) {
			// Bottom heavy triangle
			indices.push_back(static_cast<WORD>(1 + i + offset));
			indices.push_back(static_cast<WORD>(1 + i + offset + SLICECOUNT));
			if (i == SLICECOUNT - 1) {
				indices.push_back(static_cast<WORD>(1 + offset + SLICECOUNT));
			}
			else
				indices.push_back(static_cast<WORD>(2 + i + offset + SLICECOUNT));
			// Top heavy triangle
			indices.push_back(static_cast<WORD>(1 + i + offset));
			if (i == SLICECOUNT - 1) {
				indices.push_back(static_cast<WORD>(1 + offset + SLICECOUNT));
				indices.push_back(static_cast<WORD>(1 + offset));
			}
			else {
				indices.push_back(static_cast<WORD>(2 + i + offset + SLICECOUNT));
				indices.push_back(static_cast<WORD>(2 + i + offset));
			}
		}
	}
	// Bottom hemisphere
	for (auto i = STACKCOUNT/2+1; i < STACKCOUNT; i++) {
		for (auto j = 0; j < SLICECOUNT; j++) {
			// Bottom heavy triangle
			indices.push_back(static_cast<WORD>(1 + j + (i - 1) * SLICECOUNT));
			indices.push_back(static_cast<WORD>(1 + j + i * SLICECOUNT));
			if (j == SLICECOUNT - 1)
				indices.push_back(static_cast<WORD>(1 + i * SLICECOUNT));
			else
				indices.push_back(static_cast<WORD>(2 + j + i * SLICECOUNT));
			// Top heavy triangle
			indices.push_back(static_cast<WORD>(1 + j + (i - 1) * SLICECOUNT));

			if (j == SLICECOUNT - 1) {
				indices.push_back(static_cast<WORD>(1 + i * SLICECOUNT));
				indices.push_back(static_cast<WORD>(1 + (i - 1) * SLICECOUNT));
			}
			else {
				indices.push_back(static_cast<WORD>(2 + j + i * SLICECOUNT));
				indices.push_back(static_cast<WORD>(2 + j + (i - 1) * SLICECOUNT));
			}
		}
	}
	// Bottom ring
	const int final = static_cast<int>(simpleVertices.size()) - 1;
	for (auto j = 0; j < SLICECOUNT; j++) {
		const int firstInRing = final - SLICECOUNT;
		indices.push_back(static_cast<WORD>(firstInRing + j));
		indices.push_back(static_cast<WORD>(final));
		if (j == SLICECOUNT - 1)
			indices.push_back(static_cast<WORD>(firstInRing));
		else
			indices.push_back(static_cast<WORD>(firstInRing + j + 1));
	}
	// Use indices to set texture coordinates
	textureTriangle(simpleVertices, indices);
	tempGeometry.setVertices(simpleVertices);
	tempGeometry.setIndices(indices);

	return tempGeometry;
}

