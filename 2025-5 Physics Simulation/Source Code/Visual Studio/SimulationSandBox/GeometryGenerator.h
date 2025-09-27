#pragma once

#include <vector>
#include <DirectXMath.h>
#include "definitions.h"

class ObjectGeometry;
namespace helpers {
	struct SimpleVertex;
}

/////////////////////
// Static Class

class GeometryGenerator
{
	static void textureTriangle(std::vector<helpers::SimpleVertex>& vertices, const std::vector<WORD>& indices);

public:
	GeometryGenerator() = delete;
	GeometryGenerator(const GeometryGenerator& other) = delete;
	GeometryGenerator& operator=(const GeometryGenerator& other) = delete;
	GeometryGenerator(const GeometryGenerator&& other) = delete;
	GeometryGenerator& operator=(const GeometryGenerator&&) = delete;

	static ObjectGeometry createRectangularPrism(float width, float height, float depth);
	static ObjectGeometry createSphere(float radius, int tesselation);
	static ObjectGeometry createPlane(float scale);
	static ObjectGeometry createCapsule(float height, float radius, int tesselation);
};

