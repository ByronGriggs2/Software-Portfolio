#pragma once
struct Transition;
class RewardManager final
{
public:
	RewardManager() = delete;
	RewardManager(const RewardManager& other) = delete;
	RewardManager(RewardManager&& other) noexcept = delete;
	RewardManager& operator=(const RewardManager& other) = delete;
	RewardManager& operator=(RewardManager&& other) noexcept = delete;

	static double calculateReward(const Transition& transition);
};

