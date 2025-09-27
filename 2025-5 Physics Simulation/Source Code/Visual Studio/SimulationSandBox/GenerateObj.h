#pragma once

#include <vector>
#include <memory>
#include <iomanip>
#include <DirectXMath.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <filesystem>
#include <fstream>

#include "helpers.h"

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
		std::string type;
		//std::string myDir;
		std::vector<XMVECTOR> myVertices;
		std::vector<Texture> myTextures;
		std::vector<XMVECTOR> myNormals;
		std::vector<Face> myFaces;
	};

	static std::unique_ptr<GenerateObj> _instance;
	std::vector<Obj> _objectsToGenerate;

	void printValues(std::ofstream& ofs, const Obj&) const;
	void addFaceData(Face& face, int vertex, int texture, int normal) const {
		face.myData.push_back({ vertex, texture, normal });
	}

	Obj createRectangularPrism(float width, float height, float depth) const;
	//Obj createInvertedRectangularPrism(float width, float height, float depth) const;
	//Obj createArrow(float width, float height, float depth) const;
	Obj createSphere(float radius, int tesselation) const;
	//Obj createTree(float trunkHeight) const;
	//Obj createCloud(float height) const;
	Obj createPlane(float scale) const;
	Obj createCapsule(float height, float radius, int tesselation) const;

	void textureSquare(Obj& obj) const;
	void textureTriangle(Obj& obj) const;
	public:
		void generate(const std::filesystem::path&);
		static GenerateObj& getInstance() { return *_instance; }
		void resetGenerator() {
			_objectsToGenerate.clear();
		}
		void translateLast(float x, float y, float z);

		void addRectangularPrism(float width, float height, float depth) { _objectsToGenerate.push_back(createRectangularPrism(width, height, depth)); }
		void addInvertedRectangularPrism(float width, float height, float depth) { _objectsToGenerate.push_back(createInvertedRectangularPrism(width, height, depth)); }
		void addArrow(float width, float height, float depth) { _objectsToGenerate.push_back(createArrow(width, height, depth)); }
		void addSphere(float radius, int tesselation) { _objectsToGenerate.push_back(createSphere(radius, tesselation)); }
		void addTree(float trunkHeight) { _objectsToGenerate.push_back(createTree(trunkHeight)); }
		void addCloud(float height) { _objectsToGenerate.push_back(createCloud(height)); }
		void addPlane(float scale) { _objectsToGenerate.push_back(createPlane(scale)); }
		void addCapsule(float height, float radius, int tesselation) { _objectsToGenerate.push_back(createCapsule(height, radius, tesselation)); }
};
