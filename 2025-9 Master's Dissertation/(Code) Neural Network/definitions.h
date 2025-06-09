#pragma once

constexpr int HIDDEN_LAYER_COUNT = 5;
constexpr int NEURON_PER_LAYER_COUNT = 20;

namespace helpers {
	enum playerAction {
		noop,
		moveNorth,
		moveNorthwest,
		moveWest,
		moveSouthwest,
		moveSouth,
		moveSoutheast,
		moveEast,
		moveNortheast,
		dash,
		interact,
		actionCount
	};
	enum neuronType {
		normal,
		output
	};
}
