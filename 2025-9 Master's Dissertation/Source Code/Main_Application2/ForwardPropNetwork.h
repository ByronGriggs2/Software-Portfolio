#pragma once
#include <vector>
#include "definitions.h"

struct ForwardPropNetwork {
	std::vector<std::vector<std::vector<double>>> _weights;
	std::vector<std::vector<double>> _preactivation;
	std::vector<std::vector<double>> _alpha;

	bool operator==(const ForwardPropNetwork& other) const {
		if (_weights.size() != other._weights.size())
			return false;
		for (auto i = 0; i < _weights.size(); i++) {
			if (_weights[i].size() != other._weights[i].size())
				return false;
			for (auto j = 0; j < _weights[i].size(); j++) {
				if (_weights[i][j].size() != other._weights[i][j].size())
					return false;
				for (auto k = 0; k < _weights[i][j].size(); k++) {
					if (abs(_weights[i][j][k] - other._weights[i][j][k]) > EPSILON)
						return false;
				}
			}
		}
		for (auto i = 0; i < _preactivation.size(); i++) {
			if (_preactivation[i].size() != other._preactivation[i].size())
				return false;
			for (auto j = 0; j < _preactivation[i].size(); j++) {
				if (abs(_preactivation[i][j] - other._preactivation[i][j]) > EPSILON)
					return false;
			}
		}
		for (auto i = 0; i < _alpha.size(); i++) {
			if (_alpha[i].size() != other._alpha[i].size())
				return false;
			for (auto j = 0; j < _alpha[i].size(); j++) {
				if (abs(_alpha[i][j] - other._alpha[i][j]) > EPSILON)
					return false;
			}
		}
		return true;
	}
};