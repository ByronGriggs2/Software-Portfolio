#pragma once

#include <vector>

// This is a 2D boolean vector optimized for CPU efficiency
class CollisionExemptionRecord final {
public:
	std::vector<bool> _record = {};
	int _totalObjectCount = 0;

	// For bulk allocation
	inline void resizeVector(int newNumberOfObjects) {
		_totalObjectCount = newNumberOfObjects;
		const int totalUniquePairs = (_totalObjectCount * (_totalObjectCount - 1)) / 2;
		_record.resize(static_cast<size_t>(totalUniquePairs), false);
	}
	//cannot return by reference as the vector doesn't actually contain booleans. Will have to make it a vector of bits if I want that efficiency.
	inline const bool get(int i, int j) const {
		const int objIndex = i * _totalObjectCount - (i * (i + 1)) / 2 + (j - i - 1);
		return _record[objIndex];
	}
	inline void set(int i, int j) {
		const int objIndex = i * _totalObjectCount - (i * (i + 1)) / 2 + (j - i - 1);
		_record[objIndex] = true;
	}
	inline void clear(int i, int j) {
		const int objIndex = i * _totalObjectCount - (i * (i + 1)) / 2 + (j - i - 1);
		_record[objIndex] = false;
	}
	inline void resetVector() {
		_totalObjectCount = 0;
		_record.clear();
	}
};