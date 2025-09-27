#include "pch.h"
#include "DeepQManager.h"
#include "MyExcept.h"
#include "ForwardPropNetwork.h"
#include "helpers.h"
#include <omp.h>

using namespace helpers;
using std::vector, std::cout, std::endl;

namespace {
	// row vs column does not matter
	vector<double> elementwiseMultiply(const vector<double>& left, const vector<double>& right) {
		if (left.size() != right.size())
			MyExcept::error("Hadamard product with unequal dimensions");
		vector<double> temp;
		temp.reserve(left.size());
		for (auto i = 0; i < left.size(); i++) {
			temp.push_back(left[i] * right[i]);
		}
		return temp;
	}
	// vect is a column matrix
	// matr SHOULD be transposed but is NOT due to CPU usage. To compensate, I just swapped matr[row][col] with matr[col][row] and matr.size() with matr[0].size()
	vector<double> transposeMatrixByVectorMultiply(const vector<vector<double>>& matr, const vector<double>& vect) {
		if (matr.size() != vect.size())
			MyExcept::error("matrix X Vector with nonmatching size");
		vector<double> temp;
		temp.reserve(matr[0].size());
		for (auto matrRowIndex = 0; matrRowIndex < matr[0].size(); matrRowIndex++) {
			double sum = 0;
			for (auto matrColIndex = 0; matrColIndex < matr.size(); matrColIndex++) {
				sum += matr[matrColIndex][matrRowIndex] * vect[matrColIndex];
			}
			temp.push_back(sum);
		}
		return temp;
	}
	// left is a row matrix, right is a column matrix. Result is a matrix of size left.size() rows x right.size() columns
	vector<vector<double>> oneDimensionMatrixMultiply(const vector<double>& left, const vector<double>& right) {
		vector<vector<double>> temp;
		temp.reserve(left.size());
		for (auto i = 0; i < left.size(); i++) {
			vector<double> tempRow(right.size(), 0.0);
			for (auto j = 0; j < right.size(); j++) {
				tempRow[j] = left[i] * right[j];
			}
			temp.push_back(tempRow);
		}
		return temp;
	}
	void calculateRawBiasGradients(const vector<vector<vector<double>>>& deltaArray, vector<vector<double>>& gradients) {
		if (!gradients.empty()) {
			MyExcept::error("gradients should be empty before calling calculateRawBiasGradient");
		}
		// I could use reserve and resize on gradients here, but it's not a hot loop and I don't want to risk it. I use temp push_back instead (see last block)
		for (auto layer = 0; layer < HIDDEN_LAYER_COUNT + 1; layer++) {
			// sum the errors (deltas) of all trials
			vector<double> sums(deltaArray[0][layer].size(), 0.0);
			for (auto trial = 0; trial < deltaArray.size(); trial++) {
				for (auto neuron = 0; neuron < sums.size(); neuron++) {
					const double val = deltaArray[trial][layer][neuron];
					sums[neuron] += val;
				}
			}
			// Divide by trial count to create average
			vector<double> temp;
			for (auto& val : sums) {
				temp.push_back(val / static_cast<double>(deltaArray.size()));
			}
			gradients.push_back(temp);
		}
	}
	void calculateRawWeightGradients(const vector<vector<vector<double>>>& deltaArray, const vector<vector<vector<double>>>& alphaArray, vector<vector<vector<double>>>& gradients) {
		if (!gradients.empty()) {
			MyExcept::error("gradients should be empty before calling calculateRawWeightGradients");
		}
		// I could use reserve and resize here for gradients, but it's not a hot loop and I don't want to risk it. Instead, I use temp push_back (see last block)
		const int sampleSize = static_cast<int>(deltaArray.size());
		const int layer0Neurons = static_cast<int>(deltaArray[0][0].size());
		const int inputLayerNodes = static_cast<int>(alphaArray[0][0].size());
		// Sum the error-weighted outputs of all trials
		vector<vector<vector<double>>> productVector(sampleSize,
			// # neurons in current layer
			vector<vector<double>>(layer0Neurons,
				// # outputs in layer to the left
				vector<double>(inputLayerNodes, 0.0)
			));
		// For each hidden layer + output layer
		for (auto layer = 0; layer < HIDDEN_LAYER_COUNT + 1; layer++) {
			if (layer != 0) {
				const int layerNeurons = static_cast<int>(deltaArray[0][layer].size());
				const int layerInputs = static_cast<int>(alphaArray[0][layer].size());
				productVector.resize(sampleSize,
					vector<vector<double>>(layerNeurons,
						vector<double>(layerInputs, 0.0)
					)
				);
			}
			
			// Product calculated seperately to allow for clean open MP optimisation
			// The app is fast enough in release without OpenMP, so I disabled it in project settings for now
#pragma omp parallel for
			for (auto trial = 0; trial < sampleSize; trial++) {
				// matrix of errors for each input of each neuron   error for this layer      inputs (outputs of previous layer)
				productVector[trial] = oneDimensionMatrixMultiply(deltaArray[trial][layer], alphaArray[trial][layer]);
			}

			vector<vector<double>> sums(deltaArray[0][layer].size(), vector<double>(alphaArray[0][layer].size(), 0.0));
			for (auto trial = 0; trial < sampleSize; trial++) {
				for (auto neuron = 0; neuron < sums.size(); neuron++) {
					for (auto input = 0; input < sums[neuron].size(); input++) {
						sums[neuron][input] += productVector[trial][neuron][input];
					}
				}
			}
			// Divide by trial count to create average
			vector<vector<double>> finalGradients_layerTemp;
			finalGradients_layerTemp.reserve(sums.size());
			for (auto& neuron : sums) {
				vector<double> finalGradients_neuronTemp;
				finalGradients_neuronTemp.reserve(neuron.size());
				for (auto& val : neuron) {
					finalGradients_neuronTemp.push_back(val / static_cast<double>(sampleSize));
				}
				finalGradients_layerTemp.push_back(finalGradients_neuronTemp);
			}
			gradients.push_back(finalGradients_layerTemp);
		}
	}
	double calculateAverageMagnitude(const vector<vector<double>>& outer) {
		double sum = 0.0;
		int count = 0;
		for (const auto& inner : outer) {
			for (const auto& val : inner) {
				sum += abs(val);
				count++;
			}
		}
		return sum / static_cast<double>(count);
	}
	double calculateAverageMagnitude(const vector<vector<vector<double>>>& outer) {
		double sum = 0.0;
		int count = 0;
		for (const auto& middle : outer) {
			for (const auto& inner : middle) {
				for (const auto& val : inner) {
					sum += abs(val);
					count++;
				}
			}
		}
		return sum / static_cast<double>(count);
	}
}

void DeepQManager::trainingStep(const Transition& lastFrame, float epsilon, bool replayBufferOnly) {
	if (!_networkData.isInitialized())
		_networkData.initialize();
	if (lastFrame._S0.getCurrentLevel() == 0) {
		printSystemTime();
		std::cout << "##WARNING: Invalid transition, training step skipped this frame" << std::endl;
		return;
	}
	if (!lastFrame._valid)
		MyExcept::error("Invalid transition");
	// Add transition to replay buffer
	_replayBuffer.addTransition(lastFrame);
	if (replayBufferOnly) {
		return;
	}

	_networkData.addFrame(calculateLoss(lastFrame), lastFrame._R1, epsilon);
	static int i = 0;
	// Sample replay buffer
	vector<Transition> sample = _replayBuffer.getSample();
	stochasticGradientDescent(sample);
	if (i == TQN_UPDATE_RATIO - 1) {
		i = 0;
		_targetNetwork = *_DQN;
	}
	else
		i++;
}

// In general, this is a bit inefficient for readability. 
void DeepQManager::stochasticGradientDescent(const vector<Transition>& sample) {
	// Find delta and alpha for all elements in sample
	const int sampleSize = static_cast<int>(sample.size());
	vector<vector<vector<double>>> deltaArray;
	vector<vector<vector<double>>> alphaArray;

	for (auto trial = 0; trial < sampleSize; trial++) {
		if (sample[trial]._A0 == cancel)
			MyExcept::error("Neural network cannot cancel");
		if (sample[trial]._A0 == actionCount)
			MyExcept::error("Neural network cannot actionCount");

		vector<vector<double>> miniDeltaArray;
		vector<vector<double>> miniAlphaArray;
		calculateDeltaAndAlphaForTransition(sample[trial], miniDeltaArray, miniAlphaArray);
		deltaArray.push_back(miniDeltaArray);
		alphaArray.push_back(miniAlphaArray);
	}
	////////// BIAS UPDATE
	vector<vector<double>> rawBiasGradients;
 	calculateRawBiasGradients(deltaArray, rawBiasGradients);
	for (auto layer = 0; layer < HIDDEN_LAYER_COUNT + 1; layer++) {
		// Apply learning rate to raw gradients
		for (auto& val : rawBiasGradients[layer]) {
			val *= LEARNING_RATE;
			if (abs(val) > 100.0) {
				MyExcept::error("Abnormally high bias gradient");
			}
		}
		_DQN->addToBiases(layer, rawBiasGradients[layer]);
	}
	/////////// WEIGHT update
	vector<vector<vector<double>>> rawWeightGradients;
	calculateRawWeightGradients(deltaArray, alphaArray, rawWeightGradients);
	for (auto layer = 0; layer < HIDDEN_LAYER_COUNT + 1; layer++) {
		// Apply learning rate to raw gradients
		for (auto& neuron : rawWeightGradients[layer]) {
			for (auto& val : neuron) {
				val *= LEARNING_RATE;
				if (abs(val) > 100.0) {
					MyExcept::error("Abnormally high weight gradient");
				}
			}
		}
		_DQN->addToWeights(layer, rawWeightGradients[layer]);
	}
	_networkData.addToLastFrame(calculateAverageMagnitude(rawBiasGradients), calculateAverageMagnitude(rawWeightGradients));
}

double DeepQManager::calculateLoss(const Transition& lastFrame) {
	const double TNO = _targetNetwork.getHighestOutputAtomic(lastFrame._S1);
	const double y = lastFrame._F ? lastFrame._R1 : lastFrame._R1 + DISCOUNT * TNO;
	const double DQNVal = _DQN->getOutputAtomic(lastFrame._S0, lastFrame._A0);
	const double aVal = y - DQNVal;
	if (abs(aVal) >= 1)
		return abs(aVal) - 0.5;
	else
		return 0.5 * pow(aVal, 2.0);
}

void DeepQManager::calculateDeltaAndAlphaForTransition(const Transition& trans, vector<vector<double>>& miniDeltaArray, vector<vector<double>>& miniAlphaArray) {
	if (!miniDeltaArray.empty())
		MyExcept::error("miniDeltaArray should be empty before calling calculateDeltaForTransition");
	if (!miniAlphaArray.empty())
		MyExcept::error("miniAlphaArray should be empty before calling calculateAlphaForTransition");
	// I can use resize and reserve on miniDeltaArray here, but it's not a hot loop and I don't want to risk it. Instead, here I use push_back for the output layer and emplace for the hidden layers
	// Forward propagation
	const ForwardPropNetwork& forwardProp = _DQN->getForwardPropData(&trans._S0);
	for (const auto& output : forwardProp._alpha.back()) {
		if (abs(output) > 50000) {
			MyExcept::error("Abnormally high output after forward propagation");
		}
	}
	miniAlphaArray = forwardProp._alpha; // may be slow
	const double TNO = _targetNetwork.getHighestOutputAtomic(trans._S1);
	const double y = trans._F ? trans._R1 : trans._R1 + DISCOUNT * TNO;

	// Backpropagation
	vector<double> outputLayerError(forwardProp._alpha.back().size(), 0.0);
	const double DQNVal = _DQN->getOutputAtomic(trans._S0, trans._A0);
#ifdef FIRST_LEVEL_ONLY
	playerActionAlternate newAct = trans._A0 == interact ? interact_a : static_cast<playerActionAlternate>(trans._A0);
	outputLayerError[static_cast<int>(newAct)] = y-DQNVal;
#else
	outputLayerError[static_cast<int>(trans._A0)] = y-DQNVal;
#endif
	miniDeltaArray.push_back(outputLayerError);
	for (const auto& delta : miniDeltaArray[0]) {
		if (abs(delta) > 150000) {
			MyExcept::error("Abnormally high delta after output layer error calculation");
		}
	}
	// Backpropagation is backwards so this loop is reversed
	// The first element is left of the output layer, hence the -1.
	// I H H O
	// n 0 1 2
	for (auto layer = HIDDEN_LAYER_COUNT - 1; layer >= 0; layer--) {
		// f'(zli)
		vector<double> fderiv;
		fderiv.reserve(forwardProp._preactivation[layer].size());
		for (auto neuron = 0; neuron < forwardProp._preactivation[layer].size(); neuron++) {
			int derivValue = forwardProp._preactivation[layer][neuron] > 0 ? 1 : 0;
			fderiv.push_back(derivValue);
		}
		// (Wl+1)T
		const vector<vector<double>> weights = forwardProp._weights[layer + 1];
		// deltal-1
		const vector<double> rightDelta = miniDeltaArray.front();
		// (Wl+1)T*deltal-1
		// weights should be a (# of neurons)x(# of inputs per neuron) matrix, meaning that weightsTranspose is (#of inputs)x(#of neurons)
		// delta should be a (# of neurons)x1 matrix
		const vector<double> term1 = transposeMatrixByVectorMultiply(weights, rightDelta);
		// (fl-1) o [(Wl+1)T*deltal-1]
		vector<double> final = elementwiseMultiply(term1, fderiv);
		// Values are added to the front of the vector so that they are in layer order later on
		miniDeltaArray.emplace(miniDeltaArray.begin(), final);
	}
	// Huber loss
	auto sign = [](double val) {return val >= 0 ? 1.0 : -1.0; };
	for (auto& layer : miniDeltaArray) {
		for (auto& neuronError : layer) {
			neuronError = abs(neuronError) >= 1 ? sign(neuronError) : neuronError;
		}
	}
}

float DeepQManager::findLargestGradient(const Transition& trans) {
	vector<vector<vector<double>>> deltaArray;
	vector<vector<vector<double>>> alphaArray;
	vector<vector<double>> miniDeltaArray;
	vector<vector<double>> miniAlphaArray;
	calculateDeltaAndAlphaForTransition(trans, miniDeltaArray, miniAlphaArray);
	deltaArray.push_back(miniDeltaArray);
	alphaArray.push_back(miniAlphaArray);
	vector<vector<double>> rawBiasGradients;
	calculateRawBiasGradients(deltaArray, rawBiasGradients);
	vector<vector<vector<double>>> rawWeightGradients;
	calculateRawWeightGradients(deltaArray, alphaArray, rawWeightGradients);
	float highest = 0.0f;
	for (const auto& layer : rawBiasGradients) {
		for (const auto& bias : layer) {
			if (abs(bias) > highest)
				highest = static_cast<float>(abs(bias));
		}
	}
	for (const auto& layer : rawWeightGradients) {
		for (const auto& neuron : layer) {
			for (const auto& weight : neuron) {
				if (abs(weight) > highest)
					highest = static_cast<float>(abs(weight));
			}
		}
	}
	return highest;
}