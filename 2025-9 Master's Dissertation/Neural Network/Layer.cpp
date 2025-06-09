#include "pch.h"
#include "Layer.h"
#include "definitions.h"
#include "Neuron.h"

using namespace helpers;
// Should initialize neurons as Layer owns neurons
Layer::Layer(helpers::neuronType type) {
	for (auto i = 0; i < NEURON_PER_LAYER_COUNT; i++) {
		_neurons.push_back(Neuron(type));
		_outputs.push_back(_neurons.back().getOutput());
	}
}

void Layer::calculate() {
	for (auto& neuron : _neurons) {
		neuron.calculate();
	}
}

void Layer::fullyConnect(Layer& lhs, Layer& rhs) {
	for (auto& neuron : rhs._neurons) {
		neuron.clearInputs();
	}
	for (auto i = 0; i < rhs._neurons.size(); i++) {
		for (auto j = 0; j < lhs._neurons.size(); j++) {
			rhs._neurons[i].addInput(lhs._neurons[j]);
		}
	}
}

void Layer::fullyConnect(const std::vector<std::unique_ptr<float>>& inputs, Layer& rhs) {
	for (auto& neuron : rhs._neurons) {
		neuron.clearInputs();
	}
	for (auto i = 0; i < rhs._neurons.size(); i++) {
		for (auto j = 0; j < inputs.size(); j++) {
			rhs._neurons[i].addInput(inputs[j].get());
		}
	}
}