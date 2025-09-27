#include "pch.h"
#include "StatePacket.h"
#include "helpers.h"
#include "MyExcept.h"
using namespace helpers;

#ifdef FIRST_LEVEL_ONLY
#ifdef RELATIVE_POSITION
void StatePacket::getFloatVector(std::vector<std::unique_ptr<float>>& vec) const {
	constexpr int componentSize = static_cast<int>(componentCount);
	constexpr int ghostIDSize = static_cast<int>(IDCount);
	constexpr int statusSize = static_cast<int>(statusCount);
	auto insertModifiedPosition = [this, &vec](int index, const Position2D& pos) {
		if (pos != Position2D(-1000, -1000)) {
			*vec[index] = pos._X;
			*vec[index + 1] = pos._Y;
			*vec[index + 2] = 1.0f;
		}
		else {
			*vec[index] = 0.0f;
			*vec[index + 1] = 0.0f;
			*vec[index + 2] = -1.0f;
		}
		};
	if (vec.size() != NETWORK_INPUT_SIZE) {
		MyExcept::error("Incorrect input size");
		return;
	}
	// Player
	auto validate = [](float val) {
		if (abs(val + 1000) < EPSILON)
			return 0.0f;
		return val;
		};
	*vec[0] = validate(_player._position._X);
	*vec[1] = validate(_player._position._Y);
	const int heldItem1 = _player._heldItem[0] == repaired ? 6 : static_cast<int>(_player._heldItem[0]);
	*vec[2] = squishEnum(heldItem1, componentSize);
	const int heldItem2 = _player._heldItem[1] == repaired ? 6 : static_cast<int>(_player._heldItem[1]);
	*vec[3] = squishEnum(heldItem2, componentSize);
	// Zmora 1
	Position2D validZmora1 = getGhostPos(ghostID::zmora1);
	validZmora1._X = validate(validZmora1._X);
	validZmora1._Y = validate(validZmora1._Y);
	*vec[4] = validZmora1._X;
	*vec[5] = validZmora1._Y;
	*vec[6] = validZmora1.relativePos(_player._position)._X;
	*vec[7] = validZmora1.relativePos(_player._position)._Y;
	// Generator
	Position2D validGenerator = getGenerator(0)._position;
	validGenerator._X = validate(validGenerator._X);
	validGenerator._Y = validate(validGenerator._Y);
	*vec[8] = validGenerator._X;
	*vec[9] = validGenerator._Y;
	*vec[10] = validGenerator.relativePos(_player._position)._X;
	*vec[11] = validGenerator.relativePos(_player._position)._Y;
	*vec[12] = _generatorData[2]._broken == static_cast<BYTE>(1) ? 1.0f : -1.0f;
	*vec[13] = squishEnum(static_cast<int>(_generatorData[2]._want1), componentSize);
	*vec[14] = squishEnum(static_cast<int>(_generatorData[2]._want2), componentSize);
	// Components
	Position2D validGear = getMachine(gear)._position;
	validGear._X = validate(validGear._X);
	validGear._Y = validate(validGear._Y);
	*vec[15] = validGear._X;
	*vec[16] = validGear._Y;
	*vec[17] = validGear.relativePos(_player._position)._X;
	*vec[18] = validGear.relativePos(_player._position)._Y;
	*vec[19] = squishEnum(static_cast<int>(getMachine(gear)._status), statusSize);
	Position2D validPipe = getMachine(pipe)._position;
	validPipe._X = validate(validPipe._X);
	validPipe._Y = validate(validPipe._Y);
	*vec[20] = validPipe._X;
	*vec[21] = validPipe._Y;
	*vec[22] = validPipe.relativePos(_player._position)._X;
	*vec[23] = validPipe.relativePos(_player._position)._Y;
	*vec[24] = squishEnum(static_cast<int>(getMachine(pipe)._status), statusSize);
	*vec[25] = abs(_generatorData[2]._time) <= 1.0f + EPSILON ? _generatorData[2]._time : -1.0f;
}
#else
void StatePacket::getDoubleVector(std::vector<std::unique_ptr<double>>& vec) const {
	constexpr int componentSize = static_cast<int>(componentCount);
	constexpr int ghostIDSize = static_cast<int>(IDCount);
	constexpr int statusSize = static_cast<int>(statusCount);
	auto insertModifiedPosition = [this, &vec](int index, const Position2D& pos) {
		if (pos != Position2D(-1000, -1000)) {
			*vec[index] = pos._X;
			*vec[index + 1] = pos._Y;
			*vec[index + 2] = 1.0;
		}
		else {
			*vec[index] = 0.0;
			*vec[index + 1] = 0.0;
			*vec[index + 2] = -1.0;
		}
		};
	if (vec.size() != NETWORK_INPUT_SIZE) {
		MyExcept::error("Incorrect input size");
		return;
	}
	// Player
	auto validate = [](double val) {
		if (abs(val + 1000) < EPSILON)
			return 0.0;
		return val;
		};
	*vec[0] = validate(_player._position._X);
	*vec[1] = validate(_player._position._Y);
	const int heldItem1 = _player._heldItem[0] == repaired ? 6 : static_cast<int>(_player._heldItem[0]);
	*vec[2] = squishEnum(heldItem1, componentSize);
	const int heldItem2 = _player._heldItem[1] == repaired ? 6 : static_cast<int>(_player._heldItem[1]);
	*vec[3] = squishEnum(heldItem2, componentSize);
	// Zmora 1
	*vec[4] = validate(_ghostData[0]._position._X);
	*vec[5] = validate(_ghostData[0]._position._Y);

	// Generator
	*vec[6] = validate(_generatorData[2]._position._X);
	*vec[7] = validate(_generatorData[2]._position._Y);
	*vec[8] = _generatorData[2]._broken == static_cast<BYTE>(1) ? 1.0 : -1.0;
	int want1 = _generatorData[2]._want1;
	int want2 = _generatorData[2]._want2;
	want1 = want1 > 6 ? 6 : want1;
	want2 = want2 > 6 ? 6 : want2;
	*vec[9] = squishEnum(static_cast<int>(want1), componentSize);
	*vec[10] = squishEnum(static_cast<int>(want2), componentSize);
	// Components
	*vec[11] = validate(getMachine(gear)._position._X);
	*vec[12] = validate(getMachine(gear)._position._Y);
	*vec[13] = squishEnum(static_cast<int>(getMachine(gear)._status), statusSize);
	*vec[14] = validate(getMachine(pipe)._position._X);
	*vec[15] = validate(getMachine(pipe)._position._Y);
	*vec[16] = squishEnum(static_cast<int>(getMachine(pipe)._status), statusSize);
	*vec[17] = abs(_generatorData[2]._time) <= 1.0 + EPSILON ? _generatorData[2]._time : -1.0;
}
#endif
#else
void StatePacket::getFloatVector(std::vector<std::unique_ptr<float>>& vec) const {
	constexpr int componentSize = static_cast<int>(componentCount);
	constexpr int ghostIDSize = static_cast<int>(IDCount);
	constexpr int statusSize = static_cast<int>(statusCount);
	auto insertModifiedPosition = [this, &vec](int index, const Position2D& pos) {
		if (pos != Position2D(-1000, -1000)) {
			*vec[index] = pos._X;
			*vec[index + 1] = pos._Y;
			*vec[index + 2] = 1.0f;
		}
		else {
			*vec[index] = 0.0f;
			*vec[index + 1] = 0.0f;
			*vec[index + 2] = -1.0f;
		}
		};
	if (vec.size() != NETWORK_INPUT_SIZE)
		assert(false && "Incorrect network input size");
	// _player
	*vec[0] = abs(_player._position._X + 1000) < EPSILON ? 0 : _player._position._X;
	*vec[1] = abs(_player._position._Y + 1000) < EPSILON ? 0 : _player._position._Y;
	const int heldItem1 = _player._heldItem[0] == repaired ? 6 : static_cast<int>(_player._heldItem[0]);
	*vec[2] = squishEnum(heldItem1, componentSize);
	const int heldItem2 = _player._heldItem[1] == repaired ? 6 : static_cast<int>(_player._heldItem[1]);
	*vec[3] = squishEnum(heldItem2, componentSize);
	// _ghostData[3]
	for (int ghostIndex = 0; ghostIndex < 3; ghostIndex++) {
		const GhostData ghost = _ghostData[ghostIndex];
		insertModifiedPosition(4 + 4 * ghostIndex, ghost._position);
		*vec[7 + 4 * ghostIndex] = squishEnum(static_cast<int>(ghost._ID), ghostIDSize);
	}
	// _generatorData[4]
	for (int genIndex = 0; genIndex < 4; genIndex++) {
		const GeneratorData gen = _generatorData[genIndex];
		insertModifiedPosition(16 + 6 * genIndex, gen._position);
		const int want1 = gen._want1 == repaired ? 6 : static_cast<int>(gen._want1);
		*vec[19 + 6 * genIndex] = squishEnum(want1, componentSize);
		const int want2 = gen._want2 == repaired ? 6 : static_cast<int>(gen._want2);
		*vec[20 + 6 * genIndex] = squishEnum(want2, componentSize);
		*vec[21 + 6 * genIndex] = gen._broken == static_cast<BYTE>(0) ? -1.0f : 1.0f;
	}
	// _componentData[5]
	for (int compIndex = 0; compIndex < 5; compIndex++) {
		const ComponentMachineData comp = _componentData[compIndex];
		const int compID = comp._ID == repaired ? 6 : static_cast<int>(comp._ID);
		*vec[40 + 5 * compIndex] = squishEnum(compID, componentSize);
		insertModifiedPosition(41 + 5 * compIndex, comp._position);
		*vec[44 + 5 * compIndex] = squishEnum(static_cast<int>(comp._status), statusSize);
	}
	// _hazards[3]
	for (int hazIndex = 0; hazIndex < 3; hazIndex++) {
		const Position2D hazard = _hazards[hazIndex];
		insertModifiedPosition(65 + 3 * hazIndex, hazard);
	}
	// _currentLevel
	*vec[74] = squishEnum(_currentLevel, 14);
	// _dashReady
	*vec[75] = _dashReady == static_cast<BYTE>(0) ? -1.0f : 1.0f;
#ifdef APPEND_DRAG
	*vec[76] = abs(_inputDrag._X + 1000) < EPSILON ? 0 : _inputDrag._X;
	*vec[77] = abs(_inputDrag._Y + 1000) < EPSILON ? 0 : _inputDrag._Y;
	// Time the generator has left to be repaired. The network does not care that these are not in close proximity to the other generator data.
	for (auto i = 0; i < 4; i++) {
		const GeneratorData gen = _generatorData[i];
		*vec[78 + i] = gen._time;
	}
#endif
	// Time the generator has left to be repaired. The network does not care that these are not in close proximity to the other generator data.
	for (auto i = 0; i < 4; i++) {
		const GeneratorData gen = _generatorData[i];
		*vec[76 + i] = gen._time;
	}
	// The neural network does NOT need to know when the player dies
}
#endif

void StatePacket::randomize() {
	_player._position._X = randNormalizedDouble();
	_player._position._Y = randNormalizedDouble();
	_player._heldItem[0] = randComponent();
	_player._heldItem[1] = randComponent();
	for (auto& ghost : _ghostData) {
		ghost._ID = rand() % static_cast<int>(IDCount);
		ghost._position._X = randNormalizedDouble();
		ghost._position._Y = randNormalizedDouble();
	}
	for (auto& gen : _generatorData) {
		gen._broken = static_cast<BYTE>(rand() % 2);
		gen._position._X = randNormalizedDouble();
		gen._position._Y = randNormalizedDouble();
		gen._want1 = randComponent();
		gen._want2 = randComponent();
	}
	for (auto& comp : _componentData) {
		comp._ID = randComponent();
		comp._position._X = randNormalizedDouble();
		comp._position._Y = randNormalizedDouble();
		comp._status = static_cast<machineStatus>(rand() % static_cast<int>(statusCount));
	}
	for (auto& haz : _hazards) {
		haz._X = randNormalizedDouble();
		haz._Y = randNormalizedDouble();
	}
	_dashReady = static_cast<BYTE>(rand() % 2);
	_currentLevel = rand() % 13 + 1;
	_playerDeath = static_cast<BYTE>(rand() % 2);
	_inputDrag._X = randNormalizedDouble();
	_inputDrag._Y = randNormalizedDouble();
	for (auto& gen : _generatorData) {
		gen._time = randNormalizedDouble();
	}
}

Position2D StatePacket::getGhostPos(ghostID ghost) const {
	for (int i = 0; i < 3; i++) {
		if (static_cast<ghostID>(_ghostData[i]._ID) == ghost)
			return _ghostData[i]._position;
	}
	return Position2D(-1000, -1000);
}

ComponentMachineData StatePacket::getMachine(component comp) const {
	int foundIndex = -1;
	for (auto i = 0; i < 5; i++)
		if (_componentData[i]._ID == comp) {
			foundIndex = i;
			break;
		}
	if (foundIndex == -1)
#ifdef FIRST_LEVEL_ONLY
		return ComponentMachineData(gear, Position2D(-1000, -1000), ready);
#else
		return ComponentMachineData(repaired, Position2D(-1000, -1000), ready);
#endif
	return _componentData[foundIndex];
}

double StatePacket::randNormalizedDouble() {
	int randInt = rand() % 10000;
	return -1 + (2 * (randInt / 10000.0));
}

bool StatePacket::levelIsLoaded() const {
	bool loaded = false;
	for (auto& componentMachine : _componentData) {
		if (componentMachine._position != Position2D(-1000, -1000))
			loaded = true;
	}
	return loaded;
}

std::string StatePacket::toStr(helpers::ghostID id) {
	switch (id) {
	case bull:
		return "bull";
		break;
	case cutwire1:
		return "cutwire1";
		break;
	case cutwire2:
		return "cutwire2";
		break;
	case nikita:
		return "nikita";
		break;
	case zmora1:
		return "zmora1";
		break;
	case zmora2:
		return "zmora2";
		break;
	case zmora3:
		return "zmora3";
		break;
	case striga:
		return "striga";
		break;
	case nullVal:
		return "null";
		break;
	default:
		MyExcept::error("Invalid ghost ID");
		return "";
		break;
	}
}