#pragma once
#include <vector>
class Layer;

class NeuralNetwork
{
protected:
	std::vector<std::unique_ptr<float>> _inputs;
	std::vector<Layer> _network;
	std::vector<const float*> _outputs;
public:
	virtual void connectLayers() = 0;
	virtual void calculate() = 0;
};

