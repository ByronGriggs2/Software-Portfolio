#pragma once
#include "Neuron.h"
#include <vector>

namespace helpers {
	enum neuronType;
}

class Layer final
{
	std::vector<Neuron> _neurons;
	// _outputs will be a vector of nullptrs on construction. It is the responsibility of fullyConnect() to give them meaningful values.
	std::vector<const double*> _outputs;
public:
	//struct ForwardPropLayer {
	//	std::vector<std::vector<double>> _weights;
	//	std::vector<double> _preactivationValues;
	//	std::vector<double> _outputValues;
	//};
	Layer() = delete;
	Layer(helpers::neuronType type);
	Layer(const Layer& other);
	Layer(Layer&& other) noexcept;
	Layer& operator=(const Layer& other);
	Layer& operator=(Layer&& other) noexcept;
	void calculate();
	static void fullyConnect(Layer& lhs, Layer& rhs);
	static void fullyConnect(const std::vector<std::unique_ptr<double>>& inputs, Layer& rhs);
	const std::vector<const double*>& getOutputs() const { return _outputs; }

	void getForwardPropData(std::vector<std::vector<double>>& weights, std::vector<double>& preactivation, std::vector<double>& alpha) const;
	void addToBiases(const std::vector<double>&);
	void addToWeights(const std::vector<std::vector<double>>&);
	std::vector<double> getBiases() const;

	void setWeights(const std::vector<std::vector<double>>& weights);
	void setBiases(const std::vector<double>& biases);
	void randomizeWeightsAndBiases();
	void copyWeightsAndBiases(const Layer& other);
};

