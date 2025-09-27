#include "pch.h"
#include "InOutTest.h"
#include "InterfaceServer.h"
#include "definitions.h"
#include "helpers.h"
#include "actionMacros.h"

using std::cout, std::endl;
using namespace helpers;
using namespace actionMacros;

bool InOutTest::test1Funct() {
    cout << "Beginning Test1" << endl;

    _server->defaultStartup();
    _server->resetRecvCounter();
    LARGE_INTEGER start, freq;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);

    // Put things you want to quick test here followed by a return, so you don't have to wait through the whole test
    {
    }

    subtest1();
    subtest2();
    subtest2_2();
    subtest3(false);
    subtest4();
    subtest5();
    subtest6();
    subtest7();
    subtest8();
    subtestX(start, freq);
    cout << endl << "Overall test result: ";
    if (_testFailed)
        cout << "FAILED" << endl;
    else
        cout << "PASSED" << endl;
    exitGame(_server);
    return _testFailed;
}

void InOutTest::subtest1() {
    cout << "Subtest 1: Level Selection" << endl;
    enterLevel(_server);
    _server->getGameState(_gameState);
    const int initial = _gameState.getCurrentLevel();
    cout << "    Initial level = " << initial << endl;
    leaveLevel(_server);
    _server->sendAction(moveWest);
    Sleep(250);
    enterLevel(_server);
    _server->getGameState(_gameState);
    const int final = _gameState.getCurrentLevel();
    cout << "    Final level = " << final << endl;
    if (initial != final)
        cout << "    Subtest 1 PASSED" << endl;
    else {
        cout << "    Subtest 1 FAILED" << endl;
        _testFailed = true;
    }
    leaveLevel(_server);
}

void InOutTest::subtest2() {
    enterSpecificLevel(_server, 1);
    cout << "Subtest 2: Movement Inputs and Player Position" << endl;
    float oldPosX, oldPosY, newPosX, newPosY;
    auto sendAndUpdate = [this, &oldPosX, &oldPosY, &newPosX, &newPosY](playerAction action) {
        _server->sendAction(action);
        Sleep(130);
        _server->sendAction(noop);
        Sleep(300);
        oldPosX = newPosX;
        oldPosY = newPosY;
        _server->getGameState(_gameState);
        Position2D temp = _gameState.getPlayerPos();
        newPosX = temp._X;
        newPosY = temp._Y;
        };
    // Get initial game state
    _server->getGameState(_gameState);

    oldPosX = 0; oldPosY = 0; newPosX = _gameState.getPlayerPos()._X; newPosY = _gameState.getPlayerPos()._Y;
    cout << "    moveNorth: ";
    sendAndUpdate(moveNorth);
    if (newPosY - oldPosY >= 0.025f)
        cout << "PASS" << endl;
    else {
        cout << "FAIL" << endl;
        _testFailed = true;
    }
    cout << "    moveSouth: ";
    sendAndUpdate(moveSouth);
    if (newPosY - oldPosY <= -0.025f)
        cout << "PASS" << endl;
    else {
        cout << "FAIL" << endl;
        _testFailed = true;
    }
    cout << "    moveNortheast ";
    sendAndUpdate(moveNortheast);
    if (newPosY - oldPosY >= 0.0125f && newPosX - oldPosX >= 0.0125f)
        cout << "PASS" << endl;
    else {
        cout << "FAIL" << endl;
        _testFailed = true;
    }
    cout << "    moveSouthwest ";
    sendAndUpdate(moveSouthwest);
    if (newPosY - oldPosY <= -0.0125f && newPosX - oldPosX <= -0.0125f)
        cout << "PASS" << endl;
    else {
        cout << "FAIL" << endl;
        _testFailed = true;
    }
    cout << "    moveEast ";
    sendAndUpdate(moveEast);
    if (newPosX - oldPosX >= 0.025f)
        cout << "PASS" << endl;
    else {
        cout << "FAIL" << endl;
        _testFailed = true;
    }
    cout << "    moveWest ";
    sendAndUpdate(moveWest);
    if (newPosX - oldPosX <= -0.025f)
        cout << "PASS" << endl;
    else {
        cout << "FAIL" << endl;
        _testFailed = true;
    }
    cout << "    moveSoutheast ";
    sendAndUpdate(moveSoutheast);
    if (newPosY - oldPosY <= -0.0125f && newPosX - oldPosX >= 0.0125f)
        cout << "PASS" << endl;
    else {
        cout << "FAIL" << endl;
        _testFailed = true;
    }
    cout << "    moveNorthwest ";
    sendAndUpdate(moveNorthwest);
    if (newPosY - oldPosY >= 0.0125f && newPosX - oldPosX <= -0.0125f)
        cout << "PASS";
    else {
        cout << "FAIL" << endl;
        _testFailed = true;
    }
    cout << endl;
    leaveLevel(_server);
}

void InOutTest::subtest2_2() {
    auto printAndTestDash = [this](bool expected) {
        cout << "dashReady is ";
        StatePacket temp;
        _server->getGameState(temp);
        cout << temp.getDashReady();
        if (temp.getDashReady() == expected)
            cout << " PASS" << endl;
        else {
            cout << " FAIL" << endl;
            _testFailed = true;
        }
        };
    cout << "Subtest 2.2: Dashing" << endl;
    cout << "    At level start, ";
    enterSpecificLevel(_server, 10);
    LARGE_INTEGER start, freq;
    QueryPerformanceFrequency(&freq);
    printAndTestDash(true);
    float time = 0;
    _server->sendAction(dashEast);
    QueryPerformanceCounter(&start);
    Sleep(2 * ONEFRAMEMS);
    _server->sendAction(noop);
    while (true) {
        time = getTime(start, freq);
        _server->getGameState(_gameState);
        if (time >= 0.2f || !_gameState.getDashReady())
            break;
        Sleep(static_cast<DWORD>(0.5 * ONEFRAMEMS));
    }
    cout << "    Time until dash flag set to false: " << time << " seconds. ";
    if (time >= 0.2f) {
        cout << "FAIL" << endl;
        _testFailed = true;
    }
    else
        cout << "PASS" << endl;
    while (true) {
        time = getTime(start, freq);
        _server->getGameState(_gameState);
        if (time >= 1.0f || _gameState.getDashReady())
            break;
        Sleep(static_cast<DWORD>(0.5 * ONEFRAMEMS));
    }
    cout << "    Time until dash flag set to true: " << time << " seconds. ";
    if (time >= 1.0f) {
        cout << "FAIL" << endl;
        _testFailed = true;
    }
    else
        cout << "PASS" << endl;

    auto testDirectionalDash = [this](playerAction act, float x, float y) {
        _server->getGameState(_gameState);
        Position2D oldPos = _gameState.getPlayerPos();
        _server->sendAction(act);
        Sleep(2 * ONEFRAMEMS);
        _server->sendAction(noop);
        LARGE_INTEGER start, freq;
        QueryPerformanceFrequency(&freq);
        QueryPerformanceCounter(&start);
        while (true) {
            _server->getGameState(_gameState);
            if (!_gameState.getDashReady())
                break;
            if (getTime(start, freq) > 1.0f)
                break;
            Sleep(static_cast<DWORD>(0.5*ONEFRAMEMS));
        }
        while (true) {
            _server->getGameState(_gameState);
            if (_gameState.getDashReady())
                break;
            if (getTime(start, freq) > 1.0f)
                break;
            Sleep(static_cast<DWORD>(0.5*ONEFRAMEMS));
        }
        if (act == dashWest)
            int i = 0;
        Position2D newPos = _gameState.getPlayerPos();
        bool pass;
        if (x == 0.0f)
            pass = abs((newPos._Y - oldPos._Y) - y) >= EPSILON;
        else if (y == 0.0f)
            pass = abs((newPos._X - oldPos._X) - x) >= EPSILON;
        else
            pass = abs((newPos._Y - oldPos._Y) - y) >= EPSILON && abs((newPos._X - oldPos._X) - x) >= EPSILON;
        if (pass)
            cout << "PASS" << endl;
        else {
            cout << "FAIL" << endl;
            _testFailed = true;
        }
        };
    cout << "    West: ";
    testDirectionalDash(dashWest, -0.1f, 0.0f);
    cout << "    North: ";
    testDirectionalDash(dashNorth, 0.0f, 0.1f);
    cout << "    South: ";
    testDirectionalDash(dashSouth, 0.0f, -0.1f);
    cout << "    Northeast: ";
    testDirectionalDash(dashNortheast, 0.1f, 0.1f);
    cout << "    Southwest: ";
    testDirectionalDash(dashSouthwest, -0.1f, -0.1f);
    cout << "    Northwest: ";
    testDirectionalDash(dashNorthwest, -0.1f, 0.1f);
    cout << "    Southeast: ";
    testDirectionalDash(dashSoutheast, 0.1f, -0.1f);
    cout << "    East: ";
    testDirectionalDash(dashEast, 0.1f, 0.0f);
    leaveLevel(_server);
}

void InOutTest::subtest3(bool fullTest) {
    cout << "Subtest 3: Generator repair" << endl;
    bool generatorRecord[4];
    for (int i = 0; i < 4; i++)
        generatorRecord[i] = false;

    while (!generatorRecord[0] || !generatorRecord[1] || !generatorRecord[2] || !generatorRecord[3]) {
        enterSpecificLevel(_server, 9);
        Sleep(4500 - ULA_INTRO_DURATION_MS - BLOCKED_INPUTS_ON_LOAD_DURATION_MS);
        int brokenGenerator = breakGenerator(_server);
        if (generatorRecord[brokenGenerator]) {
        }
        else {
            _server->getGameState(_gameState);
            GeneratorData generator = _gameState.getGenerator(brokenGenerator);
            cout << "    After breaking, "; outputGeneratorStatus(_gameState, brokenGenerator);
            if (generator._broken && generator._want1 == 0 && generator._want2 == 0)
                cout << " PASS" << endl;
            else {
                cout << " FAIL" << endl;
                _testFailed = true;
            }
            Sleep(100);
            {
                int failures = 0;
                while (failures < 3) {
                    checkGeneratorZmora3(_server, brokenGenerator);
                    _server->getGameState(_gameState);
                    generator = _gameState.getGenerator(brokenGenerator);
                    if (generator._broken && generator._want1 != 10 && generator._want1 != 0 && generator._want2 == 10) {
                        cout << "    After checking, "; outputGeneratorStatus(_gameState, brokenGenerator);
                        cout << " PASS" << endl;
                        break;
                    }
                    else {
                        failures++;
                        leaveLevel(_server);
                        enterSpecificLevel(_server, 9);
                        breakGenerator(_server);
                        Sleep(100);
                    }
                }
                if (failures >= 3) {
                    cout << "    After checking, "; outputGeneratorStatus(_gameState, brokenGenerator);
                    cout << " FAIL" << endl;
                    _testFailed = true;
                }
            }

            repairGeneratorZmora3(_server, brokenGenerator, generator._want1);
            Sleep(100);
            _server->getGameState(_gameState);
            generator = _gameState.getGenerator(brokenGenerator);
            cout << "    After repairing, "; outputGeneratorStatus(_gameState, brokenGenerator);
            if (!generator._broken && generator._want1 == 10 && generator._want2 == 10)
                cout << " PASS" << endl;
            else {
                cout << " FAIL" << endl;
                _testFailed = true;
            }
            waitForIntro(_server);
            generatorRecord[brokenGenerator] = true;
        };
        leaveLevel(_server);
        if (!fullTest)
            break;
    }
}

void InOutTest::subtest4() {
    cout << "Subtest 4: Ghost positions" << endl;
    auto printGhost = [this](ghostID ghost) {
        _server->getGameState(_gameState);
        cout << "    " << StatePacket::toStr(ghost) << " position is " << _gameState.getGhostPos(ghost)._X << ", " << _gameState.getGhostPos(ghost)._Y;
        };
    auto ghostValid = [this](ghostID ghost) {
        _server->getGameState(_gameState);
        Position2D ghostPos = _gameState.getGhostPos(ghost);
        return abs(ghostPos._X) <= 1.01 && abs(ghostPos._Y) <= 1.01;
        };

    summonZmora2(_server);
    Sleep(100);
    printGhost(zmora2);
    if (ghostValid(zmora2))
        cout << " PASS" << endl;
    else {
        cout << " FAIL" << endl;
        _testFailed = true;
    }
    leaveLevel(_server);

    summonZmora1(_server);
    Sleep(100);
    printGhost(zmora1);
    if (ghostValid(zmora1))
        cout << " PASS" << endl;
    else {
        cout << " FAIL" << endl;
        _testFailed = true;
    }
    leaveLevel(_server);

    summonZmora3(_server);
    Sleep(100);
    printGhost(zmora3);
    if (ghostValid(zmora3))
        cout << " PASS" << endl;
    else {
        cout << " FAIL" << endl;
        _testFailed = true;
    }
    leaveLevel(_server);

    summonNikita(_server);
    Sleep(100);
    printGhost(nikita);
    if (ghostValid(nikita))
        cout << " PASS" << endl;
    else {
        cout << " FAIL" << endl;
        _testFailed = true;
    }
    leaveLevel(_server);

    summonStriga(_server);
    Sleep(100);
    printGhost(striga);
    if (ghostValid(striga))
        cout << " PASS" << endl;
    else {
        cout << " FAIL" << endl;
        _testFailed = true;
    }
    leaveLevel(_server);

    summonBull(_server);
    Sleep(100);
    printGhost(bull);
    if (ghostValid(bull))
        cout << " PASS" << endl;
    else {
        cout << " FAIL" << endl;
        _testFailed = true;
    }
    leaveLevel(_server);

    summonCutwire1(_server);
    Sleep(100);
    printGhost(cutwire1);
    if (ghostValid(cutwire1))
        cout << " PASS" << endl;
    else {
        cout << " FAIL" << endl;
        _testFailed = true;
    }
    leaveLevel(_server);

    summonCutwire2(_server);
    Sleep(100);
    printGhost(cutwire2);
    if (ghostValid(cutwire2))
        cout << " PASS" << endl;
    else {
        cout << " FAIL" << endl;
        _testFailed = true;
    }
    leaveLevel(_server);
}

void InOutTest::subtest5() {
    cout << "Subtest 5: Held items" << endl;

    enterSpecificLevel(_server, 11);
    pickupGearLevel11(_server);
    Sleep(100);
    _server->getGameState(_gameState);
    cout << "    Gear: " << componentToStr(_gameState.getHeldItem(0));
    if (_gameState.getHeldItem(0) == gear)
        cout << " PASS" << endl;
    else {
        cout << " FAIL" << endl;
    }
    leaveLevel(_server);

    enterSpecificLevel(_server, 11);
    pickupPipeLevel11(_server);
    Sleep(100);
    _server->getGameState(_gameState);
    cout << "    Pipe: " << componentToStr(_gameState.getHeldItem(0));
    if (_gameState.getHeldItem(0) == pipe)
        cout << " PASS" << endl;
    else {
        cout << " FAIL" << endl;
    }
    leaveLevel(_server);

    enterSpecificLevel(_server, 11);
    pickupRedBatteryLevel11(_server);
    Sleep(100);
    _server->getGameState(_gameState);
    cout << "    Red Battery: " << componentToStr(_gameState.getHeldItem(0));
    if (_gameState.getHeldItem(0) == redBattery)
        cout << " PASS" << endl;
    else {
        cout << " FAIL" << endl;
    }
    leaveLevel(_server);

    enterSpecificLevel(_server, 11);
    pickupBlueBatteryLevel11(_server);
    Sleep(100);
    _server->getGameState(_gameState);
    cout << "    Blue Battery: " << componentToStr(_gameState.getHeldItem(0));
    if (_gameState.getHeldItem(0) == blueBattery)
        cout << " PASS" << endl;
    else {
        cout << " FAIL" << endl;
    }
    leaveLevel(_server);

    enterSpecificLevel(_server, 11);
    pickupBlueThingLevel11(_server);
    Sleep(100);
    _server->getGameState(_gameState);
    cout << "    Blue Thing: " << componentToStr(_gameState.getHeldItem(0));
    if (_gameState.getHeldItem(0) == blueThing)
        cout << " PASS" << endl;
    else {
        cout << " FAIL" << endl;
    }
    leaveLevel(_server);

    // Assume that if held item 0 works, held item 1 works
}

void InOutTest::subtest6() {
    auto printMachine = [](const ComponentMachineData& temp) {
        cout << "    " << componentToStr(temp._ID) << " Machine at (" << temp._position._X << ", " << temp._position._Y << ") is " << statusToStr(temp._status);
        };
    auto testStatus = [this](const ComponentMachineData& temp, machineStatus stat) {
        if (temp._status == stat && abs(temp._position._X) < 1.5 && abs(temp._position._Y) < 1.5 ||
            temp._ID == blueBattery && abs(temp._position._X) < 1.5 && abs(temp._position._Y) < 1.5)
            cout << " PASS" << endl;
        else {
            cout << " FAIL" << endl;
            _testFailed = true;
        }
        };
    cout << "Subtest6: component machines" << endl;
    // assume that if one timer works they all do

    // Gear full test
    enterSpecificLevel(_server, 1);
    {
        Sleep(100);
        _server->getGameState(_gameState);
        ComponentMachineData temp = _gameState.getMachine(gear);
        cout << "    At start of level 1, ";
        printMachine(temp);
        testStatus(temp, empty);
    }
    _server->sendAction(moveSouth);
    Sleep(150);
    _server->sendAction(moveWest);
    Sleep(1200);
    _server->sendAction(interact);
    Sleep(2 * ONEFRAMEMS);
    _server->sendAction(noop);
    {
        Sleep(100);
        _server->getGameState(_gameState);
        ComponentMachineData temp = _gameState.getMachine(gear);
        cout << "    After activating, ";
        printMachine(temp);
        testStatus(temp, working);
    }
    Sleep(6000);
    {
        _server->getGameState(_gameState);
        ComponentMachineData temp = _gameState.getMachine(gear);
        cout << "    After finishing, ";
        printMachine(temp);
        testStatus(temp, ready);
    }
    leaveLevel(_server);

    // Red Battery full test
    enterSpecificLevel(_server, 8);
    {
        Sleep(100);
        _server->getGameState(_gameState);
        ComponentMachineData temp = _gameState.getMachine(redBattery);
        cout << "    At start of level 8, ";
        printMachine(temp);
        testStatus(temp, ready);
    }
    _server->sendAction(moveEast);
    Sleep(1000);
    _server->sendAction(noop);
    Sleep(200);
    _server->sendAction(moveNorth);
    Sleep(200);
    _server->sendAction(noop);
    Sleep(200);
    _server->sendAction(interact);
    Sleep(2 * ONEFRAMEMS);
    _server->sendAction(noop);
    Sleep(2 * ONEFRAMEMS);
    {
        Sleep(100);
        _server->getGameState(_gameState);
        ComponentMachineData temp = _gameState.getMachine(redBattery);
        cout << "    After taking, ";
        printMachine(temp);
        testStatus(temp, empty);
    }
    _server->sendAction(moveSoutheast);
    Sleep(400);
    _server->sendAction(noop);
    Sleep(200);
    _server->sendAction(interact);
    Sleep(2 * ONEFRAMEMS);
    _server->sendAction(noop);
    Sleep(2 * ONEFRAMEMS);
    _server->sendAction(moveNorthwest);
    Sleep(400);
    _server->sendAction(noop);
    Sleep(200);
    _server->sendAction(interact);
    Sleep(2 * ONEFRAMEMS);
    _server->sendAction(noop);
    Sleep(2 * ONEFRAMEMS);
    {
        Sleep(100);
        _server->getGameState(_gameState);
        ComponentMachineData temp = _gameState.getMachine(redBattery);
        cout << "    After placing, ";
        printMachine(temp);
        testStatus(temp, working);
    }
    Sleep(6000);
    {
        _server->getGameState(_gameState);
        ComponentMachineData temp = _gameState.getMachine(redBattery);
        cout << "    After waiting, ";
        printMachine(temp);
        testStatus(temp, ready);
    }
    leaveLevel(_server);

    // Other 3 shallow test
    enterSpecificLevel(_server, 11);
    cout << "    At start of level 11:" << endl;
    Sleep(100);
    _server->getGameState(_gameState);
    cout << "    ";
    printMachine(_gameState.getMachine(pipe));
    testStatus(_gameState.getMachine(pipe), ready);
    cout << "    ";
    printMachine(_gameState.getMachine(blueBattery));
    testStatus(_gameState.getMachine(blueBattery), ready);
    cout << "    ";
    printMachine(_gameState.getMachine(blueThing));
    testStatus(_gameState.getMachine(blueThing), ready);

    cout << "    On pickup:" << endl;
    pickupPipeLevel11(_server);
    Sleep(100);
    _server->getGameState(_gameState);
    cout << "    ";
    printMachine(_gameState.getMachine(pipe));
    testStatus(_gameState.getMachine(pipe), empty);
    leaveLevel(_server);
    enterSpecificLevel(_server, 11);
    pickupBlueBatteryLevel11(_server);
    Sleep(100);
    _server->getGameState(_gameState);
    cout << "    ";
    printMachine(_gameState.getMachine(blueBattery));
    testStatus(_gameState.getMachine(blueBattery), ready);
    leaveLevel(_server);
    enterSpecificLevel(_server, 11);
    pickupBlueThingLevel11(_server);
    Sleep(100);
    _server->getGameState(_gameState);
    cout << "    ";
    printMachine(_gameState.getMachine(blueThing));
    testStatus(_gameState.getMachine(blueThing), empty);

    leaveLevel(_server);
}

void InOutTest::subtest7() {
    cout << "Subtest 7: Hazard positions" << endl;
    auto dist = [](const Position2D& left, const Position2D& right) {
        return sqrt(pow(left._X - right._X, 2.0) + pow(left._Y - right._Y, 2.0));
        };
    auto waitUntilProjectiles = [this, &dist](int proj, bool test = true) {
        LARGE_INTEGER start, freq;
        QueryPerformanceFrequency(&freq);
        QueryPerformanceCounter(&start);
        while (true) {
            _server->getGameState(_gameState);
            if (_gameState.getHazard(proj - 1)._X != -1000) {
                if (test) {
                    Position2D playerPos = _gameState.getPlayerPos();
                    cout << "      Player position: (" << playerPos._X << ", " << playerPos._Y << ")" << endl;
                    for (int i = 0; i < proj; i++) {
                        _server->getGameState(_gameState);
                        Position2D temp = _gameState.getHazard(i);
                        cout << "        Hazard " << i << " is at (" << temp._X << ", " << temp._Y << ") ";
                        if (abs(temp._X) < 1.5 && abs(temp._Y) < 1.5)
                            cout << "PASS" << endl;
                        else
                        {
                            cout << "FAIL" << endl;
                            _testFailed = true;
                        }
                    }
                    float lastDist = -1;
                    for (int i = 0; i < proj; i++) {
                        Position2D temp = _gameState.getHazard(i);
                        if (dist(temp, playerPos) < lastDist) {
                            cout << "      ERROR: hazards are in wrong order (if they are correct)" << endl;
                            lastDist = dist(temp, playerPos);
                        }
                    }
                }
                break;
            }
            else if (getTime(start, freq) >= 5.0f) {
                Sleep(8000);
                cout << "Timed out; FAIL" << endl;
                _testFailed = true;
                break;
            }
            Sleep(static_cast<DWORD>(0.5*ONEFRAMEMS));
        }
        };
    cout << "    Zmora 2 lightning projectiles" << endl;
    summonZmora2(_server);
    _server->getGameState(_gameState);
    if (_gameState.getPlayerPos()._X < 0) {
        _server->sendAction(moveEast);
        Sleep(1300);
    }
    else {
        _server->sendAction(moveWest);
        Sleep(500);
        _server->sendAction(moveNorth);
        Sleep(1000);
    }
    _server->sendAction(noop);
    Sleep(200);
    _server->sendAction(moveSouthwest);
    Sleep(200);
    _server->sendAction(noop);
    while (true) {
        _server->getGameState(_gameState);
        if (abs(_gameState.getPlayerPos()._X - _gameState.getGhostPos(zmora2)._X) < 0.01)
            break;
        Sleep(static_cast<DWORD>(0.5*ONEFRAMEMS));
    }
    _server->sendAction(moveSouthwest);
    Sleep(500);
    _server->sendAction(noop);
    Sleep(300);
    waitUntilProjectiles(3);
    leaveLevel(_server);

    cout << "    Cutwire 1 throwing knives" << endl;
    summonCutwire1(_server);
    moveToCenter(_server);
    _server->sendAction(moveSouth);
    Sleep(200);
    _server->sendAction(noop);
    waitUntilProjectiles(3);
    cout << "    Cutwire 1 mines" << endl;
    _server->sendAction(moveEast);
    Sleep(500);
    _server->sendAction(noop);
    Sleep(500);
    _server->sendAction(moveNorth);
    Sleep(500);
    _server->sendAction(noop);
    Sleep(1000);
    waitUntilProjectiles(1);
    leaveLevel(_server);

    cout << "    Nikita flame wave" << endl;
    summonNikita(_server);
    moveToCenter(_server);
    waitUntilProjectiles(3);
    _server->sendAction(moveEast);
    Sleep(700);
    _server->sendAction(moveSouth);
    Sleep(200);
    _server->sendAction(noop);
    Sleep(1000);
    cout << "    Nikita fireball" << endl;
    waitUntilProjectiles(1);
    leaveLevel(_server);

    cout << "    Boiler flame wave" << endl;
    enterSpecificLevel(_server, 6);
    waitUntilProjectiles(3);
    leaveLevel(_server);

    cout << "    Lightning generators" << endl;
    enterSpecificLevel(_server, 9);
    waitUntilProjectiles(3);
    leaveLevel(_server);

    {
        cout << "    Striga knife throw" << endl;
        int failures = 0;
        _server->getPlayerDeathFlag(true);
        while (failures < 3) {
            summonStriga(_server);
            Sleep(1000);
            _server->sendAction(moveWest);
            Sleep(500);
            _server->sendAction(noop);
            Sleep(500);
            _server->sendAction(moveEast);
            Sleep(300);
            _server->sendAction(noop);
            if (_server->getPlayerDeathFlag(true)) {
                failures++;
                Sleep(5000);
                leaveLevel(_server);
                continue;
            }
            waitUntilProjectiles(3);
            _server->sendAction(moveEast);
            Sleep(200);
            _server->sendAction(noop);

            if (_server->getPlayerDeathFlag(true)) {
                failures++;
                Sleep(5000);
                leaveLevel(_server);
                continue;
            }
            cout << "    Striga rockets" << endl;
            waitUntilProjectiles(1, false);
            _server->sendAction(moveSoutheast);
            Sleep(100);
            _server->sendAction(moveSouth);
            Sleep(300);
            waitUntilProjectiles(3);
            leaveLevel(_server);
            break;
        }
        if (failures >= 3) {
            cout << "FAIL" << endl;
            _testFailed = true;
        }
    }

    // Zmora 3 thunder was checked manually
    // Zmora 3 shockwave was checked manually
    // Striga phase done manually
    // Striga doggies done manually
    // Skullbomb done manually
    // Skullbomb projectiles done manually
    // Slam projectiles done manually

    cout << "    Big striga walls" << endl;
    enterSpecificLevel(_server, 12);
    breakGenerator(_server);
    _server->sendAction(moveWest);
    Sleep(500);
    _server->sendAction(noop);
    Sleep(2500);
    _server->sendAction(moveSouth);
    Sleep(500);
    _server->sendAction(noop);
    waitUntilProjectiles(3);
    leaveLevel(_server);

    return;
}
void InOutTest::subtest8() {
    cout << "subtest 8: Player death" << endl;
    _server->getPlayerDeathFlag(true);
    summonBull(_server);
    bool deathSinceLastCheck = _server->getPlayerDeathFlag(true);
    cout << "    start of level: " << deathSinceLastCheck;
    if (deathSinceLastCheck) {
        cout << " FAIL" << endl;
        _testFailed = true;
    }
    else
        cout << " PASS" << endl;
    Sleep(5000);
    deathSinceLastCheck = _server->getPlayerDeathFlag(true);
    cout << "    after dying: " << deathSinceLastCheck;
    if (deathSinceLastCheck)
        cout << " PASS" << endl;
    else {
        cout << " FAIL" << endl;
        _testFailed = true;
    }
    Sleep(1000);
    leaveLevel(_server);
    deathSinceLastCheck = _server->getPlayerDeathFlag(true);
    cout << "    after leaving level: " << deathSinceLastCheck;
    if (deathSinceLastCheck) {
        cout << " FAIL" << endl;
        _testFailed = true;
    }
    else
        cout << " PASS" << endl;
    exitGame(_server);
    return;
}
void InOutTest::subtestX(LARGE_INTEGER& start, LARGE_INTEGER& freq) {
    const float time = getTime(start, freq);
    cout << "Subtest X: Game State Frequency" << endl;
    int recvFreq = static_cast<int>(_server->getRecvCounter() / time);
    cout << "    Game state received from Awaria.exe at an average frequency of " << recvFreq << " Hz." << endl;
    if (recvFreq >= 0.8*FRAMERATE)
        cout << "    Subtest X PASSED" << endl;
    else {
        cout << "    Subtest X FAILED" << endl;
        _testFailed = true;
    }
}

void InOutTest::outputGeneratorStatus(const StatePacket& _gameState, int generator) const {
    GeneratorData temp = _gameState.getGenerator(generator);
    cout << "generator " << generator << " is ";
    if (temp._broken)
        cout << "broken and wants ";
    else
        cout << "not broken and wants ";
    cout << temp._want1 << " and " << temp._want2;
}

