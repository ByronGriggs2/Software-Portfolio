#pragma once
#include "StatePacket.h"
#include <vector>

class CurriculumManager final {
	int _currentlyTrainingLevel = 1;
	int _wins = 0;
	int _losses = 0;
	int _totalEpisodes = 0;
	float _winrates[13];
	bool _completed[13];

	void selectNewLevel();
public:
	CurriculumManager() {
		for (auto& winrate : _winrates) {
			winrate = 0.0f;
		}
		for (auto& completed : _completed) {
			completed = false;
		}
	}
	void onDeath();
	void onVictory();
	int getNextLevel() const { return _currentlyTrainingLevel; }
	void update(const Transition& transition, float& epsilon);
	void saveToDisk(const std::string& filename) const;
	void loadFromDisk(const std::string& filename);
	int getEpisodes() const { return _totalEpisodes; }
	void resetEpisodes() { _totalEpisodes = 0; }

	bool operator==(const CurriculumManager& other) const;
};