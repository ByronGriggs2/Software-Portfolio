#include "pch.h"
#include "NetworkData.h"
#include "definitions.h"
#include "StatePacket.h"
#include "helpers.h"
#include "MyExcept.h"

using std::endl;

NetworkData::NetworkData() : _frameBuffer(), _ofs() {
	_frameBuffer.reserve(FRAMERATE * 10);
}

void NetworkData::initialize() {
    const std::filesystem::path fileDirectory = helpers::getWritePath("Network_statistics", "training_attempt", ".csv");
    _ofs.open(fileDirectory);
    _ofs << "Hidden layers," << HIDDEN_LAYER_COUNT << endl;
    _ofs << "Neurons per layer," << NEURON_PER_LAYER_COUNT << endl;
    _ofs << "Input size," << NETWORK_INPUT_SIZE << endl;
    _ofs << "Learning rate," << LEARNING_RATE << endl;
    _ofs << "Batch size," << BATCH_SIZE << endl;
    _ofs << "Discount," << DISCOUNT << endl;
    _ofs << "Update ratio," << TQN_UPDATE_RATIO << endl;
    _ofs << "Replay buffer size," << REPLAY_BUFFER_SIZE << endl;
    _ofs << "Target episodes," << TARGET_EPISODES << endl;
    _ofs << "Epsilon decay function,";
#ifdef AGGRESSIVE_EXPONENTIAL_EPSILON
    _ofs << "Aggressive exponential";
#else
#ifdef EXPONENTIAL_EPSILON
    _ofs << "exponential";
#else
#ifdef LINEAR_EPSILON
    _ofs << "linear";
#endif
#endif
#endif
    _ofs << endl;
    _ofs << "Reward multiplier," << REWARD_MULTIPLIER << endl;
    _ofs << "Input type,";
#ifdef MINIMAL_INPUTS
    _ofs << "Minimal" << endl;
#else
    _ofs << "Default" << endl;
#endif
    _ofs << "" << endl;
    _ofs << "Loss,Reward,Epsilon,Epsilon adjusted loss,Epsilon adjusted reward,bias gradient,weight gradient" << endl;
    _initialized = true;
}

void NetworkData::addFrame(float loss, float reward, float epsilon) {
    // 10x max expected value
    if (abs(reward > 1000)) {
        MyExcept::error((std::string("Abnormally high reward: ") + std::to_string(reward)).c_str());
	}
	// 10x max expected value
    if (abs(loss) > 100000) {
        MyExcept::error((std::string("Abnormally high loss: ") + std::to_string(loss)).c_str());
    }
	if (_frameBuffer.size() >= FRAMERATE * 10)
		saveBufferToDisk();
	_frameBuffer.push_back({ loss, reward, epsilon });
}

void NetworkData::saveBufferToDisk() {
    const int frameCount = static_cast<int>(_frameBuffer.size());
    float averageLoss = 0;
    float averageReward = 0;
    float averageEpsilon = 0;
	double averageBiasGradient = 0;
	double averageWeightGradient = 0;
    for (const auto& frame : _frameBuffer) {
        averageLoss += frame._loss;
        averageReward += frame._reward;
        averageEpsilon += frame._epsilon;
        averageBiasGradient += frame._biasGradient;
		averageWeightGradient += frame._weightGradient;
    }
    averageLoss /= frameCount;
    averageReward /= frameCount;
    averageEpsilon /= frameCount;
	averageBiasGradient /= frameCount;
	averageWeightGradient /= frameCount;
    _ofs << std::fixed << std::setprecision(15) << averageLoss << "," << averageReward << "," << averageEpsilon << "," << averageLoss * averageEpsilon << "," << averageReward * averageEpsilon << "," << averageBiasGradient << "," << averageWeightGradient << endl;
    _frameBuffer.clear();
}