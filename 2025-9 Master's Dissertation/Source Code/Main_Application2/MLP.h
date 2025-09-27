#pragma once
#include "definitions.h"
#include "Layer.h"
#include "ForwardPropNetwork.h"
#include <vector>
namespace helpers {
	enum neuronType;
	enum playerAction;
}

class StatePacket;

class MLP final
{
private:
	std::vector<std::unique_ptr<double>> _inputs;
	std::vector<Layer> _network;
	std::vector<const double*> _outputs;

	// This is the reason getForwardPropData is not thread safe
	ForwardPropNetwork _forwardPropData;
	void initForwardPropData();

	// Construction
	void connectLayers();
	void initializeWeightsAndBiases();
	void copyWeightsAndBiases(const MLP& other); // NOT atomic

	// Atomic calculation
	std::recursive_mutex _mx;
	void setInputs(const StatePacket& state);
	void calculate();

	// Atomic weight and bias getting/setting
	std::vector<std::vector<double>> getBiases();
public:
	void addToBiases(int layer, const std::vector<double>& value);
	void addToWeights(int layer, const std::vector<std::vector<double>>& value);
	void saveWeightsAndBiases(const std::string& filename = "backup");
	void loadWeightsAndBiases(const std::string& filename);

	// Calling this method at the same time as any other method is thread safe, but calling this method at the same time as itself is not.
	const ForwardPropNetwork& getForwardPropData(const StatePacket* inputs = nullptr);

	// Weights and biases are randomly initialized in the default constructor
	MLP();
	MLP(const MLP& other); // NOT atomic
	MLP(MLP&& other) noexcept;
	MLP& operator=(const MLP& other); // NOT atomic
	MLP& operator=(MLP&& other) noexcept;
	~MLP() = default;

	helpers::playerAction getActionAtomic(const StatePacket& inputs, float epsilon);
	std::vector<double> getOutputsAtomic(const StatePacket& inputs);
	double getOutputAtomic(const StatePacket& inputs, helpers::playerAction action);
	double getHighestOutputAtomic(const StatePacket& inputs);

	void handleErroneousTraining();
	bool operator==(MLP& other) {
		return getForwardPropData() == other.getForwardPropData();
	}
};

