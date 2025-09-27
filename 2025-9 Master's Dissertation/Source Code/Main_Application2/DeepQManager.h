#pragma once
#include "ReplayBuffer.h"
#include "MLP.h"
#include "NetworkData.h"
#include <vector>

// NOTE: m x 1 and 1 x n matrices are represented as 1D vectors, while m x n matrices are represented as 2D vectors. Whether the 1D vector is a row matrix (1 x n) or a column matrix (m x 1) is specified 
// above each function that uses one as a parameter.

class DeepQManager final {
	// Owned by Control
	MLP* _DQN;
	// Owned by this
	MLP _targetNetwork;
	ReplayBuffer _replayBuffer;
	NetworkData _networkData;

	void stochasticGradientDescent(const std::vector<Transition>& sample);
	double calculateLoss(const Transition& transition);
	void calculateDeltaAndAlphaForTransition(const Transition& trans, std::vector<std::vector<double>>& miniDeltaArray, std::vector<std::vector<double>>& miniAlphaArray);
public:
	DeepQManager() = delete;
	DeepQManager(MLP* dqn) : _DQN(dqn), _targetNetwork(*dqn), _replayBuffer() {}
	DeepQManager(const DeepQManager& other) = delete;
	DeepQManager(DeepQManager&& other) noexcept = delete;
	DeepQManager& operator=(const DeepQManager& other) = delete;
	DeepQManager& operator=(DeepQManager&& other) noexcept = delete;
	void trainingStep(const Transition& lastFrame, float epsilon, bool replayBufferOnly = false);
	ReplayBuffer* getReplayBuffer() { return &_replayBuffer; }
	MLP* getTargetNetwork() { return &_targetNetwork; }
	float findLargestGradient(const Transition& trans);
};