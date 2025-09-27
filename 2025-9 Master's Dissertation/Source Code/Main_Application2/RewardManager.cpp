#include "pch.h"
#include "RewardManager.h"
#include "StatePacket.h"
#include "definitions.h"
#include "MyExcept.h"

// time penalty temporarily disabled, as the agent has no reason to hurry right now
//constexpr double time_penalty = -0.05;
constexpr double time_penalty = 0.0;
constexpr double time_penalty_1s = FRAMERATE * time_penalty;
// death penalty also temporarily disabled, as I want the network to prefer fix generator->die over don't fix generator->time out. Later on dying will need to have a larger magnitude than fixing a generator, so that the network doesn't deliberately die
// to fix a generator.
//constexpr double death_penalty = -100.0;
constexpr double death_penalty = 0.0;
constexpr double win_reward = 2;
constexpr double cutsceneFinish_reward = 1;
constexpr double generatorFix_reward = 1;
constexpr double pickupItemWhileEmpty_reward = 0.1;
constexpr double activateEmptyMachine_reward = 0.1;
constexpr double checkGenerator_reward = 0.1;
constexpr double pickupHaveItem_penalty = -0.15;
constexpr double pointlessInteract_penalty = -0.004;

using namespace helpers;

namespace {
	bool generatorFix(const Transition& trans) {
		for (int i = 0; i < 4; i++) {
			if (trans._S0.getGenerator(i)._broken && !trans._S1.getGenerator(i)._broken)
				return true;
		}
		return false;
	}
	bool pickedUpItemGood(const Transition& trans) {
		for (int i = 0; i < 2; i++) {
			if (trans._S0.getHeldItem(i) == none && trans._S0.getHeldItem(i) != none)
				return true;
		}
		return false;
	}
	bool pickedUpItemBad(const Transition& trans) {
		component hand0 = trans._S0.getHeldItem(0);
		component hand1 = trans._S0.getHeldItem(1);
		if (hand0 != hand1)
			return false;
		if (trans._S0.getMachine(hand0)._status == ready && trans._S1.getMachine(hand0)._status != ready)
			return true;
		return false;
	}
	bool activatedEmptyMachine(const Transition& trans) {
		for (int i = 0; i < 5; i++) {
			if (trans._S0.getMachineAtIndex(i)._status == empty && trans._S1.getMachineAtIndex(i)._status != empty) {
				return true;
			}
		}
		return false;
	}
	bool checkedGenerator(const Transition& trans) {
		for (int i = 0; i < 4; i++) {
			const GeneratorData gen0 = trans._S0.getGenerator(i);
			const GeneratorData gen1 = trans._S1.getGenerator(i);
			const bool genReadyToBeChecked = gen0._broken && gen0._want1 == none && gen0._want2 == none;
			if (genReadyToBeChecked && gen1._want1 != none)
				return true;
		}
		return false;
	}
	bool playerDiedToHazard(const Transition& trans) {
		if (!trans._S1.getPlayerDeath())
			return false;
		for (int i = 0; i < 4; i++) {
			if (abs(trans._S0.getGenerator(i)._time) < EPSILON*10)
				return false;
		}
		return true;
	}
	bool playerWin(const Transition& trans) {
		// Winning the level is only distinguishable from dying using the player death flag
		if (trans._S0.getPlayerDeath() || trans._S1.getPlayerDeath())
			return false;
		bool winnable = false;
		for (auto i = 0; i < 4; i++) {
			if (trans._S0.getGenerator(i)._position != Position2D(-1000, -1000)) {
				winnable = true;
				break;
			}
		}
		if (!winnable)
			return false;
		for (auto i = 0; i < 4; i++) {
			if (trans._S1.getGenerator(i)._position != Position2D(-1000, -1000))
				return false;
		}
		return true;
	}
	bool cutsceneFinished(const Transition& trans) {
		return (trans._S0.getCurrentLevel() != 0 && trans._S1.getCurrentLevel() == 0);
	}
	bool pointlessInteract(const Transition& trans) {
		if (trans._A0 != interact)
			return false;
		component oldHands[2];
		oldHands[0] = trans._S0.getHeldItem(0);
		oldHands[1] = trans._S0.getHeldItem(1);
		component newHands[2];
		newHands[0] = trans._S1.getHeldItem(0);
		newHands[1] = trans._S1.getHeldItem(1);
		if (oldHands[0] != newHands[0] || oldHands[1] != newHands[1]) {
			return false;
		}
		return (!playerWin(trans) && !checkedGenerator(trans) && !generatorFix(trans) && !activatedEmptyMachine(trans));
	}
}

double RewardManager::calculateReward(const Transition& transition) {
	// Shaping rewards
	double reward = time_penalty;
	if (generatorFix(transition)) {
		reward += generatorFix_reward;
	}
	if (pickedUpItemGood(transition))
		reward += pickupItemWhileEmpty_reward;
	if (pickedUpItemBad(transition))
		reward += pickupHaveItem_penalty;
	if (activatedEmptyMachine(transition))
		reward += activateEmptyMachine_reward;
	if (checkedGenerator(transition))
		reward += checkGenerator_reward;
	if (pointlessInteract(transition))
		reward += pointlessInteract_penalty;

	// Terminal rewards
	if (playerDiedToHazard(transition)) {
		reward += death_penalty;
		std::cout << "Player died to a hazard. Either you have a bug or you fixed a generator! Yay!" << std::endl;
	}
	else if (playerWin(transition)) 
		reward += win_reward;
	else if (cutsceneFinished(transition))
		reward += cutsceneFinish_reward;
	if (reward > 1.3* win_reward) {
		std::cout << "####WARNING: reward of " << reward << " calculated" << std::endl;
	}
	return reward * REWARD_MULTIPLIER;
}