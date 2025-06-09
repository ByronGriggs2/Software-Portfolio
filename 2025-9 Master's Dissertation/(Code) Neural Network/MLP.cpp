#include "pch.h"
#include "MLP.h"
#include "Layer.h"
#include "definitions.h"

using namespace helpers;

MLP::MLP() : NeuralNetwork() {
	for (auto i = 0; i < HIDDEN_LAYER_COUNT; i++) {
		_network.push_back(Layer(normal));
	}
	_network.push_back(Layer(output));
}

void MLP::connectLayers() {
	Layer::fullyConnect(_inputs, _network[0]);
	for (auto i = 0; i < _network.size() - 1; i++) {
		Layer::fullyConnect(_network[i], _network[i + 1]);
	}
	_outputs = _network.back().getOutputs();
}

void MLP::calculate() {
	for (auto i = 0; i < _network.size(); i++) {
		_network[i].calculate();
	}
}
