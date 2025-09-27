//This class creates all the .obj files needed for the project. It should not need to run, as the objects have already been generated.
//However if there is an error in reading in the .obj files they will all be generated again

//Note that the classes/structures in this class have no direct relation to those used in the rest of the program. For example, class Obj
//is an internal data structure used for organizing data before outputting it into the obj file. Obj never interacts with the rest of the program.
#pragma once

#include <vector>
#include <memory>
#include <iomanip>
#include <DirectXMath.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <filesystem>
#include "Vector3f.h"

using namespace DirectX;

class GenerateObj final{
	struct Texture {
		float u, v;
		Texture(float a, float b) : u(a), v(b) {
		}
		Texture() = default;
	};

	struct Face {
		struct faceData {
			int vertex;
			int texture;
			int normal;
		};
		std::vector<faceData> myData;
	};

	struct info {
		int face;
		int vertex;
		int texture;
		int normal;
	};

	struct Obj {
		std::string myName;
		std::string myDir;
		std::vector<Vector3f> myVertices;
		std::vector<Texture> myTextures;
		std::vector<Vector3f> myNormals;
		std::vector<Face> myFaces;
	};

	static std::unique_ptr<GenerateObj> _instance;
	std::vector<Obj> objectsToGenerate;

	Obj createRectangularPrism(float width, float height, float depth) const;
	Obj createArrow(float width, float height, float depth) const;
	Obj createAndOutputPlayer();
	Obj createSphere(float radius, int tesselation) const;
	Obj createTree(float trunkHeight) const;
	Obj createCloud(float height) const;
	Obj createFire(float scale) const;
	void printValues(std::ofstream& ofs, const Obj&) const;
	void generateSingle(Obj& obj, const std::string& folder, const std::string& name);
	void addFaceData(Face& face, int vertex, int texture, int normal) const {
		face.myData.push_back({ vertex, texture, normal });
	}
	void translate(Obj&, float a, float b, float c) const;
	public:
		void generate();
		static GenerateObj& getInstance() { return *_instance; }
};
