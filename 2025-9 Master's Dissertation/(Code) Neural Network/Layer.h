#pragma once
#include <vector>
#include "Neuron.h"

namespace helpers {
	enum neuronType;
}

class Layer final
{
	std::vector<Neuron> _neurons;
	std::vector<const float*> _outputs;
public:
	Layer() = delete;
	Layer(helpers::neuronType type);
	void calculate();
	static void fullyConnect(Layer& lhs, Layer& rhs);
	static void fullyConnect(const std::vector<std::unique_ptr<float>>& inputs, Layer& rhs);
	const std::vector<const float*>& getOutputs() const { return _outputs; }
};

