#pragma once
#include <memory>
// Singleton that allows any class to halt execution if an invalid state is detected. Used for debugging in Release mode.
class MLP;
class ReplayBuffer;
class CurriculumManager;
class TrainingState;
class InterfaceServer;
class MyExcept final {
	MyExcept() = default;

	MLP* _DQN = nullptr;
	MLP* _targetNetwork = nullptr;
	ReplayBuffer* _replay = nullptr;
	CurriculumManager* _curriculum = nullptr;
	float* _epsilon = nullptr;
	InterfaceServer* _server = nullptr;
	bool _setup = false;
public:
	MyExcept(const MyExcept& other) = delete;
	MyExcept(MyExcept&& other) = delete;
	MyExcept& operator=(const MyExcept& other) = delete;
	MyExcept& operator=(MyExcept&& other) = delete;

	static MyExcept& getInstance();
	static void setupAutoSave(MLP* DQN, MLP* targetNetwork, ReplayBuffer* replay, CurriculumManager* curriculum, float* epsilon, InterfaceServer*);
	static void error(const char* msg = nullptr);
};