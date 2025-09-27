#include "ReadObj.h"

std::unique_ptr<ReadObj> ReadObj::_instance = std::make_unique<ReadObj>();


using helpers::SimpleVertex;

void ReadObj::read(const std::string& filename) {
	//clean up from last call
	ifs.close();
	objVector.clear();

	ifs.open(filename);
	std::string line;
	std::string word;

	std::unique_ptr<Obj> currentObj;

	while (std::getline(ifs,line)) {
		std::istringstream iss(line);
		iss >> word;
		
		if (word[0] == '#') {
			continue;
		}
		else if (word == "o") {
			if (currentObj != nullptr) {
				iss >> word;
				objVector.push_back(*currentObj);
				currentObj.reset();
				currentObj = std::make_unique<Obj>();
				currentObj->type = word;
			}
			else {
				currentObj = std::make_unique<Obj>();
				iss >> word;
				currentObj->type = word;
			}
		}
		else if (word == "v") {
			XMFLOAT3 tempVertex;
			iss >> tempVertex.x;
			iss >> tempVertex.y;
			iss >> tempVertex.z;
			currentObj->vertexPos.push_back(tempVertex);
			continue;
		}
		else if (word == "vt") {
			XMFLOAT2 tempVertex;
			iss >> tempVertex.x;
			iss >> tempVertex.y;
			currentObj->vertexTex.push_back(tempVertex);
			continue;
		}
		else if (word == "vn") {
			XMFLOAT3 tempVertex;
			iss >> tempVertex.x;
			iss >> tempVertex.y;
			iss >> tempVertex.z;
			currentObj->vertexNorm.push_back(tempVertex);
		}
		else if (word == "f") {
			Face tempFace;
			Face::faceData tempData;
			char throwaway;
			while (iss >> word) {
				std::istringstream vertexStream(word);
				//vertex /
				vertexStream >> tempData.vertex >> throwaway;
				//if vertex/texture
				if (vertexStream.peek() != '/') {
					vertexStream >> tempData.texture >> throwaway;
				}
				//if vertex//
				else {
					vertexStream.ignore();
				}
				vertexStream >> tempData.normal;
				tempFace.myData.push_back(tempData);
			}
			currentObj->faceVector.push_back(tempFace);
		}
		else {
			continue;
		}
	}
	objVector.push_back(*currentObj);
}

void ReadObj::convert(std::vector<SimpleVertex>& vertexVector, std::vector<WORD>& indexVector)const {
	vertexVector.clear();
	indexVector.clear();
	int objOffset = 0;
	for (auto& currentObj : objVector) {
		int faceOffset = 0;
		for (auto& face : currentObj.faceVector) {
			for (auto& currentVertex : face.myData) {
				SimpleVertex tempVertex;
				tempVertex.Pos = XMVectorSet(currentObj.vertexPos[currentVertex.vertex - 1].x,
										  currentObj.vertexPos[currentVertex.vertex - 1].y,
										  currentObj.vertexPos[currentVertex.vertex - 1].z, 0);

				tempVertex.Tex = XMVectorSet(currentObj.vertexTex[currentVertex.texture - 1].x,
					currentObj.vertexTex[currentVertex.texture - 1].y, 0, 0);

				tempVertex.Norm = XMVectorSet(currentObj.vertexNorm[currentVertex.normal - 1].x,
										   currentObj.vertexNorm[currentVertex.normal - 1].y,
									       currentObj.vertexNorm[currentVertex.normal - 1].z, 0);
				//tempVertex.Tangent = XMFLOAT3(-1, -1, -1);
				//tempVertex.Binormal = XMFLOAT3(-1, -1, -1);
				vertexVector.push_back(tempVertex);
			}
			//indices
			//does not work for all polygons, but works for all regular polygons
			for (auto i = 0; i < face.myData.size() - 2; i++) {
				indexVector.push_back(static_cast<WORD>(faceOffset + objOffset));
				indexVector.push_back(static_cast<WORD>(faceOffset + i + 1 + objOffset));
				indexVector.push_back(static_cast<WORD>(faceOffset + i + 2 + objOffset));

			}
			faceOffset += static_cast<int>(face.myData.size());
		}
		for (auto& face : currentObj.faceVector) {
			objOffset += static_cast<int>(face.myData.size());
		}
	}
}