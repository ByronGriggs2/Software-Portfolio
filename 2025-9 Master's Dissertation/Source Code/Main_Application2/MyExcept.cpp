#include "pch.h"
#include "MyExcept.h"
#include "helpers.h"
#include "TrainingState.h"
#include "InterfaceServer.h"

using std::cout, std::endl;

MyExcept& MyExcept::getInstance() {
	static MyExcept instance;
	return instance;
}

void MyExcept::setupAutoSave(MLP* DQN, MLP* targetNetwork, ReplayBuffer* replay, CurriculumManager* curriculum, float* epsilon, InterfaceServer* server) {
	if (DQN == nullptr || targetNetwork == nullptr || replay == nullptr || curriculum == nullptr || epsilon == nullptr || server == nullptr)
		throw std::runtime_error("Invalid auto save setup");
	MyExcept& instance = getInstance();
	instance._DQN = DQN;
	instance._targetNetwork = targetNetwork;
	instance._replay = replay;
	instance._curriculum = curriculum;
	instance._epsilon = epsilon;
	instance._server = server;
	instance._setup = true;
}

void MyExcept::error(const char* msg) {
	MyExcept& instance = getInstance();
	helpers::printSystemTime();
	cout << "####ERROR: MyExcept called: ";
	if (msg != nullptr)
		cout << msg;
	cout << endl;
	// Allows code to call exceptions without setting up the autosave
	if (instance._setup) {
		if (instance._server != nullptr)
			instance._server->stopComms(true);
		TrainingState::saveTrainingState(instance._DQN, instance._targetNetwork, instance._replay, instance._curriculum, instance._epsilon, true);
	}
	__debugbreak();
}