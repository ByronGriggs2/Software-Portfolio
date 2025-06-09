#include "pch.h"
#include "Neuron.h"
#include "networkHelpers.h"
#include "definitions.h"
#include "exceptions.h"

using namespace networkHelpers;
using namespace helpers;

Neuron::Neuron(neuronType type) :
	_myType(type),
	_bias(getInitializationValue()),
	_myValue(std::make_unique<float>(0.0f)),
	_inputLinks({}) {
}

void Neuron::calculate() {
#ifdef _DEBUG
	if (_inputLinks.empty())
		throw helpers::ShouldNotRunException();
#endif
	_myValue = 0;
	// Weighted sum
	for (auto i = 0; i < _inputLinks.size(); i++) {
		float connectionValue;
		if (_inputLinks[i]._value != nullptr)
			connectionValue = *_inputLinks[i]._value;
		else {
#ifdef _DEBUG
			throw MyExcept("Tried to dereference nullptr");
#endif
			connectionValue = 0.0f;
		}
		*_myValue += connectionValue * _inputLinks[i]._weight;
	}
	// Bias
	*_myValue += _bias;
	// Activation function
	switch (_myType) {
	case normal:
		*_myValue = std::max(0.0f, *_myValue);
		break;
	case output:
		*_myValue = *_myValue;
		break;
#ifdef _DEBUG
	default:
		throw helpers::ShouldNotRunException();
		break;
#endif
	}
}

void Neuron::addInput(const Neuron& in) {
	float temp = getInitializationValue();
	_inputLinks.push_back(Connection(in.getOutput(), temp));
}
void Neuron::addInput(const float* in) {
	float temp = getInitializationValue();
	_inputLinks.push_back(Connection(in, temp));
}