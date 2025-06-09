#pragma once
#include <vector>
//#include "definitions.h"
namespace helpers {
	enum neuronType;
}
class Neuron final
{
private:
	struct Connection {
		const float* _value;
		float _weight;
	};
private:
	helpers::neuronType _myType;
	float _bias;
	std::unique_ptr<float> _myValue;
	std::vector<Connection> _inputLinks;
public:
	Neuron() = delete;
	Neuron(helpers::neuronType type);
	void calculate();

	void addInput(const Neuron& in);
	void addInput(const float* in);
	void clearInputs() { _inputLinks.clear(); }
	const float* getOutput() const { return _myValue.get(); }
};
