#include "pch.h"
#include "ReplayBuffer.h"

using std::cout, std::endl;

void ReplayBuffer::addTransition(const Transition& t) {
	std::lock_guard<std::recursive_mutex> lock(_mx);
	if (_buf.size() == REPLAY_BUFFER_SIZE)
		_buf.erase(_buf.begin());
	_buf.push_back(t);
	//cout << "Transition with reward of " << t._R1 << " added." << endl;
}

//ReplayBuffer::Transition ReplayBuffer::operator[](int i) const{
//	return _buf[i];
//}

std::vector<Transition> ReplayBuffer::getSample() {
	std::lock_guard<std::recursive_mutex> lock(_mx);
	const int bufSize = static_cast<int>(_buf.size());
	const int sampleSize = bufSize < BATCH_SIZE ? bufSize : BATCH_SIZE;
	std::vector<Transition> sample;
	for (int i = 0; i < sampleSize; i++) {
		sample.push_back(_buf[rand() % bufSize]);
	}
//#ifdef FIRST_LEVEL_ONLY
//	float cumulativeReward = 0;
//	for (const auto& trans : sample) {
//		cumulativeReward += trans._R1;
//	}
//	cumulativeReward /= sample.size();
//	//cout << "Sample taken with average reward of " << cumulativeReward << endl;
//#endif
	return sample;
}

void ReplayBuffer::purgeLastSecond() {
	std::lock_guard<std::recursive_mutex> lock(_mx);
	constexpr int ms = 1000;
	constexpr int elems = ms / ONEFRAMEMS + 1;
	if (_buf.size() < elems) {
		_buf.clear();
		return;
	}
	_buf.erase(_buf.end() - (elems - 1), _buf.end());
}

void ReplayBuffer::saveToDisk(const std::string& filename) {
	std::lock_guard<std::recursive_mutex> lock(_mx);
	std::filesystem::path currentPath = std::filesystem::current_path();
	currentPath = currentPath / "ReplayBufferStorage";
	if (!std::filesystem::exists(currentPath))
		std::filesystem::create_directory(currentPath);
	currentPath = currentPath / filename;
	std::ofstream ofs(currentPath, std::ios::binary);
	size_t size = _buf.size();
	ofs.write(reinterpret_cast<const char*>(&size), sizeof(size));
	ofs.write(reinterpret_cast<const char*>(_buf.data()), size * sizeof(Transition));
}

void ReplayBuffer::loadFromDisk(const std::string& filename) {
	std::lock_guard<std::recursive_mutex> lock(_mx);
	std::filesystem::path currentPath = std::filesystem::current_path();
	currentPath = currentPath / "ReplayBufferStorage";
	currentPath = currentPath / filename;
	std::ifstream ifs(currentPath, std::ios::binary);
	if (!ifs.is_open()) {
		throw std::runtime_error("Failed to open file: " + filename);
	}
	size_t size;
	ifs.read(reinterpret_cast<char*>(&size), sizeof(size));
	_buf.resize(size);
	ifs.read(reinterpret_cast<char*>(_buf.data()), size * sizeof(Transition));
}