#include "ConfigReader.h"
#include <fstream>
#include <string>
#include <sstream>
#include <filesystem>
#include <cassert>
#include "ReadObj.h"
#include "StringLiterals.h"

std::unique_ptr<ConfigReader> ConfigReader::_instance = std::make_unique<ConfigReader>();

PlayerWrap ConfigReader::read(std::vector<CompleteObject>& objectsToDraw) {
	std::ifstream ifs("config.txt");
	std::string line;
	
	std::string folder;
	std::string word;
	std::string file;
	std::string name;
	while (std::getline(ifs, line)) {
		if (line[0] == '/' && line[1] == '/')
			continue;
		std::istringstream iss(line);
		iss >> folder;
		iss >> file;
		iss >> name;

		//special objects
		if (name == PLAYER) {
			generatePlayer(folder);
			continue;
		}
		if (name == ARROW) {
			generateArrow(folder);
			continue;
		}
		iss.ignore(50, ':');
		if (iss.eof()){
			generateSimple(folder, file, name, XMFLOAT3(0, 0, 0), objectsToDraw);
			continue;
		}
		//modified objects
		Rotation rotType = Rotation::null;
		bool collectable = false;
		float x = 0, y = 0, z = 0;
		int count = 1;
		float spacing = 0;
		while (iss >> word) {
			//array
			if (word[0] == 'x') {
				std::istringstream isTemp(word);
				char throwAway;
				//skip x
				isTemp >> throwAway;
				isTemp >> count;
				//skip _
				isTemp >> throwAway;
				isTemp >> spacing;
			}
			//offset
			else if (word[0] == 'O') {
				std::istringstream isTemp(word);
				char throwAway;
				//skip O
				isTemp >> throwAway;
				isTemp >> x;
				isTemp >> throwAway;
				isTemp >> y;
				isTemp >> throwAway;
				isTemp >> z;
			}
			else if (word[0] == 'C') {
				collectable = true;
			}
			else if (word[0] == 'R') {
				if (word == "Rcw") {
					rotType = Rotation::cw;
				}
				else if (word == "Rccw"){
					rotType = Rotation::ccw;
				}
				else {
					rotType = Rotation::null;
				}
			}
			iss.ignore(50, ':');
		}
		generateArray(folder, file, name, count, spacing, XMFLOAT3(x, y, z), objectsToDraw, collectable, rotType);
	}
	return PlayerWrap(std::move(tempPlayer), std::move(tempArrow));
}

void ConfigReader::generatePlayer(const std::string& folder) {
	const std::filesystem::path tempDir = std::filesystem::current_path() / ASSETS / folder;
	const std::string filename = (tempDir / PLAYEROBJ).string();
	auto& fileReader = ReadObj::getInstance();

	tempPlayer = std::make_unique<CompleteObject>(XMFLOAT3(0, 24, 0), PLAYER, false);

	fileReader.read(filename);
	fileReader.convert(*tempPlayer);
	tempPlayer->posInit();
}

void ConfigReader::generateArrow(const std::string& folder) {
	const std::filesystem::path tempDir = std::filesystem::current_path() / ASSETS / folder;
	const std::string filename = (tempDir / ARROWOBJ).string();
	auto& fileReader = ReadObj::getInstance();

	tempArrow = std::make_unique<CompleteObject>(XMFLOAT3(0, 34, 0), ARROW, false);

	fileReader.read(filename);
	fileReader.convert(*tempArrow);
	tempArrow->posInit();
}

void ConfigReader::generateSimple(const std::string& folder, const std::string& file, const std::string& name, const XMFLOAT3& offset, std::vector<CompleteObject>& objectsToDraw) const{
	const std::filesystem::path tempDir = std::filesystem::current_path() / ASSETS / folder;
	const std::string filename = (tempDir / (file + ".obj")).string();
	auto& fileReader = ReadObj::getInstance();

	CompleteObject tempObject(offset, name, false);

	fileReader.read(filename);
	fileReader.convert(tempObject);
	tempObject.posInit();

	setTanBinorm(tempObject);

	objectsToDraw.push_back(tempObject);
}

void ConfigReader::generateArray(const std::string& folder, const std::string& file, const std::string& name, int count, float spacing, const XMFLOAT3& offset, std::vector<CompleteObject>& objectsToDraw, bool collectable, Rotation rot) const{
	const std::filesystem::path tempDir = std::filesystem::current_path() / ASSETS / folder;
	const std::string filename = (tempDir / (file + ".obj")).string();
	auto& fileReader = ReadObj::getInstance();

	const float initialPos = -(spacing * count / 2);

	for (auto i = 0; i < count; i++) {
		CompleteObject tempObject(XMFLOAT3(initialPos+i*spacing+offset.x, offset.y, offset.z), name, collectable);

		fileReader.read(filename);
		fileReader.convert(tempObject);
		tempObject.posInit();

		setTanBinorm(tempObject);

		tempObject.setRotation(rot);

		objectsToDraw.push_back(tempObject);
	}
}

void ConfigReader::setTanBinorm(CompleteObject& tempObject) const {
	if (tempObject.getType() == "bumpCube") {
		for (auto i = 0; i < 4; i++) {
			tempObject.setTanBinorm(i, XMFLOAT3(1, 0, 0), XMFLOAT3(0, 0, -1));
		}
		for (auto i = 4; i < 8; i++) {
			tempObject.setTanBinorm(i, XMFLOAT3(1, 0, 0), XMFLOAT3(0, 1, 0));
		}
		for (auto i = 8; i < 12; i++) {
			tempObject.setTanBinorm(i, XMFLOAT3(0, 0, 1), XMFLOAT3(0, 1, 0));
		}
		for (auto i = 12; i < 16; i++) {
			tempObject.setTanBinorm(i, XMFLOAT3(-1, 0, 0), XMFLOAT3(0, 1, 0));
		}
		for (auto i = 16; i < 20; i++) {
			tempObject.setTanBinorm(i, XMFLOAT3(0, 0, -1), XMFLOAT3(0, 1, 0));
		}
		for (auto i = 20; i < 24; i++) {
			tempObject.setTanBinorm(i, XMFLOAT3(1, 0, 0), XMFLOAT3(0, 0, 1));
		}
	}
}