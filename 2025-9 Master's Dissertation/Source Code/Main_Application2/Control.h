#pragma once
#include "InterfaceServer.h"
#include "StatePacket.h"
#include "ReplayBuffer.h"
#include "CurriculumManager.h"
class MLP;
class DeepQManager;

namespace helpers {
	enum appMode;
}

class Control final
{
	float _epsilon = 1.0f;
	InterfaceServer _server;
	std::unique_ptr<MLP> _DQN;
	std::unique_ptr<DeepQManager> _DeepQ;
	CurriculumManager _curriculumManager;

	// Training
	void fpsCheck(LARGE_INTEGER&, const LARGE_INTEGER&, bool& ignoreOnce);
	Transition getNextTrainingTransition(int episodes);
	void handlePlayerDeath(int currentLevel);
	void handlePlayerWin(int currentLevel);

	void initializeControl(bool usingServer = false, bool usingDirectResponse = false, bool usingWatchDog = false, bool usingDeepQ = false);
public:
	Control();
	Control(const Control& other) = delete;
	Control(Control&& other) noexcept = delete;
	Control& operator=(const Control& other) = delete;
	Control& operator=(Control&& other) noexcept = delete;
	~Control();

	void trainingFunct(int startLevel, bool ignoreFirstFPSCheck = false);
	void startTraining();
	void resumeTraining(bool error = false, bool resetEpisodes = false);
	void loadAndEvaluateNetwork(const std::string& filename);

	void neuralNetworkTest();
	void cheatCodeTest();
	void responsivenessTest(bool fullTest = false);
	void inOutTest();
	void desyncTest();
	void trainingStateTest();
	void maximumRewardTest();
};

