#include "pch.h"
#include "MaximumRewardTest.h"
#include "MLP.h"
#include "DeepQManager.h"

using std::cout, std::endl, std::vector;

void MaximumRewardTest::test() {
	vector<float> maxRewards;
	for (int testNum = 0; testNum < 100; testNum++) {
		for (int reward = 0; reward < 10001; reward++) {
			Transition dummyTransition;
			dummyTransition._S0.randomize();
			dummyTransition._S1.randomize();
			dummyTransition._R1 = static_cast<float>(reward);
			
			float highest = _deepQ->findLargestGradient(dummyTransition);
			if (abs(highest) > 1e6) {
				maxRewards.push_back(static_cast<float>(reward-1));
				cout << "Test " << testNum << " reward: " << reward-1 << endl;
				break;
			}
			if (reward == 10000) {
				_deepQ->findLargestGradient(dummyTransition);
				maxRewards.push_back(static_cast<float>(reward));
				cout << "Test " << testNum << " reward: " << reward << endl;
			}
		}
	}
	cout << endl << "Test complete. Statistics: ";
	cout << endl << "Max: ";
	float max = 0;
	for (const auto& reward : maxRewards) {
		if (reward > max) {
			max = reward;
		}
	}
	cout << max;
	cout << endl << "Min: ";
	float min = 1001;
	for (const auto& reward : maxRewards) {
		if (reward < min) {
			min = reward;
		}
	}
	cout << min;
	cout << endl << "Average: ";
	float sum = 0;
	for (const auto& reward : maxRewards) {
		sum += reward;
	}
	cout << sum / maxRewards.size() << endl;
}