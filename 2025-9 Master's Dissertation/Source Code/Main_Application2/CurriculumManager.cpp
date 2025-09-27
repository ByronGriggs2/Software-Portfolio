#include "pch.h"
#include "CurriculumManager.h"
#include "MyExcept.h"

using namespace helpers;
void CurriculumManager::update(const Transition& transition, float& epsilon) {
	if (transition._S1.getPlayerDeath())
		onDeath();
	else if (transition.playerLeftLevel())
		onVictory();
	if (_wins + _losses >= EPISODE_BATCH) {
		_winrates[_currentlyTrainingLevel] = _wins / (_wins + _losses);
		selectNewLevel();
		_wins = 0;
		_losses = 0;
	}
	const int validEpisodes = _totalEpisodes > NO_TRAINING_DURATION ? _totalEpisodes - NO_TRAINING_DURATION : 0;
	const float validCompletionPercentage = static_cast<float>(validEpisodes) / static_cast<float>(TARGET_EPISODES - NO_TRAINING_DURATION);
#ifdef CONSTANT_EPSILON
	epsilon = 0.05;
#endif
#ifdef LINEAR_EPSILON
		epsilon = 1.0 - validCompletionPercentage;
#endif
#ifdef GENTLE_EXPONENTIAL_EPSILON
	constexpr float e = 2.718281828459045f;
	constexpr float a = 0.26;
	constexpr float b = 1.6;
	constexpr float offset = (1 + a);
	
	const float temp = -a * pow(e, b * validCompletionPercentage) + offset;
	epsilon = temp >= 0 ? temp : 0;
#endif
}

void CurriculumManager::selectNewLevel() {
#ifdef FIRST_LEVEL_ONLY
	return;
#else
	std::vector<float> pickWeights = std::vector<float>(13, EPSILON);
	// Very early training: levels 1, 2, or 3 never completed
	if (!_completed[0] || !_completed[1] || !_completed[2]) {
		if (!_completed[0]) {
			pickWeights[0] = 0.7f;
			pickWeights[1] = 0.15f;
			pickWeights[2] = 0.15f;
		}
		else if (!_completed[1]) {
			pickWeights[0] = 0.15f;
			pickWeights[1] = 0.15f;
			pickWeights[2] = 0.7f;
		}
		else {
			pickWeights[0] = 0.15f;
			pickWeights[1] = 0.15f;
			pickWeights[2] = 0.7f;
		}
	}
	else {
		int firstNotCompleted = -1;
		for (auto i = 0; i < 13; i++) {
			if (!_completed[i]) {
				firstNotCompleted = i;
				break;
			}
		}
		// Early training: at least one level has not been completed
		if (firstNotCompleted != -1) {
			for (auto i = 0; i < firstNotCompleted; i++) {
				pickWeights[i] = 1.0f;
			}
			pickWeights[firstNotCompleted] = 4.0f;
		}
		// Late training: all levels have been completed at least once
		else {
			// Prevent going too long without review by giving a 5% chance of unweighted selection
			if (rand() % 100 <= 5) {
				pickWeights = std::vector<float>(13, 1.0f);
			}
			for (auto i = 0; i < 13; i++) {
				pickWeights[i] = 1.0 - _winrates[i] + EPSILON;
			}
		}
	}
	float weightSum = 0;
	for (auto& weight : pickWeights)
		weightSum += weight;
	std::vector<float> pickProbability;
	for (auto& weight : pickWeights)
		pickProbability.push_back(weight / weightSum);

	const float randNum = static_cast<float>(rand() % 10000) / 10000.0f;
	float cumulativeProbability = 0.0f;
	for (int i = 0; i < pickProbability.size(); i++) {
		cumulativeProbability += pickProbability[i];
		if (randNum <= cumulativeProbability) {
			_currentlyTrainingLevel = i+1;
			return "OK";
		}
	}
	return "No level selected";
#endif
}

void CurriculumManager::onDeath() {
	_losses++;
	_totalEpisodes++;
}

void CurriculumManager::onVictory() {
	_wins++;
	_totalEpisodes++;
	_completed[_currentlyTrainingLevel] = true;
}

void CurriculumManager::saveToDisk(const std::string& filename) const {
	std::filesystem::path currentPath = std::filesystem::current_path();
	currentPath = currentPath / "CurriculumStorage";
	if (!std::filesystem::exists(currentPath))
		std::filesystem::create_directory(currentPath);
	currentPath = currentPath / filename;
	std::ofstream ofs(currentPath, std::ios::binary);
	ofs.write(reinterpret_cast<const char*>(this), sizeof(CurriculumManager));
}

void CurriculumManager::loadFromDisk(const std::string& filename) {
	std::filesystem::path currentPath = std::filesystem::current_path();
	currentPath = currentPath / "CurriculumStorage";
	currentPath = currentPath / filename;
	std::ifstream ifs(currentPath, std::ios::binary);
	if (!ifs.is_open()) {
		std::string str = "Failed to open file: ";
		str = str + filename;
		MyExcept::error(filename.c_str());
	}
	ifs.read(reinterpret_cast<char*>(this), sizeof(CurriculumManager));
}

bool CurriculumManager::operator==(const CurriculumManager& other) const {
	for (int i = 0; i < 13; i++) {
		if (_winrates[i] != other._winrates[i])
			return false;
		if (_completed[i] != other._completed[i])
			return false;
	}
	return _currentlyTrainingLevel == other._currentlyTrainingLevel &&
		_wins == other._wins &&
		_losses == other._losses &&
		_totalEpisodes == other._totalEpisodes;
}