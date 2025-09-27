#pragma once

class MLP;
class DeepQManager;
class MaximumRewardTest final
{
	std::unique_ptr<MLP> _DQN;
	std::unique_ptr<DeepQManager> _deepQ;
public:
	MaximumRewardTest() : _DQN(std::make_unique<MLP>()), _deepQ(std::make_unique<DeepQManager>(_DQN.get())) {}
	void test();
};