#include "pch.h"
#include "Neuron.h"
#include "definitions.h"
#include "MyExcept.h"
#include <random>

using namespace helpers;

Neuron::Neuron(neuronType type) :
	_myType(type),
	_myValue(std::make_unique<double>(0.0)),
	_preactivation(0.0),
	_bias(0.0),
	_inputLinksValues(std::vector<const double*>()),
	_inputLinksWeights(std::vector<double>())
{}
Neuron::Neuron(const Neuron& other) :
	_myType(other._myType),
	_myValue(std::make_unique<double>(0.0)),
	_preactivation(other._preactivation),
	_bias(other._bias),
	_inputLinksValues(std::vector<const double*>(other._inputLinksValues.size(), nullptr)),
	_inputLinksWeights(other._inputLinksWeights)
	{}
Neuron::Neuron(Neuron&& other) noexcept : _myType(std::move(other._myType)), _myValue(std::move(other._myValue)), _preactivation(other._preactivation), _bias(std::move(other._bias)), _inputLinksValues(std::move(other._inputLinksValues)), _inputLinksWeights(std::move(other._inputLinksWeights)) {}
Neuron& Neuron::operator=(const Neuron& other) {
	if (this == &other)
		return *this;
	if (_myType != other._myType || _inputLinksValues.size() != other._inputLinksValues.size())
		MyExcept::error("Assignment operator between unrelated Neurons");
	_myValue = std::make_unique<double>(0.0);
	_preactivation = other._preactivation;
	_bias = other._bias;
	_inputLinksValues = std::vector<const double*>(other._inputLinksValues.size(), nullptr);
	_inputLinksWeights = other._inputLinksWeights;
	return *this;
}
Neuron& Neuron::operator=(Neuron&& other) noexcept {
	if (this == &other)
		return *this;
	if (_myType != other._myType || _inputLinksValues.size() != other._inputLinksValues.size())
		MyExcept::error("Move assignment operator between unrelated Neurons");
	_myType = std::move(other._myType);
	_myValue = std::move(other._myValue);
	_preactivation = std::move(other._preactivation);
	_bias = std::move(other._bias);
	_inputLinksValues = std::move(other._inputLinksValues);
	_inputLinksWeights = std::move(other._inputLinksWeights);
	return *this;
}

void Neuron::calculate() {
	// takes up not insignificant CPU time
#ifdef _DEBUG
	if (_inputLinksValues.empty())
		MyExcept::error("Neuron has no inputs");
#endif
	*_myValue = 0;
	// Weighted sum
	// Note that this alternates accessing from two different vectors. Could be improved for efficiency?
	for (auto i = 0; i < _inputLinksValues.size(); i++) {
		double connectionValue;
#ifdef _DEBUG
		if (_inputLinksValues[i] != nullptr)
			connectionValue = *_inputLinksValues[i];
		else {
			MyExcept::error("Neuron input has no value ptr");
			connectionValue = 0.0f;
		}
		*_myValue += connectionValue * _inputLinksWeights[i];
#else
		connectionValue = *_inputLinksValues[i];
		*_myValue += connectionValue * _inputLinksWeights[i];
#endif
	}
	// Bias
	*_myValue += _bias;
	_preactivation = *_myValue;
	// Activation function
	switch (_myType) {
	case normal:
		*_myValue = 0.0 > *_myValue ? 0.0 : *_myValue;
		break;
	case output:
		*_myValue = *_myValue;
		break;
	default:
		MyExcept::error("Invalid neuron type");
		break;
	}
}

void Neuron::addInput(const Neuron& in) {
	_inputLinksValues.push_back(in.getOutput());
	_inputLinksWeights.push_back(0.0);
}
void Neuron::addInput(const double* in) {
	_inputLinksValues.push_back(in);
	_inputLinksWeights.push_back(0.0);
}

std::vector<double> Neuron::getWeights() const {
	std::vector<double> temp;
	temp.reserve(_inputLinksWeights.size());
	for (const auto& val : _inputLinksWeights) {
		temp.push_back(static_cast<double>(val));
	}
	return temp;
}

void Neuron::addToBias(double value) {
	_bias += value;
}

void Neuron::addToWeights(const std::vector<double>& value) {
	if (value.size() != _inputLinksWeights.size())
		MyExcept::error("vector access out of bounds");
	for (auto i = 0; i < _inputLinksWeights.size(); i++) {
		_inputLinksWeights[i] += static_cast<double>(value[i]);
	}
}

void Neuron::randomizeWeightsAndBias() {
	const float sigma = std::sqrt(2.0f / _inputLinksWeights.size());
	std::normal_distribution<float> dist(0.0f, sigma);
	std::mt19937 rng(std::random_device{}());
	for (auto& link : _inputLinksWeights) {
		link = static_cast<double>(dist(rng));
	}
	_bias = 0.01f;
}

void Neuron::setWeights(const std::vector<double>& weights) {
	if (weights.size() != _inputLinksWeights.size())
		MyExcept::error("Mismatched input count");
	_inputLinksWeights = weights;
}

void Neuron::copyWeightsAndBias(const Neuron& other) {
	if (_inputLinksValues.size() != other._inputLinksValues.size())
		MyExcept::error("Copy between unrelated Neurons");
	_bias = other._bias;
	for (auto i = 0; i < _inputLinksWeights.size(); i++) {
		_inputLinksWeights[i] = other._inputLinksWeights[i];
	}
}

void Neuron::getForwardPropData(std::vector<double>& weights, double& preactivation, double& alpha) const {
	if (weights.size() != _inputLinksWeights.size())
		MyExcept::error("Incorrect foreward prop");
	for (auto i = 0; i < _inputLinksWeights.size(); i++) {
		weights[i] = _inputLinksWeights[i];
	}
	preactivation = _preactivation;
	alpha = *_myValue;
}