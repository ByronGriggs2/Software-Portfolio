#pragma once
#include <vector>
namespace helpers {
	enum neuronType;
}
class Neuron final
{
	helpers::neuronType _myType;
	std::unique_ptr<double> _myValue;
	double _preactivation;
	// weights and bias are initialized at 0. Use randomizeWeightsAndBiases after construction to properly initialize them.
	double _bias;
	// This is an empty vector or vector of nullptrs after construction. To give meaningful values use addInput()
	std::vector<const double*> _inputLinksValues;
	std::vector<double> _inputLinksWeights;
public:
	Neuron() = delete;
	Neuron(helpers::neuronType type);
	Neuron(const Neuron& other);
	Neuron(Neuron&& other) noexcept;
	Neuron& operator=(const Neuron& other);
	Neuron& operator=(Neuron&& other) noexcept;
	void calculate();

	void addInput(const Neuron& in);
	void addInput(const double* in);
	void clearInputs() { _inputLinksValues.clear(); _inputLinksWeights.clear(); }
	const double* getOutput() const { return _myValue.get(); }
	std::vector<double> getWeights() const;
	double getPreactivation() const { return _preactivation; }
	void addToBias(double);
	void addToWeights(const std::vector<double>&);
	double getBias() const { return _bias; }
	void randomizeWeightsAndBias();
	
	void setWeights(const std::vector<double>& weights);
	void setBias(double bias) { _bias = bias; }
	void copyWeightsAndBias(const Neuron& other);

	void getForwardPropData(std::vector<double>& weights, double& preactivation, double& alpha) const;
};
