#pragma once
#include "NeuralNetwork.h"
class MLP final : public NeuralNetwork
{
public:
	MLP();
	void connectLayers();
	void calculate();
};

