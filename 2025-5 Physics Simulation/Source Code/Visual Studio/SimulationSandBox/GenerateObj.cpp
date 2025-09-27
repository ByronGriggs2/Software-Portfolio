#include "GenerateObj.h"
#include <iostream>

std::unique_ptr<GenerateObj> GenerateObj::_instance = std::make_unique<GenerateObj>();

void GenerateObj::generate(const std::filesystem::path& myPath) {
	//folders
	//std::filesystem::path tempPath = std::filesystem::current_path();
	//std::filesystem::create_directories(tempPath / "Assets");
	std::filesystem::create_directories(myPath);

	//generate basic objects
	for (auto i = 0; i < _objectsToGenerate.size(); i++) {
		std::string temp;
		if (_objectsToGenerate[i].myName == "") {
			temp = "obj" + std::to_string(i) + ".obj";
			temp = (myPath / temp).string();
		}
		else {
			temp = _objectsToGenerate[i].myName + ".obj";
			temp = (myPath / temp).string();
		}
		std::ofstream ofs(temp);
		printValues(ofs, _objectsToGenerate[i]);
	}
	_objectsToGenerate.clear();

	std::string filename = (myPath / "complete.txt").string();
	std::ofstream ofs(filename);
	ofs << "all .obj files have been generated.";
}

void GenerateObj::printValues(std::ofstream& ofs, const GenerateObj::Obj& out) const {
#ifdef _DEBUG
	if (!ofs.is_open())
		std::cout << "Error in GenerateObj::printValues. File not open" << std::endl;
#endif
	ofs << "o " << out.type << std::endl;
	ofs << std::setprecision(6) << std::fixed;
	for (auto& i : out.myVertices) {
		ofs << "v ";
		helpers::printXMVECTOR(i, ofs);
		ofs << std::endl;
	}
	for (auto& i : out.myTextures) {
		ofs << "vt " << i.u << " " << i.v << std::endl;
	}
	for (auto& i : out.myNormals) {
		ofs << "vn ";
		helpers::printXMVECTOR(i, ofs);
		ofs << std::endl;
	}
	for (auto& i : out.myFaces) {
		ofs << "f ";
		for (const auto j : i.myData) {
			ofs << j.vertex << "/" << j.texture << "/" << j.normal << " ";
		}
		ofs << std::endl;
	}
}

GenerateObj::Obj GenerateObj::createArrow(float width, float height, float depth) const {
	Obj tempObj = GenerateObj::createRectangularPrism(width, height, depth);
	tempObj.type = "arrow";
	//delete top face
	tempObj.myFaces.erase(tempObj.myFaces.begin());

	XMVECTOR vertices[6];
	vertices[0] = XMVectorSet(-width, height / 2, -depth / 2, 0);
	vertices[1] = XMVectorSet(width, height / 2, -depth / 2, 0);
	vertices[2] = XMVectorSet(width, height / 2, depth / 2, 0);
	vertices[3] = XMVectorSet(-width, height / 2, depth / 2, 0);
	vertices[4] = XMVectorSet(0.0f, (height / 2) + width, -depth / 2, 0);
	vertices[5] = XMVectorSet(0.0f, (height / 2) + width, depth / 2, 0);
	for (const auto& i : vertices) {
		tempObj.myVertices.push_back(i);
	}

	Texture triangleTextures[3];
	triangleTextures[0] = Texture(0, 1);
	triangleTextures[1] = Texture(1, 1);
	triangleTextures[2] = Texture(0.5, 0);
	for (const auto& i : triangleTextures) {
		tempObj.myTextures.push_back(i);
	}

	//remove (0, 1, 0)
	tempObj.myNormals.erase(tempObj.myNormals.begin());
	XMVECTOR diagonals[2];
	diagonals[0] = XMVectorSet(-sqrt(2.0f), sqrt(2.0f), 0.0f, 0);
	diagonals[1] = XMVectorSet(sqrt(2.0f), sqrt(2.0f), 0.0f, 0);
	for (const auto& i : diagonals) {
		tempObj.myNormals.push_back(i);
	}

	Face faces[4];

	info infos[14] = {
		{0,10,6,2},
		{0,13,7,2},
		{0,9,5,2},
		{1,10,1,6},
		{1,11,2,6},
		{1,14,3,6},
		{1,13,4,6},
		{2,12,1,5},
		{2,9,2,5},
		{2,13,3,5},
		{2,14,4,5},
		{3,11,6,4},
		{3,12,7,4},
		{3,14,5,4}
	};

	for (auto i = 0; i < 14; i++) {
		addFaceData(faces[infos[i].face], infos[i].vertex, infos[i].texture, infos[i].normal);
	}

	for (const auto& i : faces) {
		tempObj.myFaces.push_back(i);
	}
	return tempObj;
}

GenerateObj::Obj GenerateObj::createRectangularPrism(float width, float height, float depth) const {
	Obj tempObj;
	tempObj.type = "rectangular_prism";
	XMVECTOR vertices[8];
	vertices[0] = XMVectorSet(-width/2, height/2, -depth/2, 0);
	vertices[1] = XMVectorSet(width/2, height/2, -depth/2, 0);
	vertices[2] = XMVectorSet(width/2, height/2, depth/2, 0);
	vertices[3] = XMVectorSet(-width/2, height/2, depth/2, 0);
	vertices[4] = XMVectorSet(-width/2, -height/2, -depth/2, 0);
	vertices[5] = XMVectorSet(width/2, -height/2, -depth/2, 0);
	vertices[6] = XMVectorSet(width/2, -height/2, depth/2, 0);
	vertices[7] = XMVectorSet(-width/2, -height/2, depth/2, 0);
	for (const auto& i : vertices) {
		tempObj.myVertices.push_back(i);
	}
	Texture textures[4];
	textures[0] = Texture(0, 1);
	textures[1] = Texture(1, 1);
	textures[2] = Texture(1, 0);
	textures[3] = Texture(0, 0);
	for (const auto& i : textures) {
		tempObj.myTextures.push_back(i);
	}

	XMVECTOR normals[6];
	normals[0] = XMVectorSet(0, 1, 0, 0);
	normals[1] = XMVectorSet(0, 0, -1, 0);
	normals[2] = XMVectorSet(1, 0, 0, 0);
	normals[3] = XMVectorSet(0, 0, 1, 0);
	normals[4] = XMVectorSet(0, -1, 0, 0);
	normals[5] = XMVectorSet(0, -1, 0, 0);
	for (const auto& i : normals) {
		tempObj.myNormals.push_back(i);
	}

	Face faces[6];

	info infos[24] = {
		{0,1,1,1},
		{0,2,2,1},
		{0,3,3,1},
		{0,4,4,1},
		{1,1,1,2},
		{1,5,2,2},
		{1,6,3,2},
		{1,2,4,2},
		{2,2,1,3},
		{2,6,2,3},
		{2,7,3,3},
		{2,3,4,3},
		{3,3,1,4},
		{3,7,2,4},
		{3,8,3,4},
		{3,4,4,4},
		{4,4,1,5},
		{4,8,2,5},
		{4,5,3,5},
		{4,1,4,5},
		{5,8,1,6},
		{5,7,2,6},
		{5,6,3,6},
		{5,5,4,6}
	};

	for (auto i = 0; i < 24; i++) {
		addFaceData(faces[infos[i].face], infos[i].vertex, infos[i].texture, infos[i].normal);
	}

	for (const auto& i : faces) {
		tempObj.myFaces.push_back(i);
	}
	return tempObj;
}

void GenerateObj::textureSquare(GenerateObj::Obj& obj) const {
	obj.myTextures.clear();
	obj.myTextures.push_back(Texture(0, 1));
	obj.myTextures.push_back(Texture(1, 1));
	obj.myTextures.push_back(Texture(1, 0));
	obj.myTextures.push_back(Texture(0, 0));

	for (auto& face : obj.myFaces) {
		if (face.myData.size() != 4)
			continue;
		face.myData[0].texture = 1;
		face.myData[1].texture = 2;
		face.myData[2].texture = 3;
		face.myData[3].texture = 4;
	}
}

void GenerateObj::textureTriangle(GenerateObj::Obj& obj) const {
	obj.myTextures.clear();
	obj.myTextures.push_back(Texture(0, 1));
	obj.myTextures.push_back(Texture(1, 1));
	obj.myTextures.push_back(Texture(0.5, 0));

	for (auto& face : obj.myFaces) {
		if (face.myData.size() != 3)
			continue;
		face.myData[0].texture = 1;
		face.myData[1].texture = 2;
		face.myData[2].texture = 3;
	}
}

GenerateObj::Obj GenerateObj::createInvertedRectangularPrism(float width, float height, float depth) const {
	Obj temp = createRectangularPrism(width, height, depth);
	temp.myTextures[0] = Texture(0, 0);
	temp.myTextures[1] = Texture(1, 0);
	temp.myTextures[2] = Texture(1, 1);
	temp.myTextures[3] = Texture(0, 1);

	return temp;
}

//GenerateObj::Obj GenerateObj::createHemisphere(float radius, int tesselation) const{
//	Obj tempObj;
//	tempObj.type = "hemisphere";
//	const auto PI = 3.14159265359897f;
//	const auto STACKCOUNT = tesselation / 2;
//	const auto SLICECOUNT = tesselation;
//	const float PHISTEP = PI / STACKCOUNT / 2.0f;
//	const float THETASTEP = (2.0f * PI) / SLICECOUNT;
//
//	//top vertex
//	tempObj.myVertices.push_back(XMVectorSet(0.0f, radius, 0.0f, 0));
//	tempObj.myNormals.push_back(XMVectorSet(0, 1, 0, 0));
//
//	//no texture
//	tempObj.myTextures.push_back(Texture(-1, -1));
//
//	for (auto i = 1; i < STACKCOUNT; i++) {
//		const float phi = i * PHISTEP;
//		for (auto j = 0; j < SLICECOUNT; j++) {
//			const float theta = j * THETASTEP;
//
//			tempObj.myVertices.push_back(XMVectorSet(radius * sinf(phi) * cosf(theta), radius * cosf(phi), radius * sinf(phi) * sinf(theta), 0));
//			tempObj.myNormals.push_back(XMVector3Normalize(tempObj.myVertices.back()));
//		}
//	}
//
//	//top ring
//	for (auto j = 0; j < SLICECOUNT; j++) {
//		Face tempFace;
//		tempFace.myData.push_back({ 1,1,1 });
//		tempFace.myData.push_back({ 2 + j,1,2 + j });
//		if (j == SLICECOUNT - 1) {
//			tempFace.myData.push_back({ 2,1,2 });
//		}
//		else {
//			tempFace.myData.push_back({ 3 + j,1,3 + j });
//		}
//		tempObj.myFaces.push_back(tempFace);
//	}
//
//	//i=1 because first stack, STACKCOUNT-1 because last stack
//	for (auto i = 1; i < STACKCOUNT - 1; i++) {
//		for (auto j = 0; j < SLICECOUNT; j++) {
//			Face tempFace;
//			//1 from array start 1, 1 from skip top vertex
//			tempFace.myData.push_back({ 2 + j + (i - 1) * SLICECOUNT,1, 2 + j + (i - 1) * SLICECOUNT });
//			tempFace.myData.push_back({ 2 + j + (i)*SLICECOUNT,1, 2 + j + i * SLICECOUNT });
//			if (j == SLICECOUNT - 1) {
//				tempFace.myData.push_back({ 2 + (i)*SLICECOUNT,1, 2 + j + i * SLICECOUNT });
//				tempFace.myData.push_back({ 2 + (i - 1) * SLICECOUNT,1, 2 + j + (i - 1) * SLICECOUNT });
//			}
//			else {
//				tempFace.myData.push_back({ 3 + j + (i)*SLICECOUNT,1, 2 + j + i * SLICECOUNT });
//				tempFace.myData.push_back({ 3 + j + (i - 1) * SLICECOUNT,1, 2 + j + (i - 1) * SLICECOUNT });
//			}
//
//			tempObj.myFaces.push_back(tempFace);
//		}
//	}
//}

GenerateObj::Obj GenerateObj::createSphere(float radius, int tesselation) const{
	Obj tempObj;
	tempObj.type = "sphere";
	const auto PI = 3.14159265359897f;
	const auto STACKCOUNT = tesselation;
	const auto SLICECOUNT = tesselation;
	const float PHISTEP = PI / STACKCOUNT;
	const float THETASTEP = (2.0f * PI) / SLICECOUNT;

	//top vertex
	tempObj.myVertices.push_back(XMVectorSet(0.0f, radius, 0.0f, 0));
	tempObj.myNormals.push_back(XMVectorSet(0, 1, 0, 0));

	//no texture
	tempObj.myTextures.push_back(Texture(-1,-1));

	for (auto i = 1; i < STACKCOUNT; i++) {
		const float phi = i * PHISTEP;
		for (auto j = 0; j < SLICECOUNT; j++) {
			const float theta = j * THETASTEP;

			tempObj.myVertices.push_back(XMVectorSet(radius * sinf(phi) * cosf(theta), radius * cosf(phi), radius * sinf(phi)*sinf(theta), 0));
			tempObj.myNormals.push_back(XMVector3Normalize(tempObj.myVertices.back()));
		}
	}

	//bottom vertex
	tempObj.myVertices.push_back(XMVectorSet(0.0f, -radius, 0.0f, 0));
	tempObj.myNormals.push_back(XMVectorSet(0, -1, 0, 0));

	//top ring
	for (auto j = 0; j < SLICECOUNT; j++) {
		Face tempFace;
		tempFace.myData.push_back({ 1,1,1 });
		tempFace.myData.push_back({ 2 + j,1,2 + j});
		if (j == SLICECOUNT - 1) {
			tempFace.myData.push_back({ 2,1,2 });
		}
		else {
			tempFace.myData.push_back({ 3 + j,1,3 + j });
		}
		tempObj.myFaces.push_back(tempFace);
	}
	//i=1 because first stack, STACKCOUNT-1 because last stack
	for (auto i = 1; i < STACKCOUNT-1; i++) {
		for (auto j = 0; j < SLICECOUNT; j++) {
			Face tempFace;
			//1 from array start 1, 1 from skip top vertex
			tempFace.myData.push_back({ 2 + j + (i-1) * SLICECOUNT,1, 2 + j + (i-1) * SLICECOUNT });
			tempFace.myData.push_back({ 2 + j + (i) * SLICECOUNT,1, 2 + j + i * SLICECOUNT });
			if (j == SLICECOUNT - 1) {
				tempFace.myData.push_back({ 2 + (i)*SLICECOUNT,1, 2 + i * SLICECOUNT });
				tempFace.myData.push_back({ 2 + (i - 1) * SLICECOUNT,1, 2 + (i - 1) * SLICECOUNT });
			}
			else {
				tempFace.myData.push_back({ 3 + j + (i)*SLICECOUNT,1, 3 + j + i * SLICECOUNT });
				tempFace.myData.push_back({ 3 + j + (i - 1) * SLICECOUNT,1, 3 + j + (i - 1) * SLICECOUNT });
			}

			tempObj.myFaces.push_back(tempFace);
		}
	}
	//puts at last row vertices
	auto offset = 1 + SLICECOUNT * (STACKCOUNT - 2);

	for (auto j = 0; j < SLICECOUNT; j++) {
		Face tempFace;
		
		tempFace.myData.push_back({ offset + j + 1,1,offset + j + 1 });
		tempFace.myData.push_back({ static_cast<int>(tempObj.myVertices.size()),1,static_cast<int>(tempObj.myNormals.size()) });
		if (j == SLICECOUNT - 1) {
			tempFace.myData.push_back({ offset + 1,1,offset + 1 });
		}
		else {
			tempFace.myData.push_back({ offset + j + 2,1,offset + j + 2 });
		}
		tempObj.myFaces.push_back(tempFace);
	}

	textureTriangle(tempObj);
	textureSquare(tempObj);

	return tempObj;
}

GenerateObj::Obj GenerateObj::createCapsule(float height, float radius, int tesselation) const {
	if (tesselation % 2 != 0)
		tesselation += 1;
	Obj tempObj;
	tempObj.type = "capsule";
	const auto PI = 3.14159265359897f;
	const auto STACKCOUNT = tesselation;
	const auto SLICECOUNT = tesselation;
	const float PHISTEP = PI / STACKCOUNT;
	const float THETASTEP = (2.0f * PI) / SLICECOUNT;

	//no texture
	tempObj.myTextures.push_back(Texture(-1, -1));

	// Vertices
	
	//top vertex
	tempObj.myVertices.push_back(XMVectorSet(0.0f, radius + height / 2.0f, 0.0f, 0));
	tempObj.myNormals.push_back(XMVectorSet(0, 1, 0, 0));

	// Top hemisphere
	for (auto i = 1; i < STACKCOUNT/2+1; i++) {
		const float phi = i * PHISTEP;
		for (auto j = 0; j < SLICECOUNT; j++) {
			const float theta = j * THETASTEP;

			tempObj.myVertices.push_back(XMVectorSet(radius * sinf(phi) * cosf(theta), radius * cosf(phi) + height / 2.0f, radius * sinf(phi) * sinf(theta), 0));
			tempObj.myNormals.push_back(XMVector3Normalize(tempObj.myVertices.back()));
		}
	}

	// Bottom hemisphere
	for (auto i = STACKCOUNT/2; i < STACKCOUNT; i++) {
		const float phi = i * PHISTEP;
		for (auto j = 0; j < SLICECOUNT; j++) {
			const float theta = j * THETASTEP;

			tempObj.myVertices.push_back(XMVectorSet(radius * sinf(phi) * cosf(theta), radius * cosf(phi) - height / 2.0f, radius * sinf(phi) * sinf(theta), 0));
			tempObj.myNormals.push_back(XMVector3Normalize(tempObj.myVertices.back()));
		}
	}

	//Bottom vertex
	tempObj.myVertices.push_back(XMVectorSet(0.0f, -radius - height / 2.0f, 0.0f, 0));
	tempObj.myNormals.push_back(XMVectorSet(0, -1, 0, 0));

	// Faces

	// Top ring
	for (auto j = 0; j < SLICECOUNT; j++) {
		Face tempFace;
		tempFace.myData.push_back({ 1,1,1 });
		tempFace.myData.push_back({ 2 + j,1,2 + j });
		if (j == SLICECOUNT - 1) {
			tempFace.myData.push_back({ 2,1,2 });
		}
		else {
			tempFace.myData.push_back({ 3 + j,1,3 + j });
		}
		tempObj.myFaces.push_back(tempFace);
	}

	// Top hemisphere
	for (auto i = 1; i < STACKCOUNT/2; i++) {
		for (auto j = 0; j < SLICECOUNT; j++) {
			Face tempFace;
			//1 from array start 1, 1 from skip top vertex
			tempFace.myData.push_back({ 2 + j + (i - 1) * SLICECOUNT,1, 2 + j + (i - 1) * SLICECOUNT });
			tempFace.myData.push_back({ 2 + j + (i)*SLICECOUNT,1, 2 + j + i * SLICECOUNT });
			if (j == SLICECOUNT - 1) {
				tempFace.myData.push_back({ 2 + (i)*SLICECOUNT,1, 2 + i * SLICECOUNT });
				tempFace.myData.push_back({ 2 + (i - 1) * SLICECOUNT,1, 2 + (i - 1) * SLICECOUNT });
			}
			else {
				tempFace.myData.push_back({ 3 + j + (i)*SLICECOUNT,1, 3 + j + i * SLICECOUNT });
				tempFace.myData.push_back({ 3 + j + (i - 1) * SLICECOUNT,1, 3 + j + (i - 1) * SLICECOUNT });
			}

			tempObj.myFaces.push_back(tempFace);
		}
	}
	// Middle bit
	{
		const auto offset = (STACKCOUNT / 2 - 1) * SLICECOUNT;
		for (auto i = 0; i < SLICECOUNT; i++) {
			Face tempFace;
			tempFace.myData.push_back({ 2 + i + offset, 1, 2 + i + offset });
			tempFace.myData.push_back({ 2 + i + offset + SLICECOUNT, 1, 2 + i + offset + SLICECOUNT });
			if (i == SLICECOUNT - 1) {
				tempFace.myData.push_back({ 2 + offset + SLICECOUNT, 1, 2 + offset + SLICECOUNT });
				tempFace.myData.push_back({ 2 + offset, 1, 2 + offset });
			}
			else {
				tempFace.myData.push_back({ 3 + i + offset + SLICECOUNT,1, 3 + i + offset + SLICECOUNT });
				tempFace.myData.push_back({ 3 + i + offset,1, 3 + i + offset});
			}
			tempObj.myFaces.push_back(tempFace);
		}
	}
	// Bottom hemisphere
	for (auto i = STACKCOUNT/2+1; i < STACKCOUNT; i++) {
		for (auto j = 0; j < SLICECOUNT; j++) {
			Face tempFace;
			//1 from array start 1, 1 from skip top vertex
			tempFace.myData.push_back({ 2 + j + (i - 1) * SLICECOUNT,1, 2 + j + (i - 1) * SLICECOUNT });
			tempFace.myData.push_back({ 2 + j + (i)*SLICECOUNT,1, 2 + j + i * SLICECOUNT });
			if (j == SLICECOUNT - 1) {
				tempFace.myData.push_back({ 2 + (i)*SLICECOUNT,1, 2 + i * SLICECOUNT });
				tempFace.myData.push_back({ 2 + (i - 1) * SLICECOUNT,1, 2 + (i - 1) * SLICECOUNT });
			}
			else {
				tempFace.myData.push_back({ 3 + j + (i)*SLICECOUNT,1, 3 + j + i * SLICECOUNT });
				tempFace.myData.push_back({ 3 + j + (i - 1) * SLICECOUNT,1, 3 + j + (i - 1) * SLICECOUNT });
			}

			tempObj.myFaces.push_back(tempFace);
		}
	}
	// Bottom ring
	{
		const auto offset = static_cast<int>(tempObj.myVertices.size()) - SLICECOUNT - 1;
		for (auto j = 0; j < SLICECOUNT; j++) {
			Face tempFace;
			tempFace.myData.push_back({ offset + j + 1,1,offset + j + 1 });
			tempFace.myData.push_back({ static_cast<int>(tempObj.myVertices.size()),1,static_cast<int>(tempObj.myNormals.size()) });
			if (j == SLICECOUNT - 1) {
				tempFace.myData.push_back({ offset + 1,1,offset + 1 });
			}
			else {
				tempFace.myData.push_back({ offset + j + 2,1,offset + j + 2 });
			}
			tempObj.myFaces.push_back(tempFace);
		}
	}

	textureTriangle(tempObj);
	textureSquare(tempObj);
	return tempObj;
}


//GenerateObj::Obj GenerateObj::createTree(float trunkHeight) const{
//	Obj tempObj;
//	tempObj.type = "tree";
//	//trunk
//	tempObj.myVertices.push_back(XMVectorSet(-0.05f*trunkHeight, trunkHeight, 0, 0));
//	tempObj.myVertices.push_back(XMVectorSet(-0.05f*trunkHeight, 0, 0, 0));
//	tempObj.myVertices.push_back(XMVectorSet(0.05f * trunkHeight, 0, 0, 0));
//	tempObj.myVertices.push_back(XMVectorSet(0.05f * trunkHeight, trunkHeight, 0, 0));
//
//	//leaves
//	tempObj.myVertices.push_back(XMVectorSet(-0.35f * trunkHeight, trunkHeight, 0, 0));
//	tempObj.myVertices.push_back(XMVectorSet(0.35f * trunkHeight, trunkHeight, 0, 0));
//	tempObj.myVertices.push_back(XMVectorSet(0, trunkHeight + 0.35f * sqrt(3.0f) * trunkHeight, 0, 0));
//
//	//texture
//	tempObj.myTextures.push_back(Texture(-1, -1));
//
//	//normal
//	tempObj.myNormals.push_back(XMVectorSet(0, 0, -1, 0));
//
//	//faces
//	Face trunk;
//	for (auto i = 1; i < 5; i++) {
//		trunk.myData.push_back({ i,1,1 });
//	}
//	Face leaves;
//	for (auto i = 1; i < 4; i++) {
//		leaves.myData.push_back({ i + 4,1,1 });
//	}
//
//	tempObj.myFaces.push_back(trunk);
//	tempObj.myFaces.push_back(leaves);
//	return tempObj;
//}
//
//GenerateObj::Obj GenerateObj::createCloud(float height) const{
//	Obj tempObj;
//	tempObj.type = "cloud";
//	const float radius = height / 8.0f;
//	const float PI = 3.14159265359897f;
//
//	for (auto i = 0; i < 12; i++) {
//		const XMFLOAT3 center = XMFLOAT3(3.0f * radius * cosf(PI * (i / 6.0f)), 3 * radius * sinf(PI * (i / 6.0f)), 0);
//		for (auto j = 0; j < 50; j++) {
//			tempObj.myVertices.push_back(XMVectorSet(center.x + radius * cosf(PI * (j / 25.0f)), center.y + radius * sinf(PI * (j / 25.0f)), 0, 0));
//		}
//	}
//	for (auto j = 0; j < 50; j++) {
//		tempObj.myVertices.push_back(XMVectorSet(3.0f*radius * cosf(PI * (j / 25.0f)), 3.0f*radius * sinf(PI * (j / 25.0f)), 0, 0));
//	}
//
//	tempObj.myTextures.push_back(Texture(-1, -1));
//	tempObj.myNormals.push_back(XMVectorSet(0, 0, -1, 0));
//
//	for (auto i = 0; i < 12; i++) {
//		Face tempFace;
//		for (auto j = 0; j < 50; j++) {
//			tempFace.myData.push_back({ i * 50 + j+1, 1, 1 });
//		}
//		tempObj.myFaces.push_back(tempFace);
//	}
//	Face tempFace;
//	for (auto j = 0; j < 50; j++) {
//		tempFace.myData.push_back({ j + 1 + 12 * 50,1,1 });
//	}
//	tempObj.myFaces.push_back(tempFace);
//
//	return tempObj;
//}

GenerateObj::Obj GenerateObj::createPlane(float scale) const {
	Obj tempObj;
	tempObj.type = "plane";
	for (auto i = 0; i < 100; i++) {
		for (auto j = 0; j < 100; j++) {
			tempObj.myVertices.push_back(XMVectorSet(scale * j - scale*50, 0, scale * i - scale * 50, 0));
		}
	}
	tempObj.myNormals.push_back(XMVectorSet(0, 1, 0, 0));

	for (auto i = 0; i < 99; i++) {
		for (auto j = 0; j < 99; j++) {
			Face tempFace;
			tempFace.myData.push_back({ j + 100 * (i + 1) + 1,1,1 });
			tempFace.myData.push_back({ j + 100 * (i + 1) + 2,1,1 });
			tempFace.myData.push_back({ j + 100 * i + 2,1,1 });
			tempFace.myData.push_back({ j + 100 * i+1, 1, 1 });
			tempObj.myFaces.push_back(tempFace);
		}
	}

	textureSquare(tempObj);

	return tempObj;
}

void GenerateObj::translateLast(float x, float y, float z) {
	XMVECTOR temp = XMVectorSet(x, y, z, 0);
	for (auto& vertex : _objectsToGenerate.back().myVertices) {
		vertex += temp;
	}
}

