#include "pch.h"
#include "Control.h"
#include "definitions.h"
#include "DeepQManager.h"
#include "InOutTest.h"
#include "actionMacros.h"
#include "fstream"
#include "helpers.h"
#include "RewardManager.h"
#include "TrainingState.h"
#include "AutoRecoveryTest.h"
#include "TrainingStateTest.h"
#include "MyExcept.h"
#include "MaximumRewardTest.h"
// To increase Sleep resolution
#pragma comment(lib, "winmm.lib")
#include <mmsystem.h>

using namespace helpers;
using std::cout, std::endl;

Control::Control() :
    _server(),
    _DQN(std::make_unique<MLP>()),
    _DeepQ(nullptr)
{
    // Increase resolution of Sleep() to 1 ms
    timeBeginPeriod(1);
}
Control::~Control() {
    timeEndPeriod(1);
}
void Control::initializeControl(bool usingServer, bool usingDirectResponse, bool usingWatchDog, bool usingDeepQ) {
    // setup DeepQ
    if (usingDeepQ) {
        _DeepQ = std::make_unique<DeepQManager>(_DQN.get());
    }
    // setup server
    if (usingServer) {
        if (_server.initialize() == -1)
            MyExcept::getInstance().error("Failed to initialize server");
        _server.resetRecvCounter();
        _server.setEpsilon(_epsilon);
    }
    // setup server direct response
    if (usingDirectResponse && usingWatchDog) {
        _server.setupDirectResponse(_DQN.get(), _DeepQ->getReplayBuffer());
    }
    else if (usingDirectResponse && !usingWatchDog) {
        _server.setupDirectResponse(_DQN.get(), nullptr);
    }
    // setup MyExcept auto save
    if (usingDeepQ && usingServer) {
        MyExcept::setupAutoSave(_DQN.get(), _DeepQ->getTargetNetwork(), _DeepQ->getReplayBuffer(), &_curriculumManager, &_epsilon, &_server);
    }
    else if (usingDeepQ && !usingServer) {
        MyExcept::setupAutoSave(_DQN.get(), _DeepQ->getTargetNetwork(), _DeepQ->getReplayBuffer(), &_curriculumManager, &_epsilon, nullptr);
    }
    // navigate to level select
    if (usingServer)
        _server.defaultStartup();
}
//////////////////////////// Training
void Control::fpsCheck(LARGE_INTEGER& start, const LARGE_INTEGER& freq, bool& ignoreOnce) {
    int frames = _server.getRecvCounter();
    if (frames % 300 == 299) {
        if (ignoreOnce) {
            ignoreOnce = false;
		}
        else {
            const float time = getTime(start, freq);
            const float fps = _server.getRecvCounter() / time;
            if (fps < 45 || fps > 70) {
                std::string str = "Average fps over the last 300 frames was ";
                str = str + std::to_string(fps);
                MyExcept::getInstance().error(str.c_str());
            }
        }
        _server.resetRecvCounter();
		QueryPerformanceCounter(&start);
    }
}
Transition Control::getNextTrainingTransition(int episodes) {
    Transition chosenTransition;
    // Check if player has died or won this frame
    {
        Transition checkAhead = _server.getNewestTransition(false);
        bool death = checkAhead._S1.getPlayerDeath();
        bool win = checkAhead.playerLeftLevel();
        if (death) {
            handlePlayerDeath(checkAhead._S0.getCurrentLevel());
        }
        else if (win) {
            handlePlayerWin(checkAhead._S0.getCurrentLevel());
        }
        if (death || win) {
            // Transition does not need to be removed from buffer, as it is automatically removed when handlePlayerDeath or handlePlayerWin are called
            chosenTransition = checkAhead;
            chosenTransition._F = true;
            // Periodically save neural network in case of failure
            if ((episodes+1) % EPISODE_BATCH == 0)
                TrainingState::saveTrainingState(_DQN.get(), _DeepQ->getTargetNetwork(), _DeepQ->getReplayBuffer(), &_curriculumManager, &_epsilon);
        }
        // Normal frame
        else {
            chosenTransition = _server.getOldestTransition(true);
            chosenTransition._F = false;
        }
    }
    if (chosenTransition._valid)
        MyExcept::error("Transition should not be valid yet");
    chosenTransition._valid = true;
    chosenTransition._R1 = RewardManager::calculateReward(chosenTransition);
    return chosenTransition;
}
void Control::handlePlayerDeath(int currentLevel) {
    _server.deactivateDirectResponse();
    _server.sendAction(noop);
    // Wait for death animation
    switch (currentLevel) {
    case 10:
    case 11:
    case 12:
        Sleep(YAGA_DEATH_DURATION_MS);
        break;
    case 13:
        Sleep(SUPER_DEATH_DURATION_MS);
        break;
    default:
        Sleep(ULA_DEATH_DURATION_MS);
        break;
    }
    _server.activateDirectResponse();
}
void Control::handlePlayerWin(int currentLevel) {
    // This probably doesn't work in the main branch but here I just need to re-enter the same level so it probably works without testing
    _server.deactivateDirectResponse();
    StatePacket gamestate;
    _server.getGameState(gamestate);
    if (gamestate.getCurrentLevel() != 0) {
        cout << "## WARNING: not in menu after win" << endl;
        actionMacros::leaveLevelRobust(&_server);
    }
    Sleep(3000);
    _server.sendAction(moveWest);
    Sleep(3 * ONEFRAMEMS);
    _server.sendAction(noop);
    Sleep(3 * ONEFRAMEMS);
    actionMacros::enterLevel(&_server);
    _server.activateDirectResponse();
}
void Control::trainingFunct(int startingLevel, bool ignoreFirstFPSCheck) {
    cout << "Training function started at ";
    helpers::printSystemTime();
    cout << endl;

	int currentlyTrainingLevel = startingLevel;
    LARGE_INTEGER start, freq;
    QueryPerformanceFrequency(&freq);
    _server.activateDirectResponse();
    QueryPerformanceCounter(&start);
	_server.resetRecvCounter();
    // 1 loop per episode
    while (_curriculumManager.getEpisodes() < TARGET_EPISODES) {
		int episodes = _curriculumManager.getEpisodes();
        Transition chosenTransition = getNextTrainingTransition(episodes);
        // Run a training step. Network statistics are updated here.
        if (TARGET_EPISODES > 60 && episodes < NO_TRAINING_DURATION) {
            _DeepQ->trainingStep(chosenTransition, _epsilon, true);
        }
        else
            _DeepQ->trainingStep(chosenTransition, _epsilon);
        // Update the curriculum manager, which chooses the next level and epsilon
        _curriculumManager.update(chosenTransition, _epsilon);
        // Switch to a different level for the next episode, if needed
        if (_curriculumManager.getNextLevel() != currentlyTrainingLevel && episodes < TARGET_EPISODES) {
            _server.deactivateDirectResponse();
            actionMacros::leaveLevelRobust(&_server);
            currentlyTrainingLevel = _curriculumManager.getNextLevel();
            actionMacros::enterSpecificLevel(&_server, currentlyTrainingLevel);
            _server.activateDirectResponse();
        }
        _server.setEpsilon(_epsilon);
        //fpsCheck(start, freq, ignoreFirstFPSCheck);
    }
    _server.deactivateDirectResponse();
    cout << "Training finished after " << _curriculumManager.getEpisodes() << " episodes." << endl;
    Sleep(1000);
    actionMacros::leaveLevelRobust(&_server);
    actionMacros::exitGame(&_server);
    _server.stopComms(true);
    TrainingState::saveTrainingState(_DQN.get(), _DeepQ->getTargetNetwork(), _DeepQ->getReplayBuffer(), &_curriculumManager, &_epsilon);
    return;
}
/////////////////////////////// Start training with a randomized network
void Control::startTraining() {
    initializeControl(true, true, true, true);
    actionMacros::enterSpecificLevel(&_server, 1);
    trainingFunct(1);
}
///////////////////////////////// Resume training from an error state or saved state
void Control::resumeTraining(bool error, bool resetEpisodes) {
    printSystemTime();
    cout << "Resuming training from saved state" << endl;
    initializeControl(true, true, true, true);
	TrainingState::loadTrainingState(_DQN.get(), _DeepQ->getTargetNetwork(), _DeepQ->getReplayBuffer(), &_curriculumManager, &_epsilon, error);
    if (resetEpisodes)
		_curriculumManager.resetEpisodes();
	const int level = _curriculumManager.getNextLevel();
    actionMacros::enterSpecificLevel(&_server, level);
    trainingFunct(level, true);
}
//////////////////////////// Load and evaluate network
void Control::loadAndEvaluateNetwork(const std::string& filename) {
    _epsilon = 0.05f;
    initializeControl(true, true, false, false);
    _DQN->loadWeightsAndBiases(filename);
    actionMacros::enterSpecificLevel(&_server, 1);

    cout << "Testing network winrate" << endl;
    _server.activateDirectResponse();
    float wins = 0;
    float losses = 0;
    while (wins + losses < TARGET_EPISODES) {
        Transition thisPreventsAnAnnoyingDebugTextMessage = _server.getOldestTransition(true);
        if (_server.getPlayerDeathFlag(true)) {
            losses += 1;
            continue;
        }
        else if (_server.getPlayerWinFlag(true)) {
            _server.deactivateDirectResponse();
            Sleep(3000);
            _server.sendAction(moveWest);
            Sleep(3 * ONEFRAMEMS);
            _server.sendAction(noop);
            Sleep(3 * ONEFRAMEMS);
            actionMacros::enterLevel(&_server);
            _server.activateDirectResponse();
        }
    }
    cout << "Testing complete: network winrate = " << (wins / (wins + losses)) / 100.0f << "%" << endl;
}

///////////////////////////// Tests
// Trains the network to always move west given randomized game states
void Control::neuralNetworkTest() { // does not need server
    constexpr int TRAINING_STEPS = 1000;
    _DeepQ = std::make_unique<DeepQManager>(_DQN.get());

    StatePacket gameState;
    LARGE_INTEGER start, freq;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
    for (int i = 0; i < TRAINING_STEPS+1; i++) {
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
    cout << "Training Complete. Step frequency: " << static_cast<float>(TRAINING_STEPS) / getTime(start, freq) << endl;
    auto testDQN = [this, &gameState]() {
        cout << "Testing network..." << endl;
        float success = 0;
        float failure = 0;
        for (auto i = 0; i < 1000; i++) {
            gameState.randomize();
            const playerAction chosen = _DQN->getActionAtomic(gameState, 0.0f);
            if (chosen == moveWest)
                success++;
            else
                failure++;
        }
        cout << "Network correct guess rate: " << 100.0f * (success / (success + failure)) << "%" << endl;
        };
    testDQN();
    cout << endl;

    cout << "Saving network to disk and deleting application copy..." << endl;
    _DQN->saveWeightsAndBiases();
    _DQN.release();
    _DeepQ.release();
    cout << "Reloading saved network weights..." << endl;
    _DQN = std::make_unique<MLP>();
    _DQN->loadWeightsAndBiases("backup0.txt");
    testDQN();
}

void Control::cheatCodeTest() {
    auto enterCredits = [this]() {
        Sleep(3 * ONEFRAMEMS);
        _server.sendAction(cancel);
        Sleep(3 * ONEFRAMEMS);
        _server.sendAction(moveNorth);
        Sleep(3 * ONEFRAMEMS);
        _server.sendAction(noop);
        Sleep(3 * ONEFRAMEMS);
        _server.sendAction(moveNorth);
        Sleep(3 * ONEFRAMEMS);
        _server.sendAction(interact);
        Sleep(10 * ONEFRAMEMS);
		};
    std::vector<playerAction> cheatCode({
    moveSouth,
    noop,
    moveNorth,
    noop,
    moveEast,
    noop,
    moveNorth,
    noop,
    moveWest,
    noop,
    moveSouth,
    noop,
    moveSouth,
    noop,
    moveWest,
    noop,
    moveNorth,
    noop,
    moveSouth,
    noop,
    moveEast,
    noop,
    moveNorth
        }
    );

    _DeepQ = std::make_unique<DeepQManager>(_DQN.get());
    _server.setupDirectResponse(_DQN.get(),_DeepQ->getReplayBuffer());
    _server.setEpsilon(0.0);
    _server.defaultStartup();

	// Create set of random game states. They will be reused across training steps
    std::vector<StatePacket> cheatStates(cheatCode.size() + 1, StatePacket());
    for (auto& state : cheatStates) {
        state.randomize();
    }
    StatePacket finalState;
    finalState.randomize();

    int cheatCodeIndex = 0;
    int trainingSteps = 0;
	int bestIndex = 0;
    cout << "Training network to input cheat code" << endl;
    for (int i = 0; i < 5; i++) {
        // The network cannot leave this loop until it chooses the correct action 22 times in a row
        while (cheatCodeIndex < cheatCode.size()) {
            Transition trans;
            // given the current position in the cheat code, what does the network want to do,
			// given the corresponding game state?
            trans._S0 = cheatStates[cheatCodeIndex];
            trans._A0 = _DQN->getActionAtomic(trans._S0, _epsilon);
            // Correct action: reward 50
            if (trans._A0 == cheatCode[cheatCodeIndex]) {
                trans._R1 = 50;
                trans._S1 = cheatStates[cheatCodeIndex + 1];
                trans._F = false;
                cheatCodeIndex++;
            }
			// Incorrect action: reward -100, reset cheat code index to 0
            else {
                trans._R1 = -100;
                trans._S1 = cheatStates[0];
                trans._F = true;
                cheatCodeIndex = 0;
            }
            trans._valid = true;
            _DeepQ->trainingStep(trans, _epsilon);
            trainingSteps++;
            // Set epsilon such that an already perfect network has a 50% chance of getting a new best index next loop
            _epsilon = 1.0f - static_cast<float>(pow(0.5, 1.0 / static_cast<double>(bestIndex + 1)));
			// For user sanity, allows them to track the training progress
            if (cheatCodeIndex > bestIndex)
                bestIndex++;
            if (trainingSteps % 100 == 0 || i > 0) {
                cout << "Training step " << trainingSteps << " completed. Number of correct sequences = " << i << ". Best index = " << bestIndex << "." << endl;
			}
        }
        // For fully completing the cheat code sequence, reward 100
        Transition endTransition;
        endTransition._S0 = cheatStates.back();
        endTransition._A0 = cheatCode.back();
        endTransition._S1 = finalState;
        endTransition._R1 = 100;
        endTransition._F = true;
        endTransition._valid = true;
        _DeepQ->trainingStep(endTransition, _epsilon);
        cheatCodeIndex = 0;
        bestIndex = 0;
    }
    cout << "Training complete." << endl;
	_DQN->saveWeightsAndBiases("cheatCodeTrained");
    cout << "Testing trained network" << endl;
    int success = 0;
    for (auto i = 0; i < 100; i++) {
        for (auto j = 0; j < cheatCode.size(); j++) {
            playerAction action = _DQN->getActionAtomic(cheatStates[j], 0.0f);
            if (action != cheatCode[j])
                break;
            if (j == cheatCode.size())
                success++;
        }
    }
    cout << "Network chose the correct sequence in " << success << " out of 100 tests" << endl;

    cout << "To test in game, start Awaria now." << endl;
	_server.setupDirectResponse(_DQN.get(), nullptr);
	_server.setEpsilon(0.0f);
	_server.defaultStartup();
    enterCredits();
    for (auto index = 0; index < cheatCode.size(); index++) {
        StatePacket unused;
        while (!_server.getGameState(unused))
            Sleep(0.25 * ONEFRAMEMS);
        playerAction action = _DQN->getActionAtomic(cheatStates[index], 0);
        _server.sendAction(action);
        Sleep(0.5 * ONEFRAMEMS);
    }
    cout << "Cheat code complete" << endl;
}

// Checks that the an action sent on frame N results in a change in game state on frame N+1.
void Control::responsivenessTest(bool fullTest) {
    // Lambda tests responsiveness in server normal mode
    auto testActionIndirect = [this](playerAction action) {
        Sleep(250);
        StatePacket S0;
        while (!_server.getGameState(S0))
            Sleep(0.25 * ONEFRAMEMS);
        _server.sendAction(action);
        StatePacket S1;
        int frameCount = 1;
        while (true) {
            while (!_server.getGameState(S1))
                Sleep(0.25 * ONEFRAMEMS);
            const Position2D oldPos = S0.getPlayerPos();
            const Position2D newPos = S1.getPlayerPos();
            if (sqrt(pow(oldPos._X - newPos._X, 2.0) + pow(oldPos._Y - newPos._Y, 2.0)) >= 0.002f)
                break;
            frameCount++;
        }
        _server.sendAction(noop);
        cout << "Number of frames before response reflected in game state: " << frameCount;
        if (frameCount == 1)
            cout << " PASS" << endl;
        else {
            const Position2D oldPos = S0.getPlayerPos();
            const Position2D newPos = S1.getPlayerPos();
            cout << " S0, A0, S1: (" << oldPos._X << ", " << oldPos._Y << "), " << action << ", (" << newPos._X << ", " << newPos._Y << ")" << endl;
            cout << " FAIL" << endl;
        }
        };
	// Lambda tests responsiveness in server direct response mode
    auto testActionDirect = [this](playerAction action) {
        float deltaX = 0.0f;
        float deltaY = 0.0f;
        switch (action) {
        case moveNorth:
            deltaY = 0.005f;
            break;
        case moveWest:
            deltaX = -0.005f;
            break;
        case moveSouth:
            deltaY = -0.005f;
            break;
        case moveEast:
            deltaX = 0.005f;
            break;
        default:
            cout << "Only cardinal directions supported" << endl;
            return;
        }
        Transition chosenTransition;
        while (true) {
            _server.activateDirectResponse();
            {
                const Transition deathTransition = _server.getNewestTransition(false);
                if (deathTransition._S1.getPlayerDeath()) {
                    _server.deactivateDirectResponse();
                    Sleep(5000);
                    actionMacros::moveToCenter(&_server, 0, 0.3f);
                    continue;
                }
            }
            chosenTransition = _server.getOldestTransition(true);
            if (chosenTransition._A0 == action)
                break;
            _server.deactivateDirectResponse();
            actionMacros::moveToCenter(&_server, 0, 0.3f);
            Sleep(250);
        }
        _server.deactivateDirectResponse();
        const Position2D oldPos = chosenTransition._S0.getPlayerPos();
        const Position2D newPos = chosenTransition._S1.getPlayerPos();
        if ((deltaX <= 0 && newPos._X - oldPos._X <= deltaX || deltaX >= 0 && newPos._X - oldPos._X >= deltaX) &&
            (deltaY <= 0 && newPos._Y - oldPos._Y <= deltaY || deltaY >= 0 && newPos._Y - oldPos._Y >= deltaY))
            cout << "Action reflected in next frame - PASS" << endl;
        else {
            cout << "Action NOT reflected in next frame - FAIL" << endl;
            cout << "S0, A0, S1: (" << oldPos._X << ", " << oldPos._Y << "), " << chosenTransition._A0 << ", (" << newPos._X << ", " << newPos._Y << ")" << endl;
        }
        };
    _server.defaultStartup();
    actionMacros::enterSpecificLevel(&_server, 1);
    cout << "Testing the response time of the game state given an action stimulus... " << endl;
    Sleep(500);
    if (fullTest) {
        testActionIndirect(moveNorth);
        testActionIndirect(moveSouth);
        testActionIndirect(moveEast);
        testActionIndirect(moveWest);
        testActionIndirect(moveNorthwest);
        testActionIndirect(moveNortheast);
        testActionIndirect(moveSouthwest);
        testActionIndirect(moveSoutheast);
    }
    testActionDirect(moveNorth);
    testActionDirect(moveSouth);
    testActionDirect(moveEast);
    testActionDirect(moveWest);
    actionMacros::leaveLevel(&_server);
    actionMacros::exitGame(&_server);
}
void Control::inOutTest() {
    InOutTest test(&_server);
    test.test1Funct();
}

void Control::desyncTest() {
    _DeepQ = std::make_unique<DeepQManager>(_DQN.get());
    _server.setupDirectResponse(_DQN.get(), _DeepQ->getReplayBuffer());
    AutoRecoveryTest myTest(&_server);
    myTest.test();
}

void Control::trainingStateTest() {
    _DeepQ = std::make_unique<DeepQManager>(_DQN.get());
    TrainingStateTest myTest(_DQN.get(), _DeepQ.get());
    myTest.test();
}

void Control::maximumRewardTest() {
    initializeControl(false, false, false, false);
    MaximumRewardTest myTest;
	myTest.test();
}