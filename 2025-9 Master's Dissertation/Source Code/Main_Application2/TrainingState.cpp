#include "pch.h"
#include "TrainingState.h"
#include "MLP.h"
#include "ReplayBuffer.h"
#include "CurriculumManager.h"
#include "helpers.h"

constexpr const char* DQNFile = "toLoadAsDQN";
constexpr const char* targetNetworkFile = "toLoadAsTargetNetwork";
constexpr const char* replayBufferFile = "toLoadAsReplayBuffer.txt";
constexpr const char* epsilonFile = "toLoadAsEpsilon.txt";
constexpr const char* curriculumFile = "toLoadAsCurriculum.txt";

void TrainingState::saveTrainingState(MLP* DQN, MLP* targetNetwork, ReplayBuffer* replayBuffer, const CurriculumManager* curriculum, const float* epsilon, bool error) {
	if (error) {
		const std::string err("Error");
		DQN->saveWeightsAndBiases(err + DQNFile);
		targetNetwork->saveWeightsAndBiases(err + targetNetworkFile);
		replayBuffer->saveToDisk(err + replayBufferFile);
		curriculum->saveToDisk(err + curriculumFile);
		std::ofstream ofs(err + epsilonFile);
		ofs << *epsilon;
	}
	else {
		DQN->saveWeightsAndBiases(DQNFile);
		targetNetwork->saveWeightsAndBiases(targetNetworkFile);
		replayBuffer->saveToDisk(replayBufferFile);
		curriculum->saveToDisk(curriculumFile);
		std::ofstream ofs(epsilonFile);
		ofs << *epsilon;
	}
}

void TrainingState::loadTrainingState(MLP* DQN, MLP* targetNetwork, ReplayBuffer* replayBuffer, CurriculumManager* curriculum, float* epsilon, bool error) {
	auto findMostRecentFilename = [](std::string filename) {
		int i = 1;
		std::filesystem::path currentPath = std::filesystem::current_path();
		currentPath = currentPath / "Network_Backup";
		while (true) {
			if (!helpers::fileExists(currentPath, filename, ".txt", i))
				break;
			i++;
		}
		std::string fullname = filename + helpers::intToStr(i-1) + ".txt";
		return fullname;
		};
	if (error) {
		const std::string err("Error");
		DQN->loadWeightsAndBiases(findMostRecentFilename(err + DQNFile));
		targetNetwork->loadWeightsAndBiases(findMostRecentFilename(err+targetNetworkFile));
		replayBuffer->loadFromDisk(err + replayBufferFile);
		curriculum->loadFromDisk(err + curriculumFile);
		std::ifstream ifs(err + epsilonFile);
		ifs >> *epsilon;
		return;
	}
	else {
		DQN->loadWeightsAndBiases(findMostRecentFilename(DQNFile));
		targetNetwork->loadWeightsAndBiases(findMostRecentFilename(targetNetworkFile));
		replayBuffer->loadFromDisk(replayBufferFile);
		curriculum->loadFromDisk(curriculumFile);
		std::ifstream ifs(epsilonFile);
		ifs >> *epsilon;
	}
}