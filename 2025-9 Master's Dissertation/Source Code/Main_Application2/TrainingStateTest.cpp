#include "pch.h"
#include "TrainingStateTest.h"
#include "definitions.h"
#include "DeepQManager.h"
#include "MLP.h"
#include "TrainingState.h"
#include "CurriculumManager.h"
#include "StatePacket.h"
#include "MyExcept.h"

using namespace helpers;
using std::cout, std::endl;

void TrainingStateTest::copyOfNeuralNetworkTest() { // does not need server
    constexpr int TRAINING_STEPS = 50;

    StatePacket gameState;
    LARGE_INTEGER start, freq;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
    for (int i = 0; i < TRAINING_STEPS + 1; i++) {
        gameState.randomize();
        playerAction chosen = _DQN->getActionAtomic(gameState, _epsilon);
        float reward = chosen == moveWest ? 100.0f : 0.0f;
        Transition thisFrameTransition(gameState, chosen, gameState, reward, true, true);
        _DeepQ->trainingStep(thisFrameTransition, _epsilon);
        if (i % 100 == 0 && i != 0) {
            cout << "Training step " << i << " Completed." << endl;
        }
    }
    cout << endl;
    cout << "Training Complete.";
}

void TrainingStateTest::test() {
    // Fill up replay buffer, DQN, and target network
    copyOfNeuralNetworkTest();
    // Add dummy data to curriculum manager, which also sets epsilon
    _Curriculum = std::make_unique<CurriculumManager>();
    for (auto i = 0; i < 50; i++) {
        Transition temp;
        temp._S0.randomize();
        temp._S1.randomize();
        _Curriculum->update(temp, _epsilon);
    }
    TrainingState::saveTrainingState(_DQN, _DeepQ->getTargetNetwork(), _DeepQ->getReplayBuffer(), _Curriculum.get(), &_epsilon);
    if (!validateTrainingTest()) {
        MyExcept::error("test failed! 1");
    }
    TrainingState::saveTrainingState(_DQN, _DeepQ->getTargetNetwork(), _DeepQ->getReplayBuffer(), _Curriculum.get(), &_epsilon, true);
    if (!validateTrainingTest(true))
        MyExcept::error("test failed! 2");
    cout << "All tests passed" << endl;
}

bool TrainingStateTest::validateTrainingTest(bool error) {
    std::unique_ptr<MLP> loadedDQN = std::make_unique<MLP>();
    DeepQManager tempDeepQ(loadedDQN.get());
    MLP* loadedTargetNetwork = tempDeepQ.getTargetNetwork();
    ReplayBuffer* loadedReplayBuffer = tempDeepQ.getReplayBuffer();
    std::unique_ptr<CurriculumManager> loadedCurriculum = std::make_unique<CurriculumManager>();
    float loadedEpsilon = -9999999.0f;

    TrainingState::loadTrainingState(loadedDQN.get(), loadedTargetNetwork, loadedReplayBuffer, loadedCurriculum.get(), &loadedEpsilon, error);
    // Feed the same input to all 4 networks
    {
        StatePacket temp;
        temp.randomize();
        _DQN->getActionAtomic(temp, 0.0f);
        loadedDQN->getActionAtomic(temp, 0.0f);
        _DeepQ->getTargetNetwork()->getActionAtomic(temp, 0.0f);
        loadedTargetNetwork->getActionAtomic(temp, 0.0f);
    }

    return
        (*_DQN == *loadedDQN) &&
        (*(_DeepQ->getTargetNetwork()) == *loadedTargetNetwork) &&
        (*(_DeepQ->getReplayBuffer()) == *loadedReplayBuffer) &&
        (*_Curriculum == *loadedCurriculum) &&
        (_epsilon == loadedEpsilon);
}