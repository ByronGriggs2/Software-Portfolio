#pragma once

#include <vector>
#include "definitions.h"
#include "DirectXMath.h"

namespace helpers {
	struct SimpleVertex;
}

class ObjectGeometry {
	std::vector<helpers::SimpleVertex> _vertices;
	std::vector<WORD> _indices;

public:
	ObjectGeometry(std::string type);
	ObjectGeometry();

	int indexCount() const { return static_cast<int>(_indices.size()); }
	int vertexCount() const { return static_cast<int>(_vertices.size()); }

	void setVertices(const std::vector<helpers::SimpleVertex>& v) {
		_vertices = v;
	}
	void setIndices(const std::vector <WORD>& i) {
		_indices = i;
	}
	const std::vector<helpers::SimpleVertex>& getVertices() const { return _vertices; }
	const std::vector<WORD>& getIndices() const { return _indices; }
};