#pragma once
#include "StatePacket.h"
#include "definitions.h"
#include <vector>
#include <mutex>

class ReplayBuffer {
private:
	std::vector<Transition> _buf;
	std::recursive_mutex _mx;
public:
	void addTransition(const Transition& t);
	//Transition operator[](int i) const;
	std::vector<Transition> getSample();
	void purgeLastSecond();
	void saveToDisk(const std::string& filename);
	void loadFromDisk(const std::string& filename);

	bool operator==(const ReplayBuffer& other) const {
		return _buf == other._buf;
	}
};