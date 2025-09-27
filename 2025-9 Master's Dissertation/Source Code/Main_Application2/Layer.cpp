#include "pch.h"
#include "Layer.h"
#include "definitions.h"
#include "MyExcept.h"
#include "ForwardPropNetwork.h"

using namespace helpers;
// Should initialize neurons as Layer owns neurons
Layer::Layer(helpers::neuronType type) {
#ifdef FIRST_LEVEL_ONLY
	const int neuronCount = type == output ? actionCount_a : NEURON_PER_LAYER_COUNT;
#else
	const int neuronCount = type == output ? actionCount-1 : NEURON_PER_LAYER_COUNT;
#endif
	for (auto i = 0; i < neuronCount; i++) {
		_neurons.push_back(Neuron(type));
	}
}
Layer::Layer(const Layer& other) : _neurons(other._neurons), _outputs(other._outputs.size(), nullptr) {}
Layer::Layer(Layer&& other) noexcept : _neurons(std::move(other._neurons)), _outputs(std::move(other._outputs)) {}
Layer& Layer::operator=(const Layer& other) {
	if (this == &other)
		return *this;
	if (_neurons.size() != other._neurons.size() || _outputs.size() != other._outputs.size())
		MyExcept::error("Assignment operator on unrelated Layers");
	_neurons = other._neurons;
	_outputs = std::vector<const double*>(_outputs.size(), nullptr);
	return *this;
}
Layer& Layer::operator=(Layer&& other) noexcept {
	if (this == &other)
		return *this;
	if (_neurons.size() != other._neurons.size() || _outputs.size() != other._outputs.size())
		MyExcept::error("Move assignment operator on unrelated Layers");
	_neurons = std::move(other._neurons);
	_outputs = std::move(other._outputs);
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
	rhs._outputs.clear();
	for (auto& neuron : rhs._neurons) {
		rhs._outputs.push_back(neuron.getOutput());
	}
}

void Layer::fullyConnect(const std::vector<std::unique_ptr<double>>& inputs, Layer& rhs) {
	for (auto& neuron : rhs._neurons) {
		neuron.clearInputs();
	}
	for (auto i = 0; i < rhs._neurons.size(); i++) {
		for (auto j = 0; j < inputs.size(); j++) {
			rhs._neurons[i].addInput(inputs[j].get());
		}
	}
	rhs._outputs.clear();
	for (auto& neuron : rhs._neurons) {
		rhs._outputs.push_back(neuron.getOutput());
	}
}

void Layer::getForwardPropData(std::vector<std::vector<double>>& weights, std::vector<double>& preactivation, std::vector<double>& alpha) const {
	for (auto i = 0; i < _neurons.size(); i++) {
		_neurons[i].getForwardPropData(weights[i], preactivation[i], alpha[i]);
	}
}

void Layer::addToBiases(const std::vector<double>& value) {
	if (value.size() != _neurons.size())
		MyExcept::error("vector access out of bounds");
	for (auto i = 0; i < _neurons.size(); i++) {
		_neurons[i].addToBias(value[i]);
	}
}

void Layer::addToWeights(const std::vector<std::vector<double>>& value) {
	if (value.size() != _neurons.size())
		MyExcept::error("vector access out of bounds");
	for (auto i = 0; i < _neurons.size(); i++) {
		_neurons[i].addToWeights(value[i]);
	}
}

std::vector<double> Layer::getBiases() const {
	std::vector<double> temp;
	for (const auto& neuron : _neurons) {
		temp.push_back(neuron.getBias());
	}
	return temp;
}

void Layer::setWeights(const std::vector<std::vector<double>>& weights) {
	if (weights.size() != _neurons.size())
		MyExcept::error("Mismatched neuron count");
	for (auto neuron = 0; neuron < _neurons.size(); neuron++) {
		_neurons[neuron].setWeights(weights[neuron]);
	}
}

void Layer::setBiases(const std::vector<double>& biases) {
	if (biases.size() != _neurons.size())
		MyExcept::error("Mismatched neuron count");
	for (auto neuron = 0; neuron < _neurons.size(); neuron++) {
		_neurons[neuron].setBias(biases[neuron]);
	}
}

void Layer::randomizeWeightsAndBiases() {
	for (auto& neuron : _neurons) {
		neuron.randomizeWeightsAndBias();
	}
}

void Layer::copyWeightsAndBiases(const Layer& other) {
	if (_neurons.size() != other._neurons.size())
		MyExcept::error("Copying between unrelated layers");
	for (auto neuron = 0; neuron < _neurons.size(); neuron++) {
		_neurons[neuron].copyWeightsAndBias(other._neurons[neuron]);
	}
}