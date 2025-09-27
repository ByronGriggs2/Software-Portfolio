#pragma once
#include <vector>
#include "Layer.h"
#include <memory>

class NeuralNetwork
{
protected:


	// For a usable network, inheritors of NeuralNetwork should have an _outputs member variable. It has been omitted from NeuralNetwork
	// becuase assigning the pointers is a responsibility of the inheritor


public:
	
	NeuralNetwork() = delete;
	// weights and biases are initialized as 0
	NeuralNetwork(int inputCount, int hiddenLayerCount);
	NeuralNetwork(const NeuralNetwork& other);
	NeuralNetwork(NeuralNetwork&& other) noexcept;
	NeuralNetwork& operator=(const NeuralNetwork& other);
	NeuralNetwork& operator=(NeuralNetwork&& other) noexcept;

	// The copy constructor and assignment operator of NeuralNetwork do not perform deep copies- they either initialize references as vectors of nullptr or empty vectors of type pointer.
	// connectLayers should be called by inheritors of NeuralNetwork inside their copy constructor and assignment operator to set up the references, which depends on architecture.
	virtual void connectLayers() = 0;
	// Once the references have been set up by connectLayers, the weights will need to be copied over from the neurons.
	virtual void copyWeights(const NeuralNetwork&) = 0;
	virtual void calculate() = 0;
	
};

