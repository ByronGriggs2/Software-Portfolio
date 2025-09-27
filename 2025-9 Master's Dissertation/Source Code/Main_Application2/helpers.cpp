#include "pch.h"
#include "helpers.h"

using std::endl;

std::filesystem::path helpers::getWritePath(const std::string& foldername, const std::string& filename, const std::string& fileExtension) {
	std::ofstream ofs;
	std::filesystem::path currentPath = std::filesystem::current_path();
	currentPath = currentPath / foldername;
	if (!std::filesystem::exists(currentPath))
		std::filesystem::create_directory(currentPath);

	int index = 0;
	while (true) {
		if (!fileExists(currentPath, filename, fileExtension, index))
			break;
		index++;
	}
	std::string combined = filename + intToStr(index) + fileExtension;
	std::filesystem::path temp = currentPath / combined.c_str();
	return temp;
}

bool helpers::fileExists(const std::filesystem::path& directory, const std::string& filename, const std::string& extension, int num) {
	std::string combined = filename + intToStr(num) + extension;
	std::filesystem::path temp = directory / combined.c_str();
	return std::filesystem::exists(temp);
}

std::string helpers::intToStr(int in) {
	if (in == 0)
		return std::string("0");
	std::string str;
	while (in != 0) {
		str.insert(str.begin(), static_cast<char>('0' + in % 10));
		in -= in % 10;
		in /= 10;
	}
	return str;
}

float helpers::getTime(LARGE_INTEGER start, LARGE_INTEGER freq) {
	LARGE_INTEGER end;
	QueryPerformanceCounter(&end);
	return (static_cast<float>((end.QuadPart - start.QuadPart) / static_cast<double>(freq.QuadPart)));
}

void helpers::printSystemTime() {
	time_t now = time(nullptr);
	tm localTime;
	localtime_s(&localTime, &now);
	std::cout << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S") << " ";
}