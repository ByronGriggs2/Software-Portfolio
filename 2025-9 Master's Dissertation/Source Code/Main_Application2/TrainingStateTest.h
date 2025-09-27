#pragma once

class DeepQManager;
class MLP;
class CurriculumManager;
class TrainingStateTest
{
	DeepQManager* _DeepQ;
	MLP* _DQN;
	std::unique_ptr<CurriculumManager> _Curriculum;
	float _epsilon;

	void copyOfNeuralNetworkTest();
	bool validateTrainingTest(bool error = false);
public:
	TrainingStateTest() = delete;
	TrainingStateTest(MLP* mlp, DeepQManager* deepQ) : _DeepQ(deepQ), _DQN(mlp) {}

	void test();
};

