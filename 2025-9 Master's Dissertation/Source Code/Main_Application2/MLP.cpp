#include "pch.h"
#include "MLP.h"
#include "definitions.h"
#include "StatePacket.h"
#include "helpers.h"
#include "MyExcept.h"
using namespace helpers;
using std::cout, std::endl, std::lock_guard, std::recursive_mutex;

void MLP::initForwardPropData() {
	_forwardPropData._alpha.resize(_network.size() + 1);
	// alpha[0] is inputs
	_forwardPropData._alpha[0].resize(_inputs.size());
	// alpha.back() is outputs
	_forwardPropData._alpha[_network.size()].resize(_outputs.size());
	// other alpha is hidden layer outputs
	for (auto i = 1; i < _network.size(); i++) {
		_forwardPropData._alpha[i].resize(NEURON_PER_LAYER_COUNT);
	}
	_forwardPropData._preactivation.resize(_network.size());
	// preactivation.back() is preactivation of output neurons
	_forwardPropData._preactivation[_network.size() - 1].resize(_outputs.size());
	// other preactivation is preactivation of hidden layer neurons
	for (auto i = 0; i < _network.size() - 1; i++) {
		_forwardPropData._preactivation[i].resize(NEURON_PER_LAYER_COUNT);
	}
	_forwardPropData._weights.resize(_network.size());
	// weights.back() is vector of weight vectors for output layer
	_forwardPropData._weights[_network.size() - 1].resize(_outputs.size());
	// other weights is vector of weight vectors for hidden layers
	for (auto i = 0; i < _network.size() - 1; i++) {
		_forwardPropData._weights[i].resize(NEURON_PER_LAYER_COUNT);
	}
	// weights[0] is weights from inputs to first layer
	for (auto& weightVector : _forwardPropData._weights[0]) {
		weightVector.resize(_inputs.size());
	}
	// other weights are weights from previous layer to current layer
	for (auto i = 1; i < _network.size(); i++) {
		for (auto& weightVector : _forwardPropData._weights[i]) {
			weightVector.resize(NEURON_PER_LAYER_COUNT);
		}
	}
}

MLP::MLP() {
	_inputs.reserve(NETWORK_INPUT_SIZE);
	for (auto i = 0; i < NETWORK_INPUT_SIZE; i++) {
		_inputs.push_back(std::make_unique<double>(0.0));
	}
	_network.reserve(HIDDEN_LAYER_COUNT + 1);
	for (auto i = 0; i < HIDDEN_LAYER_COUNT; i++) {
		_network.push_back(Layer(normal));
	}
	_network.push_back(Layer(output));
	connectLayers();
	initializeWeightsAndBiases();
	initForwardPropData();
}

MLP::MLP(const MLP& other) : _network(other._network) {
	_inputs.reserve(other._inputs.size());
	for (const auto& input : other._inputs) {
		_inputs.push_back(std::make_unique<double>(*input));
	}
	connectLayers();
	copyWeightsAndBiases(other);
	initForwardPropData();
}

MLP& MLP::operator=(const MLP& other) {
	if (this == &other)
		return *this;
	// Not inherently invalid, but against expected usage
	if (_inputs.size() != other._inputs.size() || _network.size() != other._network.size() || _outputs.size() != other._outputs.size())
		MyExcept::error("assignment operator between unrelated networks");
	_inputs.clear();
	for (int i = 0; i < other._inputs.size(); i++) {
		_inputs.push_back(std::make_unique<double>(*(other._inputs[i])));
	}
	_network = other._network;
	connectLayers();
	copyWeightsAndBiases(other);
	return *this;
}

MLP::MLP(MLP&& other) noexcept : _inputs(std::move(other._inputs)), _network(std::move(other._network)), _outputs(std::move(other._outputs)), _forwardPropData(std::move(other._forwardPropData)) {}

MLP& MLP::operator=(MLP&& other) noexcept {
	if (this == &other)
		return *this;
	if (_inputs.size() != other._inputs.size() || _network.size() != other._network.size())
		MyExcept::error("move assignment operator between unrelated networks");
	_inputs = std::move(other._inputs);
	_network = std::move(other._network);
	_outputs = std::move(other._outputs);
	_forwardPropData = std::move(other._forwardPropData);
	return *this;
}

// Initialises _outputs, must always be called in every constructor
void MLP::connectLayers() {
	Layer::fullyConnect(_inputs, _network[0]);
	for (auto i = 0; i < _network.size() - 1; i++) {
		Layer::fullyConnect(_network[i], _network[i + 1]);
	}
	_outputs = _network.back().getOutputs();
}

// "Randomly" assigns weights and biases. Without this they are initialized as 0 (or copied). Should be called after connectLayers().
void MLP::initializeWeightsAndBiases() {
	for (auto& layer : _network)
		layer.randomizeWeightsAndBiases();
}

// Copies weights and biases. Used in assignment operator and copy constructor. Should be called after connectLayers().
void MLP::copyWeightsAndBiases(const MLP& other) {
	if (_network.size() != other._network.size())
		MyExcept::error("Copying incorrectly sized networks");
	for (auto i = 0; i < _network.size(); i++) {
		_network[i].copyWeightsAndBiases(other._network[i]);
	}
}

void MLP::calculate() {
	for (auto i = 0; i < _network.size(); i++) {
		_network[i].calculate();
	}
}

void MLP::setInputs(const StatePacket& gameState) {
	gameState.getDoubleVector(_inputs);
}

#ifdef FIRST_LEVEL_ONLY
double MLP::getOutputAtomic(const StatePacket& gameState, playerAction act) {
	playerActionAlternate newAct = act == interact ? interact_a : static_cast<playerActionAlternate>(act);
	if (static_cast<int>(newAct) >= static_cast<int>(actionCount_a) || static_cast<int>(newAct) < 0){
		MyExcept::error("Invalid player action");
		return 0.0f;
	}
	lock_guard<recursive_mutex> lock(_mx);
	setInputs(gameState);
	calculate();
	return *_outputs[static_cast<int>(newAct)];
}
#else
float MLP::getOutputAtomic(const StatePacket& gameState, playerAction act) {
	if (act == playerAction::actionCount || act == playerAction::cancel || static_cast<int>(act) < 0)
		MyExcept::error("Invalid player action");
	lock_guard<recursive_mutex> lock(_mx);
	setInputs(gameState);
	calculate();
	return *_outputs[static_cast<int>(act)];
}
#endif

double MLP::getHighestOutputAtomic(const StatePacket& gameState) {
	lock_guard<recursive_mutex> lock(_mx);
	setInputs(gameState);
	calculate();
	double highest = -100000.0;
	// noop is the default if the network thinks it is guaranteed to get a massive negative reward
	int highestAction = 0;
	for (auto i = 0; i < _outputs.size(); i++) {
		if (*_outputs[i] > highest) {
			highest = *_outputs[i];
			highestAction = i;
		}
	}
	if (highestAction < 0 || highestAction > actionCount - 1)
		MyExcept::error("Invalid action selected");
	if (highestAction == actionCount - 1)
		MyExcept::error("Cancel action selected");
	return highest;
}

std::vector<double> MLP::getOutputsAtomic(const StatePacket& gameState) {
	std::vector<double> result;
	result.reserve(_outputs.size());
	lock_guard<recursive_mutex> lock(_mx);
	setInputs(gameState);
	calculate();
	for (auto val : _outputs)
		result.push_back(*val);
	return result;
}

playerAction MLP::getActionAtomic(const StatePacket& gameState, float epsilon) {
	const float roll = (rand() % 10000) / 10000.0f;
	if (roll < epsilon) {
#ifdef FIRST_LEVEL_ONLY
		const int roll2 = rand() % (actionCount_a);
		const playerAction toReturn = roll2 == static_cast<int>(interact_a) ? interact : static_cast<playerAction>(roll2);
		return toReturn;
#else
		const int roll2 = rand() % (actionCount - 1);
		return static_cast<playerAction>(roll2);
#endif
	}
	lock_guard<recursive_mutex> lock(_mx);
	setInputs(gameState);
	calculate();
	float highest = -100000.0f;
	// noop is the default if the network thinks it is guaranteed to get a massive negative reward
	int highestAction = 0;
	for (auto i = 0; i < _outputs.size(); i++) {
		if (*_outputs[i] > highest) {
			highest = *_outputs[i];
			highestAction = i;
		}
	}
	if (highestAction < 0 || highestAction > actionCount-1)
		MyExcept::error("Invalid action selected");
	if (highestAction == actionCount - 1)
		MyExcept::error("Cancel action selected");
#ifdef FIRST_LEVEL_ONLY
	playerAction toReturn = highestAction == static_cast<int>(interact_a) ? interact : static_cast<playerAction>(highestAction);
	return toReturn;
#else
	return static_cast<playerAction>(highestAction);
#endif
}

const ForwardPropNetwork& MLP::getForwardPropData(const StatePacket* inputPacket) {
	lock_guard<recursive_mutex> lock(_mx);
	if (inputPacket != nullptr) {
		setInputs(*inputPacket);
		calculate();
	}
	if (_inputs.size() != _forwardPropData._alpha[0].size())
		MyExcept::error("Input size does not match forward prop data size");
	for (auto i = 0; i < _inputs.size(); i++) {
		_forwardPropData._alpha[0][i] = *_inputs[i];
	}
	for (auto i = 0; i < _network.size(); i++) {
		_network[i].getForwardPropData(_forwardPropData._weights[i], _forwardPropData._preactivation[i], _forwardPropData._alpha[i+1]);
	}
	return _forwardPropData;
}

void MLP::addToBiases(int layer, const std::vector<double>& value) {
	if (layer < 0 || layer > _network.size() - 1)
		MyExcept::error("out of bounds vector access");
	lock_guard<recursive_mutex> lock(_mx);
	_network[layer].addToBiases(value);
}

void MLP::addToWeights(int layer, const std::vector<std::vector<double>>& value) {
	if (layer < 0 || layer > _network.size() - 1)
		MyExcept::error("out of bounds vector access");
	lock_guard<recursive_mutex> lock(_mx);
	_network[layer].addToWeights(value);
}

std::vector<std::vector<double>> MLP::getBiases() {
	std::vector<std::vector<double>> temp;
	lock_guard<recursive_mutex> lock(_mx);
	for (const auto& layer : _network) {
		temp.push_back(layer.getBiases());
	}
	return temp;
}

void MLP::saveWeightsAndBiases(const std::string& filename) {
	std::ofstream ofs;
	ofs.open(helpers::getWritePath("Network_Backup", filename));
	const ForwardPropNetwork& forwardProp = getForwardPropData();
	std::vector<std::vector<std::vector<double>>> weights = forwardProp._weights;
	ofs << "start weights" << endl;
	for (const auto& layer : weights) {
		ofs << "start layer" << endl;
		for (const auto& neuron : layer) {
			ofs << "start neuron" << endl;
			for (const auto& weight : neuron) {
				ofs << std::fixed << std::setprecision(15) << weight << " ";
			}
			ofs << endl << "end neuron" << endl;
		}
		ofs << "end layer" << endl;
	}
	ofs << "end weights" << endl;

	std::vector<std::vector<double>> biases = getBiases();
	ofs << "start biases" << endl;
	for (const auto& layer : biases) {
		ofs << "start layer" << endl;
		for (const auto& neuron : layer) {
			ofs << std::fixed << std::setprecision(15) << neuron << " ";
		}
		ofs << endl << "end layer" << endl;
	}
	ofs << "end biases" << endl;
}



void MLP::loadWeightsAndBiases(const std::string& filename) {
	std::filesystem::path pathToOpen = std::filesystem::current_path() / "Network_Backup" / filename;
	std::ifstream ifs;
	ifs.open(pathToOpen);
	if (!ifs.is_open()) {
		throw std::runtime_error("Failed to open file: " + filename);
	}

	std::vector<std::vector<std::vector<double>>> weights;
	std::vector<std::vector<double>> biases;
	std::string line;
	while (getline(ifs, line)) {
		std::vector<std::vector<double>> layerWeights;
		std::vector<double> neuronWeights;
		std::vector<double> layerBiases;
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
					double value;
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
					double value;
					std::istringstream iss(line);
					while (iss >> value)
						layerBiases.push_back(value);
				}
			}
		}
	}
	lock_guard<recursive_mutex> lock(_mx);
	if (_network.size() != weights.size() || weights.size() != biases.size())
		MyExcept::error("Parsed network is not the same size as current network");
	for (auto layer = 0; layer < _network.size(); layer++) {
		_network[layer].setWeights(weights[layer]);
		_network[layer].setBiases(biases[layer]);
	}
}

// Loads the backup before the last
void MLP::handleErroneousTraining() {
	std::filesystem::path currentPath = std::filesystem::current_path() / "Network_Backup";
	int index = 0;
	while (true) {
		if (!fileExists(currentPath, "backup", ".txt", index))
			break;
		index++;
	}
	if (index == 0)
		return;
	if (index == 1) {
		loadWeightsAndBiases(std::string("backup0.txt"));
		return;
	}
	loadWeightsAndBiases(std::string("backup") + helpers::intToStr(index - 2) + ".txt");
}