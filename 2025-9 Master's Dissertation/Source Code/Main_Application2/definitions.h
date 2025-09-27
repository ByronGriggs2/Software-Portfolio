#pragma once

//#define APPEND_DRAG
//#define AGGRESSIVE_EXPONENTIAL_EPSILON
//#define EXPONENTIAL_EPSILON
#define LINEAR_EPSILON
//#define GENTLE_EXPONENTIAL_EPSILON
//#define SAFE_TO_TAB_OUT
//#define CONSTANT_EPSILON
#define FIRST_LEVEL_ONLY
//#define RELATIVE_POSITION
//#define EASY_MODE

typedef unsigned char BYTE;
typedef unsigned long DWORD;

constexpr int HIDDEN_LAYER_COUNT = 2;
constexpr int NEURON_PER_LAYER_COUNT = 64;
#ifdef FIRST_LEVEL_ONLY
#ifdef RELATIVE_POSITION
constexpr int NETWORK_INPUT_SIZE = 26;
#else
constexpr int NETWORK_INPUT_SIZE = 18;
#endif
#else
#ifdef APPEND_DRAG
constexpr int NETWORK_INPUT_SIZE = 82;
#else
constexpr int NETWORK_INPUT_SIZE = 80;
#endif
#endif
constexpr double LEARNING_RATE = 1e-4;
constexpr int BATCH_SIZE = 32;
//epsilon is variable
constexpr float DISCOUNT = 0.99f;
constexpr int TQN_UPDATE_RATIO = 1000;
constexpr int REPLAY_BUFFER_SIZE = 100000;

constexpr int TARGET_EPISODES = 20;
#ifdef EASY_MODE
constexpr int NO_TRAINING_DURATION = 0;
constexpr int EPISODE_BATCH = 20;
#else
constexpr int NO_TRAINING_DURATION = 15;
constexpr int EPISODE_BATCH = 100;
#endif
constexpr double REWARD_MULTIPLIER = 1;

constexpr int FRAMERATE = 60;
constexpr double EPSILON = 1e-5;
constexpr DWORD ONEFRAMEMS = static_cast<DWORD>(1000.0f / FRAMERATE);
constexpr DWORD GENERATOR_DEATH_DURATION_MS = static_cast<DWORD>(2960 + ONEFRAMEMS);
constexpr DWORD ULA_DEATH_DURATION_MS = static_cast<DWORD>(2961) + ONEFRAMEMS;
constexpr DWORD YAGA_DEATH_DURATION_MS = static_cast<DWORD>(2979 + ONEFRAMEMS);
constexpr DWORD SUPER_DEATH_DURATION_MS = static_cast<DWORD>(2987) + ONEFRAMEMS;
constexpr DWORD ULA_INTRO_DURATION_MS = static_cast<DWORD>(2753) + ONEFRAMEMS;
constexpr DWORD YAGA_INTRO_DURATION_MS = static_cast<DWORD>(2921) + ONEFRAMEMS;
constexpr DWORD SUPER_INTRO_DURATION_MS = static_cast<DWORD>(2941) + ONEFRAMEMS;
constexpr DWORD BLOCKED_INPUTS_ON_LOAD_DURATION_MS = static_cast<DWORD>(3378 - SUPER_INTRO_DURATION_MS + 2 * ONEFRAMEMS);

namespace helpers {
	enum playerAction {
		noop,//0
		moveNorth,//1
		moveNorthwest,//2
		moveWest, //3
		moveSouthwest,//4
		moveSouth,//5
		moveSoutheast,//6
		moveEast,//7
		moveNortheast,//8
		dashNorth,//9
		dashNorthwest,//10
		dashWest,//11
		dashSouthwest,//12
		dashSouth,//13
		dashSoutheast,//14
		dashEast,//15
		dashNortheast,//16
		interact,//17
		cancel,//18
		actionCount//19
	};
	enum playerActionAlternate {
		noop_a,//0
		moveNorth_a,//1
		moveNorthwest_a,//2
		moveWest_a, //3
		moveSouthwest_a,//4
		moveSouth_a,//5
		moveSoutheast_a,//6
		moveEast_a,//7
		moveNortheast_a,//8
		interact_a,//9
		actionCount_a//10
	};
	enum neuronType {
		normal,
		output,
		typeCount
	};
	enum appMode {
		playGame,
		test1,
		neuralNetworkTest,
		training,
		loadTrainedNetwork,
		sanityCheck,
		responsiveTest,
		modeCount
	};
	enum ghostID
	{
		bull,
		cutwire1,
		cutwire2,
		nikita,
		zmora1,
		zmora2,
		zmora3,
		striga,
		nullVal,
		IDCount
	};
	enum component {
		none,
		gear,
		pipe,
		redBattery,
		blueBattery,
		blueThing,
		repaired = 10,
		componentCount = 7
	};
	inline component randComponent() {
		int randInt = rand() % componentCount;
		randInt = randInt == 6 ? 10 : randInt;
		return static_cast<component>(randInt);
	}
	std::string componentToStr(component);
	enum machineStatus {
		empty,
		working,
		ready,
		statusCount
	};
	std::string statusToStr(machineStatus);
#pragma pack(push, 1)
	struct Position2D {
		float _X = -1000.0f;
		float _Y = -1000.0f;
		bool operator==(const Position2D& other)const { return (abs(_X-other._X) < EPSILON) && (abs(_Y-other._Y) < EPSILON); }
		bool operator!=(const Position2D& other)const { return (abs(_X-other._X) > EPSILON) || (abs(_Y-other._Y) > EPSILON); }
		Position2D relativePos(Position2D other) const {
			return Position2D{ _X - other._X, _Y - other._Y };
		}
	};
#pragma pack(pop)
#pragma pack(push, 1)
	struct GeneratorData {
		Position2D _position;
		int _want1 = 0;
		int _want2 = 0;
		BYTE _broken = 0;
		float _time = 0.0f;
		bool operator==(const GeneratorData& other) const { return _position == other._position && _want1 == other._want1 && _want2 == other._want2 && _broken == other._broken && _time == other._time; }
	};
#pragma pack(pop)
#pragma pack(push, 1)
	struct ComponentMachineData {
		component _ID = none;
		Position2D _position;
		machineStatus _status = empty;
		bool operator==(const ComponentMachineData& other) const { return _ID == other._ID && _position == other._position && _status == other._status; }
	};
#pragma pack(pop)
#pragma pack(push, 1)
	struct GhostData {
		Position2D _position;
		int _ID = 8;
		bool operator==(const GhostData& other) const { return _position == other._position && _ID == other._ID; }
	};
#pragma pack(pop)
#pragma pack(push, 1)
	struct Player {
		Position2D _position;
		component _heldItem[2];
		Player() {
			_heldItem[0] = none;
			_heldItem[1] = none;
		}
		bool operator==(const Player& other) const { return _position == other._position && _heldItem[0] == other._heldItem[0] && _heldItem[1] == other._heldItem[1]; }
	};
#pragma pack(pop)
	inline double squishEnum(int enumVal, int enumCount) {
		const double stepSize = 2.0 / (enumCount-1);
		return -1.0 + enumVal * stepSize;
	}
}
