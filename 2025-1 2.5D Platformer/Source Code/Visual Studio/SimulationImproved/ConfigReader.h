//Parses config file and uses it to call ReadObj::generate, which reads in from the obj files

#pragma once

#include <memory>
#include <iostream>
#include "CompleteObject.h"
#include "PlayerWrap.h"

class ConfigReader final{
	static std::unique_ptr<ConfigReader> _instance;

	std::unique_ptr<CompleteObject> tempPlayer;
	std::unique_ptr<CompleteObject> tempArrow;

	void generatePlayer(const std::string& folder);
	void generateArrow(const std::string& folder);
	void generateSimple(const std::string& folder, const std::string& file, const std::string& name, const XMFLOAT3& offset, std::vector<CompleteObject>& objectsToDraw) const;
	void generateArray(const std::string& folder, const std::string& file, const std::string& name, int, float, const XMFLOAT3&, std::vector<CompleteObject>&, bool, Rotation) const;
	void setTanBinorm(CompleteObject&) const;

public:

	static ConfigReader& getInstance() { return *_instance; }

	PlayerWrap read(std::vector<CompleteObject>& objectsToDraw);
};