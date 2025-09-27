#include "pch.h"
#include "NeuralNetwork.h"
#include "definitions.h"

using std::cout, std::endl;
using namespace helpers;

NeuralNetwork::NeuralNetwork(int inputCount, int hiddenLayerCount){
	
}
NeuralNetwork::NeuralNetwork(const NeuralNetwork& other) : _network(other._network) {
	const int inputCount = other._inputs.size();
	_inputs.reserve(inputCount);
	for (int i = 0; i < inputCount; ++i) {
		_inputs.push_back(std::make_unique<float>(*(other._inputs[i])));
	}
}
NeuralNetwork::NeuralNetwork(NeuralNetwork&& other) noexcept : _inputs(std::move(other._inputs)), _network(std::move(other._network)) {}
NeuralNetwork& NeuralNetwork::operator=(const NeuralNetwork& other) {
	if (this == &other)
		return *this;
	if (_inputs.size() != other._inputs.size() || _network.size() != other._network.size())
		assert(false && "assignment operator between unrelated networks");
	_inputs.clear();
	for (int i = 0; i < other._inputs.size(); i++) {
		_inputs.push_back(std::make_unique<float>(*(other._inputs[i])));
	}
	_network = other._network;
	return *this;
}
NeuralNetwork& NeuralNetwork::operator=(NeuralNetwork&& other) noexcept {
	if (this == &other)
		return *this;
	if (_inputs.size() != other._inputs.size() || _network.size() != other._network.size())
		assert(false && "move assignment operator between unrelated networks");
	_inputs = std::move(other._inputs);
	_network = std::move(other._network);
	return *this;
}

NeuralNetwork::ForwardPropNetwork NeuralNetwork::getForwardPropData() const {
	ForwardPropNetwork data;
	// Add input layer
	std::vector<float> inputs;
	for (auto& input : _inputs) {
		inputs.push_back(*input);
	}
	data._alpha.push_back(inputs);

	for (auto& layer : _network) {
		Layer::ForwardPropLayer layerData = layer.getForwardPropData();
		data._alpha.push_back(layerData._outputValues);
		data._preactivation.push_back(layerData._preactivationValues);
		data._weights.push_back(layerData._weights);
	}
	return data;
}

void NeuralNetwork::subtractFromBiases(int layer, const std::vector<float>& value) {
	if (layer < 0 || layer > _network.size() - 1)
		assert(false && "out of bounds vector access");
	_network[layer].subtractFromBiases(value);
}

void NeuralNetwork::subtractFromWeights(int layer, const std::vector<std::vector<float>>& value) {
	if (layer < 0 || layer > _network.size() - 1)
		assert(false && "out of bounds vector access");
	_network[layer].subtractFromWeights(value);
}

std::vector<std::vector<float>> NeuralNetwork::getBiases() const {
	std::vector<std::vector<float>> temp;
	for (const auto& layer : _network) {
		temp.push_back(layer.getBiases());
	}
	return temp;
}

void NeuralNetwork::saveWeightsAndBiases() const {
	auto intToStr = [](int in) {
		if (in < 0)
			assert(false && "Negative unsupported");
		if (in == 0)
			return std::string("0");
		std::string str;
		while (in != 0) {
			str.insert(str.begin(), static_cast<char>('0' + in % 10));
			in -= in % 10;
			in /= 10;
		}
		return str;
		};
	std::ofstream ofs;
	std::filesystem::path currentPath = std::filesystem::current_path();
	currentPath = currentPath / "Network_Backup";
	if (!std::filesystem::exists(currentPath))
		std::filesystem::create_directory(currentPath);
	{
		int index = 0;
		std::string myStr;
		std::filesystem::path tempPath;
		while (true) {
			myStr = "backup" + intToStr(index) + std::string(".txt");
			tempPath = currentPath / myStr.c_str();
			if (!std::filesystem::exists(tempPath))
				break;
			index++;
		}
		ofs.open(tempPath);
	}
	ForwardPropNetwork forwardProp = getForwardPropData();
	std::vector<std::vector<std::vector<float>>> weights = forwardProp._weights;
	ofs << "start weights" << endl;
	for (const auto& layer : weights) {
		ofs << "start layer" << endl;
		for (const auto& neuron : layer) {
			ofs << "start neuron" << endl;
			for (const auto& weight : neuron) {
				ofs << weight << " ";
			}
			ofs << endl << "end neuron" << endl;
		}
		ofs << "end layer" << endl;
	}
	ofs << "end weights" << endl;

	std::vector<std::vector<float>> biases = getBiases();
	ofs << "start biases" << endl;
	for (const auto& layer : biases) {
		ofs << "start layer" << endl;
		for (const auto& neuron : layer) {
			ofs << neuron << " ";
		}
		ofs << endl << "end layer" << endl;
	}
	ofs << "end biases" << endl;
}



void NeuralNetwork::loadWeightsAndBiases(const std::string& filename) {
	std::filesystem::path pathToOpen = std::filesystem::current_path() / "Network_Backup" / filename;
	std::ifstream ifs;
	ifs.open(pathToOpen);
	if (!ifs.is_open()) {
		cout << "########ERROR cannot open file " << filename << endl;
		while (true);
	}

	std::vector<std::vector<std::vector<float>>> weights;
	std::vector<std::vector<float>> biases;
	std::string line;
	while (getline(ifs, line)) {
		std::vector<std::vector<float>> layerWeights;
		std::vector<float> neuronWeights;
		std::vector<float> layerBiases;
		if (line.find("start weights") != std::string::npos) {
			while (getline(ifs, line)) {
				if (line.find("end weights") != std::string::npos)
					break;
				else if (line.find("start layer") != std::string::npos)
					layerWeights.clear();
				else if (line.find("end layer") != std::string::npos)
					weights.push_back(layerWeights);
				else if (line.find("start neuron") != std::string::npos)
					neuronWeights.clear();
				else if (line.find("end neuron") != std::string::npos)
					layerWeights.push_back(neuronWeights);
				else {
					float value;
					std::istringstream iss(line);
					while (iss >> value)
						neuronWeights.push_back(value);
				}
			}
		}
		else if (line.find("start biases") != std::string::npos) {
			while (getline(ifs, line)) {
				if (line.find("end biases") != std::string::npos)
					break;
				else if (line.find("start layer") != std::string::npos)
					layerBiases.clear();
				else if (line.find("end layer") != std::string::npos)
					biases.push_back(layerBiases);
				else {
					float value;
					std::istringstream iss(line);
					while (iss >> value)
						layerBiases.push_back(value);
				}
			}
		}
	}
	if (_network.size() != weights.size() || weights.size() != biases.size())
		assert(false && "Parsed network is not the same size as current network");
	for (auto layer = 0; layer < _network.size(); layer++) {
		_network[layer].setWeights(weights[layer]);
		_network[layer].setBiases(biases[layer]);
	}
}