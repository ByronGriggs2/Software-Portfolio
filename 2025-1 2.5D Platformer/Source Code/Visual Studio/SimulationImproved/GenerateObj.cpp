#include "GenerateObj.h"
#include "StringLiterals.h"

std::unique_ptr<GenerateObj> GenerateObj::_instance = std::make_unique<GenerateObj>();

void GenerateObj::generate() {
	//folders
	std::filesystem::path myPath = std::filesystem::current_path();
	std::filesystem::create_directories(myPath / ASSETS);
	myPath = myPath / ASSETS;
	std::filesystem::create_directories(myPath / BACKGROUND);
	std::filesystem::create_directories(myPath / FOREGROUND);
	std::filesystem::create_directories(myPath / MISC);
	std::filesystem::create_directories(myPath / PLATFORMS);
	std::filesystem::create_directories(myPath / PLAYER);
	//platforms
	std::vector<Obj> platforms;
	platforms.push_back(createRectangularPrism(500.0f, 5.0f, 10.0f));
	platforms.push_back(createRectangularPrism(80.0f, 5.0f, 10.0f));
	platforms.push_back(createRectangularPrism(50.0f, 5.0f, 10.0f));
	platforms.push_back(createRectangularPrism(100.0f, 5.0f, 10.0f));
	platforms.push_back(createRectangularPrism(10.0f, 5.0f, 10.0f));
	platforms.push_back(createRectangularPrism(100.0f, 5.0f, 10.0f));
	platforms.push_back(createRectangularPrism(90.0f, 5.0f, 10.0f));
	platforms.push_back(createRectangularPrism(100.0f, 5.0f, 10.0f));
	platforms.push_back(createRectangularPrism(40.0f, 5.0f, 10.0f));
	platforms.push_back(createRectangularPrism(10.0f, 5.0f, 10.0f));
	for (auto i = 0; i < platforms.size(); i++) {
		const std::string temp = PLATFORM + std::to_string(i + 1);
		generateSingle(platforms[i], PLATFORMS, temp);
	}
	//hud arrow
	{
		Obj temp = createArrow(2.0f, 6.0f, 1.0f);
		generateSingle(temp, MISC, ARROW);
	}
	//phong, gourad and high-poly spheres
	{
		Obj temp1 = createSphere(15, 15);
		generateSingle(temp1, MISC, SPHERE1);
		Obj temp2 = createSphere(15, 100);
		generateSingle(temp2, MISC, SPHERE2);
	}
	//bump cube
	{
		Obj temp = createRectangularPrism(15, 15, 15);
		generateSingle(temp, MISC, CUBE1);
	}
	//ground in background
	{
		Obj temp = createRectangularPrism(5000.0f, 2.0f, 99.0f);
		generateSingle(temp, BACKGROUND, GROUND1);
	}
	//tree
	{
		Obj temp = createTree(60);
		generateSingle(temp, BACKGROUND, TREE1);
	}
	//cloud
	{
		Obj temp = createCloud(400);
		generateSingle(temp, BACKGROUND, CLOUD1);
	}
	//fire
	{
		Obj temp = createFire(15);
		generateSingle(temp, MISC, FIRE1);
	}
	//sky
	{
		Obj temp = createRectangularPrism(100000.0f, 100000.0f, 100000.0f);
		generateSingle(temp, BACKGROUND, SKY1);
	}
	//foreground tree
	//identical to other tree, but in a different folder (with a different filename)
	{
		Obj temp = createTree(60);
		generateSingle(temp, FOREGROUND, TREEF1);
	}
	//Ground in foreground
	{
		Obj temp = createRectangularPrism(5000.0f, 0.0f, 1000.0f);
		generateSingle(temp, FOREGROUND, HORIZONTAL1);
	}
	//cliff wall
	{
		Obj temp = createRectangularPrism(230.0f, 230.0f, 0.0f);
		generateSingle(temp, FOREGROUND, WALLFACE1);
	}

	//generate basic objects
	for (auto& object : objectsToGenerate) {
		std::ofstream ofs(object.myDir);
		printValues(ofs, object);
	}
	objectsToGenerate.clear();

	//player
	createAndOutputPlayer();

	std::filesystem::path tempDir = std::filesystem::current_path() / ASSETS / MISC;
	std::string filename = (tempDir / "complete.txt").string();
	std::ofstream ofs(filename);
	ofs << "all .obj files have been generated.";
}

void GenerateObj::generateSingle(Obj& obj, const std::string& folder, const std::string& name) {
	const std::filesystem::path tempDir = std::filesystem::current_path() / "Assets" / folder;
	obj.myName = name;
	obj.myDir = (tempDir / (name + ".obj")).string();
	objectsToGenerate.push_back(obj);
}

void GenerateObj::printValues(std::ofstream& ofs, const GenerateObj::Obj& out) const {
	ofs << "o " << out.myName << std::endl;
	ofs << std::setprecision(6) << std::fixed;
	for (auto& i : out.myVertices) {
		ofs << "v ";
		i.print(ofs);
		ofs << std::endl;
	}
	for (auto& i : out.myTextures) {
		ofs << "vt " << i.u << " " << i.v << std::endl;
	}
	for (auto& i : out.myNormals) {
		ofs << "vn ";
		i.print(ofs);
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
	//delete top face
	tempObj.myFaces.erase(tempObj.myFaces.begin());

	Vector3f vertices[6];
	vertices[0] = Vector3f(-width, height / 2, -depth / 2);
	vertices[1] = Vector3f(width, height / 2, -depth / 2);
	vertices[2] = Vector3f(width, height / 2, depth / 2);
	vertices[3] = Vector3f(-width, height / 2, depth / 2);
	vertices[4] = Vector3f(0.0f, (height / 2) + width, -depth / 2);
	vertices[5] = Vector3f(0.0f, (height / 2) + width, depth / 2);
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
	Vector3f diagonals[2];
	diagonals[0] = Vector3f(-sqrt(2.0f), sqrt(2.0f), 0.0f);
	diagonals[1] = Vector3f(sqrt(2.0f), sqrt(2.0f), 0.0f);
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
	Vector3f vertices[8];
	vertices[0] = Vector3f(-width/2, height/2, -depth/2);
	vertices[1] = Vector3f(width/2, height/2, -depth/2);
	vertices[2] = Vector3f(width/2, height/2, depth/2);
	vertices[3] = Vector3f(-width/2, height/2, depth/2);
	vertices[4] = Vector3f(-width/2, -height/2, -depth/2);
	vertices[5] = Vector3f(width/2, -height/2, -depth/2);
	vertices[6] = Vector3f(width/2, -height/2, depth/2);
	vertices[7] = Vector3f(-width/2, -height/2, depth/2);
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

	Vector3f normals[6];
	normals[0] = Vector3f(0, 1, 0);
	normals[1] = Vector3f(0, 0, -1);
	normals[2] = Vector3f(1, 0, 0);
	normals[3] = Vector3f(0, 0, 1);
	normals[4] = Vector3f(0, -1, 0);
	normals[5] = Vector3f(0, -1, 0);
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

//Player model is the same as a previous lab
GenerateObj::Obj GenerateObj::createAndOutputPlayer() {
	Obj upperHead = createRectangularPrism(2, 2, 2);
	upperHead.myName = "upperHead";
	objectsToGenerate.push_back(upperHead);

	Obj lowerHead = createRectangularPrism(2, 2, 4);
	translate(lowerHead, 0, -2, 0);
	lowerHead.myName = "lowerHead";
	objectsToGenerate.push_back(lowerHead);

	Obj torso = createRectangularPrism(2, 10, 2);
	translate(torso, 0, -8, 0);
	torso.myName = "torso";
	objectsToGenerate.push_back(torso);

	Obj pelvis = createRectangularPrism(2, 2, 4);
	translate(pelvis, 0, -14, 0);
	pelvis.myName = "pelvis";
	objectsToGenerate.push_back(pelvis);

	Obj rightLeg = createRectangularPrism(2, 10, 2);
	translate(rightLeg, 0, -18, 3);
	rightLeg.myName = "rightLeg";
	objectsToGenerate.push_back(rightLeg);

	Obj leftLeg = createRectangularPrism(2, 10, 2);
	translate(leftLeg, 0, -18, -3);
	leftLeg.myName = "leftLeg";
	objectsToGenerate.push_back(leftLeg);

	Obj leftArm = createRectangularPrism(2, 6, 2);
	translate(leftArm, 0, -8, -2);
	leftArm.myName = "leftArm";
	objectsToGenerate.push_back(leftArm);

	Obj rightArm = createRectangularPrism(2, 6, 2);
	translate(rightArm, 0, -8, 2);
	rightArm.myName = "rightArm";
	objectsToGenerate.push_back(rightArm);

	std::filesystem::path tempDir = std::filesystem::current_path() / "Assets" / "Player";
	std::string filename = (tempDir / ("player.obj")).string();
	std::ofstream ofs(filename);
	for (const auto& i : objectsToGenerate) {
		printValues(ofs, i);
	}

	return upperHead;
}

GenerateObj::Obj GenerateObj::createSphere(float radius, int tesselation) const{
	Obj tempObj;
	const auto PI = 3.14159265359897f;
	const auto STACKCOUNT = tesselation;
	const auto SLICECOUNT = tesselation;
	const float PHISTEP = PI / STACKCOUNT;
	const float THETASTEP = (2.0f * PI) / SLICECOUNT;

	//top vertex
	tempObj.myVertices.push_back(Vector3f(0.0f, radius, 0.0f));
	tempObj.myNormals.push_back(Vector3f(0, 1, 0));

	//no texture
	tempObj.myTextures.push_back(Texture(-1,-1));

	for (auto i = 1; i < STACKCOUNT; i++) {
		const float phi = i * PHISTEP;
		for (auto j = 0; j < SLICECOUNT; j++) {
			const float theta = j * THETASTEP;

			tempObj.myVertices.push_back(Vector3f(radius * sinf(phi) * cosf(theta), radius * cosf(phi), radius * sinf(phi)*sinf(theta)));
			XMVECTOR tempNormal = tempObj.myVertices.back().directX();
			tempNormal = XMVector3Normalize(tempNormal);
			tempObj.myNormals.push_back(Vector3f(tempNormal));
		}
	}

	//bottom vertex
	tempObj.myVertices.push_back(Vector3f(0.0f, -radius, 0.0f));
	tempObj.myNormals.push_back(Vector3f(0, -1, 0));

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
				tempFace.myData.push_back({ 2 + (i)*SLICECOUNT,1, 2 + j + i * SLICECOUNT });
				tempFace.myData.push_back({ 2 + (i - 1) * SLICECOUNT,1, 2 + j + (i - 1) * SLICECOUNT });
			}
			else {
				tempFace.myData.push_back({ 3 + j + (i)*SLICECOUNT,1, 2 + j + i * SLICECOUNT });
				tempFace.myData.push_back({ 3 + j + (i - 1) * SLICECOUNT,1, 2 + j + (i - 1) * SLICECOUNT });
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
	return tempObj;
}

GenerateObj::Obj GenerateObj::createTree(float trunkHeight) const{
	Obj tempObj;
	//trunk
	tempObj.myVertices.push_back(Vector3f(-0.05f*trunkHeight, trunkHeight, 0));
	tempObj.myVertices.push_back(Vector3f(-0.05f*trunkHeight, 0, 0));
	tempObj.myVertices.push_back(Vector3f(0.05f * trunkHeight, 0, 0));
	tempObj.myVertices.push_back(Vector3f(0.05f * trunkHeight, trunkHeight, 0));

	//leaves
	tempObj.myVertices.push_back(Vector3f(-0.35f * trunkHeight, trunkHeight, 0));
	tempObj.myVertices.push_back(Vector3f(0.35f * trunkHeight, trunkHeight, 0));
	tempObj.myVertices.push_back(Vector3f(0, trunkHeight + 0.35f * sqrt(3.0f) * trunkHeight, 0));

	//texture
	tempObj.myTextures.push_back(Texture(-1, -1));

	//normal
	tempObj.myNormals.push_back(Vector3f(0, 0, -1));

	//faces
	Face trunk;
	for (auto i = 1; i < 5; i++) {
		trunk.myData.push_back({ i,1,1 });
	}
	Face leaves;
	for (auto i = 1; i < 4; i++) {
		leaves.myData.push_back({ i + 4,1,1 });
	}

	tempObj.myFaces.push_back(trunk);
	tempObj.myFaces.push_back(leaves);
	return tempObj;
}

GenerateObj::Obj GenerateObj::createCloud(float height) const{
	Obj tempObj;
	const float radius = height / 8.0f;
	const float PI = 3.14159265359897f;

	for (auto i = 0; i < 12; i++) {
		const XMFLOAT3 center = XMFLOAT3(3.0f * radius * cosf(PI * (i / 6.0f)), 3 * radius * sinf(PI * (i / 6.0f)), 0);
		for (auto j = 0; j < 50; j++) {
			tempObj.myVertices.push_back(Vector3f(center.x + radius * cosf(PI * (j / 25.0f)), center.y + radius * sinf(PI * (j / 25.0f)), 0));
		}
	}
	for (auto j = 0; j < 50; j++) {
		tempObj.myVertices.push_back(Vector3f(3.0f*radius * cosf(PI * (j / 25.0f)), 3.0f*radius * sinf(PI * (j / 25.0f)), 0));
	}

	tempObj.myTextures.push_back(Texture(-1, -1));
	tempObj.myNormals.push_back(Vector3f(0, 0, -1));

	for (auto i = 0; i < 12; i++) {
		Face tempFace;
		for (auto j = 0; j < 50; j++) {
			tempFace.myData.push_back({ i * 50 + j+1, 1, 1 });
		}
		tempObj.myFaces.push_back(tempFace);
	}
	Face tempFace;
	for (auto j = 0; j < 50; j++) {
		tempFace.myData.push_back({ j + 1 + 12 * 50,1,1 });
	}
	tempObj.myFaces.push_back(tempFace);

	return tempObj;
}

GenerateObj::Obj GenerateObj::createFire(float scale) const{
	const int PARTICLECOUNT = 100;

	Obj tempObj;
	for (auto i = 0; i < PARTICLECOUNT; i++) {
		tempObj.myVertices.push_back(Vector3f(-scale, -scale, (99-i)*scale));
		tempObj.myVertices.push_back(Vector3f(scale, -scale, (99-i)*scale));
		tempObj.myVertices.push_back(Vector3f(scale, scale, (99-i)*scale));
		tempObj.myVertices.push_back(Vector3f(-scale, scale, (99-i)*scale));
	}

	tempObj.myTextures.push_back(Texture( - 1, -1));
	
	tempObj.myNormals.push_back(Vector3f(0, 0, -1));

	for (auto i = 0; i < PARTICLECOUNT; i++) {
		Face tempFace;
		tempFace.myData.push_back({ 1 + 4 * i, 1, 1 });
		tempFace.myData.push_back({ 2 + 4 * i, 1, 1 });
		tempFace.myData.push_back({ 3 + 4 * i,1,1 });
		tempFace.myData.push_back({ 4 + 4 * i,1,1 });
		tempObj.myFaces.push_back(tempFace);
	}
	return tempObj;
}

void GenerateObj::translate(Obj& obj, float a, float b, float c) const {
	for (auto& vertex : obj.myVertices) {
		vertex.add(a, b, c);
	}
}
