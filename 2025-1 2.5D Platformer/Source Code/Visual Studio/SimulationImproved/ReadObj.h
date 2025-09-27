//Given a filename and vertex and index lists (by reference), this singleton populates vertex and index vectors by parsing a .obj file

#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
//for SimpleVector
#include "CompleteObject.h"
#include "GenerateObj.h"

class ReadObj final{
	struct Face {
		struct faceData {
			int vertex;
			int texture;
			int normal;
		};
		std::vector<faceData> myData;
	};

	//Distinct from GenerateObj::Obj and CompleteObject::CompleteObject, this struct organizes data during parsing and vertex/index list setting
	struct Obj {
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
	void convert(CompleteObject&)const;
	static ReadObj& getInstance() { return *_instance; }
};