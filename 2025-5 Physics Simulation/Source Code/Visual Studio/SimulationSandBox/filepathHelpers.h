#pragma once

#include <filesystem>
#include <iostream>
#include <Windows.h>

namespace helpers {
	inline std::filesystem::path getRuntimeFilepath() {
		char exePath[MAX_PATH];
		GetModuleFileNameA(NULL, exePath, MAX_PATH);
		// Application
		std::filesystem::path myPath(exePath);
		// Configuration
		myPath.remove_filename();
		// Random slashes idk
		myPath = myPath.parent_path();
		// Platform
		myPath = myPath.parent_path();
		// Solution directory
		myPath = myPath.parent_path();
		// Project
		return (myPath / "SimulationSandBox");
	}
}