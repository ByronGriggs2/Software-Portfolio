#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include "GenerateObj.h"
#include "helpers.h"

class ReadObj final{
	struct Face {
		struct faceData {
			int vertex;
			int texture;
			int normal;
		};
		std::vector<faceData> myData;
	};

	struct Obj {
		std::string type;
		std::vector<XMFLOAT3> vertexPos;
		std::vector<XMFLOAT2> vertexTex;
		std::vector<XMFLOAT3> vertexNorm;
		std::vector<Face> faceVector;
	};

	std::ifstream ifs;
	std::vector<Obj> objVector;
	static std::unique_ptr<ReadObj> _instance;

public:
	void read(const std::string& filename);
	void convert(std::vector<helpers::SimpleVertex>&, std::vector<WORD>&) const;
	static ReadObj& getInstance() { return *_instance; }
};