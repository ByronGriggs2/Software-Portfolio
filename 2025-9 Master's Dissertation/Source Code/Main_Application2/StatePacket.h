#pragma once
#include "definitions.h"
#include <vector>
#include <memory>
#include <iostream>

#pragma pack(push, 1)
class StatePacket {
	helpers::Player _player;
	helpers::GhostData _ghostData[3];
	helpers::GeneratorData _generatorData[4];
	helpers::ComponentMachineData _componentData[5];
	helpers::Position2D _hazards[3];
	int _currentLevel = 0;
	BYTE _dashReady = static_cast<BYTE>(1);
	BYTE _playerDeath = static_cast<BYTE>(0);
	helpers::Position2D _inputDrag;

	double randNormalizedDouble();
	friend std::ostream& operator<<(std::ostream&, const StatePacket&);
public:
	helpers::Position2D getPlayerPos() const { return _player._position; }
	helpers::component getHeldItem(int index) const { return _player._heldItem[index]; }
	helpers::GhostData getGhost(int index) const { return _ghostData[index]; }
	helpers::Position2D getGhostPos(helpers::ghostID ghost) const;
	helpers::GeneratorData getGenerator(int index) const { return _generatorData[index]; }
	helpers::ComponentMachineData getMachineAtIndex(int index) const { return _componentData[index]; }
	helpers::ComponentMachineData getMachine(helpers::component) const;
	helpers::Position2D getHazard(int index) const { return _hazards[index]; }
	helpers::Position2D getInputDrag() const { return _inputDrag; }

	bool getDashReady() const { return _dashReady == static_cast<BYTE>(1) ? true : false; }
	bool getPlayerDeath() const { return _playerDeath == static_cast<BYTE>(1) ? true : false; }
	int getCurrentLevel() const { return _currentLevel; }
#ifdef FIRST_LEVEL_ONLY
	void getDoubleVector(std::vector<std::unique_ptr<double>>& vec) const;
#else
	void getDoubleVector(std::vector<std::unique_ptr<double>>& vec) const;
#endif
	bool levelIsLoaded() const;

	static void printToConsole(const StatePacket& packet) { std::cout << packet; }
	void randomize();
	static std::string toStr(helpers::ghostID id);

	bool operator==(const StatePacket& other) const {
		if (!(_player == other._player))
			return false;
		for (auto i = 0; i < 3; i++) {
			if (!(_ghostData[i] == other._ghostData[i]))
				return false;
		}
		for (auto i = 0; i < 4; i++) {
			if (!(_generatorData[i] == other._generatorData[i]))
				return false;
		}
		for (auto i = 0; i < 5; i++) {
			if (!(_componentData[i] == other._componentData[i]))
				return false;
		}
		for (auto i = 0; i < 3; i++) {
			if (!(_hazards[i] == other._hazards[i]))
				return false;
		}
		return
			_currentLevel == other._currentLevel &&
			_dashReady == other._dashReady &&
			_playerDeath == other._playerDeath &&
			_inputDrag == other._inputDrag;
	}
};
#pragma pack(pop)

inline std::ostream& operator<<(std::ostream& os, const StatePacket& packet) {
	os << "Player Position: " << packet._player._position._X << ", " << packet._player._position._Y << std::endl;
	os << "Player items: " << helpers::componentToStr(packet._player._heldItem[0]) << " and " << helpers::componentToStr(packet._player._heldItem[1]) << std::endl;
	for (auto i = 0; i < 3; i++) {
		os << StatePacket::toStr(static_cast<helpers::ghostID>(packet._ghostData[i]._ID)) << " Position: " << packet._ghostData[i]._position._X << ", " << packet._ghostData[i]._position._Y << std::endl;
	}
	for (auto i = 0; i < 4; i++) {
		os << "Generator " << i << " (" << packet._generatorData[i]._position._X <<", " << packet._generatorData[i]._position._Y << ")" " is ";
		if (packet._generatorData[i]._broken)
			os << "broken and";
		else
			os << "not broken and";
		os << " wants " << packet._generatorData[i]._want1 << " and " << packet._generatorData[i]._want2 << std::endl;
	}
	for (auto comp : packet._componentData) {
		os << "Component Machine " << helpers::componentToStr(comp._ID) << " at (" << comp._position._X << ", " << comp._position._Y << ") " << " is " << helpers::statusToStr(comp._status) << std::endl;
	}
	for (int i = 0; i < 3; i++) {
		os << "Hazard " << i << " is at (" << packet._hazards[i]._X << ", " << packet._hazards[i]._Y << ")" << std::endl;
	}
	os << "Current Level: " << static_cast<int>(packet._currentLevel) << std::endl;
	os << "Player dash ready: " << static_cast<int>(packet._dashReady) << std::endl;
	os << "Player died this frame: " << static_cast<bool>(packet._playerDeath) << std::endl << std::endl;
	return os;
}

struct Transition {
	StatePacket _S0;
	helpers::playerAction _A0 = helpers::playerAction::noop;
	StatePacket _S1;
	double _R1 = 0;
	bool _F = false;
	bool _valid = false;

	bool playerLeftLevel() const { return _S0.getCurrentLevel() != 0 && _S1.getCurrentLevel() == 0; }
	void printToConsole() const {
		std::cout << "    S0: " << std::endl;
		StatePacket::printToConsole(_S0);
		std::cout << "    S1: " << std::endl;
		StatePacket::printToConsole(_S1);
		std::cout << "    A0: " << static_cast<int>(_A0) << std::endl;
		std::cout << "    R1: " << _R1 << std::endl;
		std::cout << "    F: " << _F << std::endl;
		std::cout << "    valid: " << _valid << std::endl;
		};
	bool operator==(const Transition& other) const {
		return _S0 == other._S0 && _A0 == other._A0 && _S1 == other._S1 && _R1 == other._R1 && _F == other._F && _valid == other._valid;
	}
};