#pragma once
#include <filesystem>

namespace helpers {
	std::filesystem::path getWritePath(const std::string& foldername, const std::string& filename, const std::string& fileExtension = ".txt");
	float getTime(LARGE_INTEGER start, LARGE_INTEGER freq);
	void printSystemTime();
	bool fileExists(const std::filesystem::path& directory, const std::string& filename, const std::string& extension, int number);
	std::string intToStr(int);
}