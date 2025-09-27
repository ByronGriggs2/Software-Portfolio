#pragma once

#include<vector>

class NetworkData final {
	struct TrainedFrame {
		float _loss;
		float _reward;
		float _epsilon;
		double _biasGradient;
		double _weightGradient;
	};
	std::vector<TrainedFrame> _frameBuffer;
	std::ofstream _ofs;
	bool _initialized = false;

	void saveBufferToDisk();
public:
	NetworkData();
	void initialize();
	bool isInitialized() const { return _initialized; }
	void addFrame(float loss, float reward, float epsilon);
	void addToLastFrame(double biasGradient, double weightGradient) {
		if (_frameBuffer.empty()) {
			return;
		}
		_frameBuffer.back()._biasGradient = biasGradient;
		_frameBuffer.back()._weightGradient = weightGradient;
	}
};