#include "pch.h"
#include "actionMacros.h"
#include "definitions.h"
#include "Interfaceserver.h"
#include "MyExcept.h"

using namespace helpers;
using std::cout, std::endl;

void actionMacros::infiniteMovementLoop(InterfaceServer* server) {
    Sleep(3000);
    while (true) {
        server->getGameStateToConsole();
        server->sendAction(moveWest);
        Sleep(250);
        server->getGameStateToConsole();
        server->sendAction(moveNorth);
        Sleep(250);
        server->getGameStateToConsole();
        server->sendAction(moveEast);
        Sleep(250);
        server->getGameStateToConsole();
        server->sendAction(moveSouth);
        Sleep(250);
    }
}
void actionMacros::enterLevel(InterfaceServer* server) {
    for (int i = 0; i < 2; i++) {
        server->sendAction(interact);
        Sleep(3*ONEFRAMEMS);
        server->sendAction(noop);
        Sleep(200);
    }
    Sleep(4300);
};
void actionMacros::leaveLevel(InterfaceServer* server) {
    server->sendAction(cancel);
    Sleep(3*ONEFRAMEMS);
    server->sendAction(noop);
    Sleep(250);
    server->sendAction(moveNorth);
    Sleep(3*ONEFRAMEMS);
    server->sendAction(noop);
    Sleep(250);
    server->sendAction(interact);
    Sleep(3*ONEFRAMEMS);
    server->sendAction(noop);
    Sleep(2000);
    server->sendAction(interact);
    Sleep(3*ONEFRAMEMS);
    server->sendAction(noop);
    Sleep(500);
}
void actionMacros::leaveLevelRobust(InterfaceServer* server) {
    StatePacket tempState;
    server->getGameState(tempState);
    while (true) {
        leaveLevel(server);
        server->getGameState(tempState);
        if (tempState.getCurrentLevel() == 0)
            break;
        Sleep(5000);
    }
}
void actionMacros::exitGame(InterfaceServer* server) {
    server->sendAction(cancel);
    Sleep(3*ONEFRAMEMS);
    server->sendAction(noop);
    Sleep(3*ONEFRAMEMS);
    for (int i = 0; i < 4; i++) {
        server->sendAction(moveSouth);
        Sleep(3*ONEFRAMEMS);
        server->sendAction(noop);
        Sleep(3*ONEFRAMEMS);
    }
    server->sendAction(interact);
    Sleep(250);
}
void actionMacros::enterSpecificLevel(InterfaceServer* server, int levelToEnter) {
    const int offset = 13 - levelToEnter;
    if (offset > 0) {
        for (int i = 0; i < offset; i++) {
            server->sendAction(moveWest);
            Sleep(3*ONEFRAMEMS);
            server->sendAction(noop);
            Sleep(3*ONEFRAMEMS);
        }
    }
    server->sendAction(interact);
    Sleep(3 * ONEFRAMEMS);
    server->sendAction(noop);
    Sleep(3 * ONEFRAMEMS);
    server->sendAction(interact);
    Sleep(3 * ONEFRAMEMS);
    server->sendAction(noop);
    switch (levelToEnter) {
    case 10:
    case 11:
    case 12:
        Sleep(YAGA_INTRO_DURATION_MS);
        break;
    case 13:
        Sleep(SUPER_INTRO_DURATION_MS);
        break;
    default:
        Sleep(ULA_INTRO_DURATION_MS);
        break;
    }
    Sleep(BLOCKED_INPUTS_ON_LOAD_DURATION_MS);
    //while (true) {
    //    StatePacket temp;
    //    server->getGameState(temp);
    //    for (auto i = 0; i < 4; i++) {
    //        if (temp.getGenerator(i)._position != Position2D(-1000, -1000))
    //            return;
    //    }
    //    Sleep(0.25 * ONEFRAMEMS);
    //}
}
// Untested
void actionMacros::enterLevelSelectFromWin(InterfaceServer* server, int currentLevel) {
    if (currentLevel == 13) {
        // Shortest animation containing a door
        Sleep(ULA_INTRO_DURATION_MS);
        server->sendAction(interact);
        Sleep(2 * ONEFRAMEMS);
        server->sendAction(noop);
        Sleep(2 * ONEFRAMEMS);
        server->sendAction(interact);
        Sleep(2 * ONEFRAMEMS);
        server->sendAction(noop);
        Sleep(2 * ONEFRAMEMS);
    }
    else {
        Sleep(ULA_INTRO_DURATION_MS);
        server->sendAction(moveWest);
        Sleep(2 * ONEFRAMEMS);
        server->sendAction(noop);
        Sleep(2 * ONEFRAMEMS);
    }
}
int actionMacros::breakGenerator(InterfaceServer* server) {
    server->sendAction(moveEast);
    Sleep(2*ONEFRAMEMS);
    server->sendAction(noop);
    Sleep(150);
    server->sendAction(moveWest);
    Sleep(2*ONEFRAMEMS);
    server->sendAction(noop);
    Sleep(800);
    StatePacket gameState;
    server->getGameState(gameState);
    if (gameState.getGenerator(0)._broken)
        return 0;
    else if (gameState.getGenerator(1)._broken)
        return 1;
    else if (gameState.getGenerator(2)._broken)
        return 2;
    else if (gameState.getGenerator(3)._broken)
        return 3;
    else
        return -1;
}
void actionMacros::waitForIntro(InterfaceServer* server) {
    server->sendAction(noop);
    Sleep(4000);
    server->sendAction(interact);
    Sleep(2 * ONEFRAMEMS);
    server->sendAction(noop);
    Sleep(500);
}
void actionMacros::moveToCenter(InterfaceServer* server, float xOffset, float yOffset) {
    StatePacket gameState;
    const float acceptableError = 0.05f;
    auto moveX = [&gameState, &acceptableError, &server, &xOffset]() {
        while (true) {
            server->getGameState(gameState);
            Position2D currentPos = gameState.getPlayerPos();
            if (currentPos._X - xOffset > acceptableError)
                server->sendAction(moveWest);
            else if (currentPos._X - xOffset < -acceptableError)
                server->sendAction(moveEast);
            else {
                server->sendAction(noop);
                return true;
            }
            Sleep(4 * ONEFRAMEMS);
            server->getGameState(gameState);
            Position2D newPos = gameState.getPlayerPos();
            if (abs(currentPos._X - newPos._X) < EPSILON) {
                server->sendAction(noop);
                return false;
            }
        }
        };
    auto moveY = [&gameState, &acceptableError, &server, &yOffset]() {
        while (true) {
            server->getGameState(gameState);
            Position2D currentPos = gameState.getPlayerPos();
            if (currentPos._Y - yOffset > acceptableError)
                server->sendAction(moveSouth);
            else if (currentPos._Y - yOffset < -acceptableError)
                server->sendAction(moveNorth);
            else {
                server->sendAction(noop);
                return true;
            }
            Sleep(4 * ONEFRAMEMS);
            server->getGameState(gameState);
            Position2D newPos = gameState.getPlayerPos();
            if (abs(currentPos._Y - newPos._Y) < EPSILON) {
                server->sendAction(noop);
                return false;
            }
        }
        };
    for (int i = 0; i < 3; i++) {
        bool xDone = moveX();
        bool yDone = moveY();
        if (xDone && yDone)
            return;
    }
}

void actionMacros::summonZmora1(InterfaceServer* server) {
    enterSpecificLevel(server, 1);

    server->sendAction(moveSouth);
    Sleep(150);
    server->sendAction(moveWest);
    Sleep(1200);
    server->sendAction(moveNorth);
    Sleep(2*ONEFRAMEMS);
    server->sendAction(interact);
    Sleep(6000);
    server->sendAction(interact);
    Sleep(100);
    server->sendAction(moveEast);
    Sleep(1500);
    server->sendAction(moveSouth);
    Sleep(200);
    server->sendAction(moveEast);
    Sleep(200);
    server->sendAction(interact);
    Sleep(2*ONEFRAMEMS);
    server->sendAction(noop);
    Sleep(150);
    server->sendAction(interact);
    Sleep(2*ONEFRAMEMS);
    waitForIntro(server);
}

void actionMacros::summonZmora2(InterfaceServer* server) {
    StatePacket gameState;

    enterSpecificLevel(server, 2);
    int brokenGenerator = breakGenerator(server);
    if (brokenGenerator == 1) {
        server->sendAction(moveWest);
        Sleep(1000);
    }
    else if (brokenGenerator == 2) {
        server->sendAction(moveSouth);
        Sleep(700);
        server->sendAction(moveEast);
        Sleep(750);
    }
    else
        MyExcept::error("Invalid generator");
    server->sendAction(interact);
    Sleep(2*ONEFRAMEMS);
    server->sendAction(noop);
    Sleep(150);
    
    server->getGameState(gameState);
    int requestedItem = gameState.getGenerator(brokenGenerator)._want1;
    if (brokenGenerator == 2 && requestedItem == 1) {
        server->sendAction(moveWest);
        Sleep(330);
        server->sendAction(moveNorth);
        Sleep(600);
        server->sendAction(moveNortheast);
        Sleep(500);
        server->sendAction(interact);
        Sleep(2*ONEFRAMEMS);
        server->sendAction(moveSouthwest);
        Sleep(250);
        server->sendAction(moveSouth);
        Sleep(600);
        server->sendAction(moveEast);
        Sleep(330);
    }
    else if (brokenGenerator == 2 && requestedItem == 2) {
        server->sendAction(moveWest);
        Sleep(1500);
        server->sendAction(interact);
        Sleep(2*ONEFRAMEMS);
        server->sendAction(moveEast);
        Sleep(1500);
    }
    else if (brokenGenerator == 1 && requestedItem == 2) {
        server->sendAction(moveEast);
        Sleep(400);
        server->sendAction(moveSouth);
        Sleep(600);
        server->sendAction(interact);
        Sleep(2*ONEFRAMEMS);
        server->sendAction(moveNorth);
        Sleep(700);
        server->sendAction(moveWest);
        Sleep(400);
    }
    else if (brokenGenerator == 1 && requestedItem == 1){
        server->sendAction(moveEast);
        Sleep(1500);
        server->sendAction(moveNorth);
        Sleep(150);
        server->sendAction(interact);
        Sleep(2*ONEFRAMEMS);
        server->sendAction(moveWest);
        Sleep(1500);
    }
    else {
        leaveLevel(server);
        summonZmora2(server);
        return;
    }

    server->sendAction(interact);
    Sleep(2*ONEFRAMEMS);
    server->sendAction(noop);
    Sleep(3000);
    server->sendAction(interact);
    Sleep(2*ONEFRAMEMS);
    server->sendAction(noop);
}

// The rough positions of objects will be hard coded here so I don't have to explore every possible path. Units of length are in character-speed-milliseconds
struct Position {
    float _x;
    float _y;
    Position() {
        _x = 0;
        _y = 0;
    }
    Position(float xx, float yy) {
        _x = xx;
        _y = yy;
    }
    bool operator==(const Position& other) const{
        return _x == other._x && _y == other._y;
    }
};

struct Zmora3Objects {
    static constexpr int x1 = -1290;
    static constexpr int x2 = -1290;
    static constexpr int x3 = -400;
    static constexpr int x0 = 0;
    static constexpr int x4 = -x3;
    static constexpr int x5 = -x2;
    static constexpr int x6 = -x1;
    static constexpr int y1 = -650;
    static constexpr int y0 = 0;
    static constexpr int y2 = 400;

    const Position _generators[4];
    const Position _playerPos;
    const Position _components[6];

    Zmora3Objects()
        : _generators{
            // top left
            Position(x2, y2),
            // bottom left
            Position(x1,y1),
            // top right
            Position(x5,y2),
            // bottom right
            Position(x6,y1)
        },
        _playerPos(x0, y0),
        _components{
            // unused
            Position(),
            // unused
            Position(),
            // top right (pipe)
            Position(x4,y2),
            // bottom left (red battery)
            Position(x3,y1),
            // top left (blue battery)
            Position(x3,y2),
            // bottom right (blue thing)
            Position(x4,y1)
        }
    {
    }
};

void actionMacros::moveToPositionZmora3(InterfaceServer* server, Position& currentPos, const Position& goalPos) {
    Zmora3Objects objects;
    float deltaX = goalPos._x - currentPos._x;
    float deltaY = goalPos._y - currentPos._y;
    auto moveY = [&server, deltaY]() {
        if (deltaY < 0) {
            server->sendAction(moveSouth);
            Sleep(static_cast<DWORD>(abs(deltaY)));
        }
        else if (deltaY > 0) {
            server->sendAction(moveNorth);
            Sleep(static_cast<DWORD>(deltaY));
        }
        server->sendAction(noop);
        Sleep(150);
        };
    auto moveX = [&server, deltaX]() {
        if (deltaX < 0) {
            server->sendAction(moveWest);
            Sleep(static_cast<DWORD>(abs(deltaX)));
        }
        else if (deltaX > 0) {
            server->sendAction(moveEast);
            Sleep(static_cast<DWORD>(deltaX));
        }
        server->sendAction(noop);
        Sleep(150);
        };
    if (abs(currentPos._x) > abs(goalPos._x)) {
        moveY();
        moveX();
    }
    else {
        moveX();
        moveY();
    }
    currentPos = goalPos;
}

void actionMacros::checkGeneratorZmora3(InterfaceServer* server, int brokenGenerator) {
    int failures = 0;
    Zmora3Objects objects;
    Position myPos = objects._playerPos;
    moveToPositionZmora3(server, myPos, objects._generators[brokenGenerator]);
    Sleep(100);
    server->sendAction(interact);
    Sleep(2*ONEFRAMEMS);
    server->sendAction(noop);
    Sleep(100);
}

void actionMacros::repairGeneratorZmora3(InterfaceServer* server, int brokenGenerator, int wantedItem) {
    Zmora3Objects objects;
    Position myPos = objects._generators[brokenGenerator];
    moveToPositionZmora3(server, myPos, objects._components[wantedItem]);
    server->sendAction(interact);
    Sleep(2*ONEFRAMEMS);
    moveToPositionZmora3(server, myPos, objects._generators[brokenGenerator]);
    server->sendAction(interact);
    Sleep(2*ONEFRAMEMS);
    server->sendAction(noop);
}

void actionMacros::summonZmora3(InterfaceServer* server) {
    Zmora3Objects objects;
    StatePacket gameState;

    enterSpecificLevel(server, 9);
    Sleep(4300 - ULA_INTRO_DURATION_MS - BLOCKED_INPUTS_ON_LOAD_DURATION_MS);
    int brokenGenerator = breakGenerator(server);
    checkGeneratorZmora3(server, brokenGenerator);
    server->getGameState(gameState);
    repairGeneratorZmora3(server, brokenGenerator, gameState.getGenerator(brokenGenerator)._want1);
    waitForIntro(server);
}

void actionMacros::summonNikita(InterfaceServer* server) {
    enterSpecificLevel(server, 5);
    int brokenGenerator = breakGenerator(server);
    if (brokenGenerator == 2) {
        server->sendAction(moveNortheast);
        Sleep(350);
        server->sendAction(moveEast);
        Sleep(1070);
    }
    if (brokenGenerator == 1) {
        server->sendAction(moveSouthwest);
        Sleep(200);
        server->sendAction(moveSouth);
        Sleep(600);
        server->sendAction(moveWest);
        Sleep(500);
    }
    server->sendAction(interact);
    Sleep(2*ONEFRAMEMS);
    server->sendAction(noop);
    Sleep(2 * ONEFRAMEMS);
    Sleep(200);

    StatePacket gameState;
    server->getGameState(gameState);
    int partWanted = gameState.getGenerator(brokenGenerator)._want1;
    if (brokenGenerator == 2) {
        if (partWanted == 5 || partWanted == 4) {
            server->sendAction(moveWest);
            Sleep(50);
            server->sendAction(noop);
            Sleep(200);
            server->sendAction(moveSouthwest);
            Sleep(700);
            server->sendAction(noop);
            Sleep(100);
            server->sendAction(moveSouth);
            Sleep(400);
            server->sendAction(noop);
            Sleep(100);
            if (partWanted == 5) {
                server->sendAction(moveWest);
                Sleep(200);
                server->sendAction(noop);
                Sleep(100);
                server->sendAction(interact);
                Sleep(2 * ONEFRAMEMS);
                server->sendAction(moveEast);
                Sleep(200);
                server->sendAction(noop);
                Sleep(100);
            }
            else if (partWanted == 4) {
                server->sendAction(moveEast);
                Sleep(200);
                server->sendAction(noop);
                Sleep(100);
                server->sendAction(interact);
                Sleep(2 * ONEFRAMEMS);
                server->sendAction(moveWest);
                Sleep(200);
                server->sendAction(noop);
                Sleep(100);
            }
            server->sendAction(moveNorth);
            Sleep(450);
            server->sendAction(noop);
            Sleep(100);
            server->sendAction(moveNortheast);
            Sleep(700);
            server->sendAction(noop);
            Sleep(100);
        }
        else {
            server->sendAction(moveWest);
            Sleep(1500);
            server->sendAction(noop);
            Sleep(100);
            server->sendAction(interact);
            Sleep(2 * ONEFRAMEMS);
            server->sendAction(moveEast);
            Sleep(1500);
        }
    }
    else if (brokenGenerator == 1) {
        if (partWanted == 5) {
            server->sendAction(moveEast);
            Sleep(800);
            server->sendAction(noop);
            Sleep(100);
            server->sendAction(interact);
            Sleep(2 * ONEFRAMEMS);
            server->sendAction(moveWest);
            Sleep(800);
        }
        else if (partWanted == 4) {
            server->sendAction(moveEast);
            Sleep(1400);
            server->sendAction(noop);
            Sleep(100);
            server->sendAction(interact);
            Sleep(2 * ONEFRAMEMS);
            server->sendAction(moveWest);
            Sleep(1400);
        }
        else {
            server->sendAction(moveEast);
            Sleep(400);
            server->sendAction(noop);
            Sleep(100);
            server->sendAction(moveNorth);
            Sleep(1000);
            server->sendAction(noop);
            Sleep(100);
            server->sendAction(interact);
            Sleep(2 * ONEFRAMEMS);
            server->sendAction(moveSouth);
            Sleep(1000);
            server->sendAction(moveWest);
            Sleep(400);
        }
    }
    else
        MyExcept::error("Invalid generator");
    server->sendAction(interact);
    Sleep(2*ONEFRAMEMS);
    waitForIntro(server);
}

struct Cutwire2Objects {
    static constexpr float x1 = -690;
    static constexpr float x0 = 0;
    static constexpr float x2 = -280;
    static constexpr float x3 = 120;
    static constexpr float x4 = 890;
    static constexpr float x5 = 1500;
    static constexpr float y0 = 0;
    static constexpr float y1 = -690;
    static constexpr float y2 = 150;

    const Position _generators[4];
    const Position _components[6];
    const Position _playerPos;

    Cutwire2Objects()
        : _generators{
            // top left
            Position(x1, y0),
            // bottom left
            Position(x1,y1),
            // top right
            Position(x5,y0),
            // bottom right
            Position(x5,y1)
        },
        _playerPos(x0, y0),
        _components{
            // unused
            Position(),
            // top left (gear)
            Position(x2,y2),
            // bottom right (pipe)
            Position(x4,y1),
            // top right (red battery)
            Position(x4,y2),
            // middle
            Position(x3,y1),
            // bottom left (blue thing)
            Position(x2,y1)
        }
    {
    }
};

void actionMacros::moveToPositionCutwire2(InterfaceServer* server, Position& currentPos, const Position& goalPos, bool intermediate) {
    Cutwire2Objects objects;
    auto moveY = [&server, &currentPos, &goalPos]() {
        float deltaY = goalPos._y - currentPos._y;
        if (deltaY < 0) {
            server->sendAction(moveSouth);
            Sleep(static_cast<DWORD>(abs(deltaY)));
        }
        else if (deltaY > 0) {
            server->sendAction(moveNorth);
            Sleep(static_cast<DWORD>(deltaY));
        }
        server->sendAction(noop);
        Sleep(150);
        };
    auto moveX = [&server, &currentPos, &goalPos]() {
        float deltaX = goalPos._x - currentPos._x;
        if (deltaX < 0) {
            server->sendAction(moveWest);
            Sleep(static_cast<DWORD>(abs(deltaX)));
        }
        else if (deltaX > 0) {
            server->sendAction(moveEast);
            Sleep(static_cast<DWORD>(deltaX));
        }
        server->sendAction(noop);
        Sleep(150);
        };
    if (currentPos._y > objects._generators[0]._y + EPSILON && !intermediate) {
        moveToPositionCutwire2(server, currentPos, Position(currentPos._x, objects._generators[0]._y), true);
        moveToPositionCutwire2(server, currentPos, goalPos, false);
        return;
    }
    else if (currentPos._x > objects._generators[0]._x+EPSILON && !intermediate) {
        moveToPositionCutwire2(server, currentPos, Position(objects._generators[0]._x, currentPos._y), true);
        moveToPositionCutwire2(server, currentPos, goalPos, false);
        return;
    }
    else if (goalPos._y > objects._generators[0]._y+EPSILON) {
        moveToPositionCutwire2(server, currentPos, objects._generators[0], true);
        moveX();
        moveY();
    }
    else {
        moveY();
        moveX();
    }
    currentPos = goalPos;
}

void actionMacros::summonCutwire2(InterfaceServer* server) {
    Cutwire2Objects objects;
    enterSpecificLevel(server, 10);
    for (int i = 0; i < 2; i++) {
        server->sendAction(dashEast);
        Sleep(2 * ONEFRAMEMS);
        server->sendAction(noop);
        Sleep(1000);
        server->sendAction(dashWest);
        Sleep(2 * ONEFRAMEMS);
        server->sendAction(noop);
        Sleep(1000);
    }
    server->sendAction(moveNorth);
    Sleep(80);
    server->sendAction(noop);
    Sleep(500);
    StatePacket gameState;
    server->getGameState(gameState);
    int brokenGenerator = -1;
    for (int i = 0; i < 4; i++) {
        if (gameState.getGenerator(i)._broken){
            brokenGenerator = i;
            break;
        }
    }
    if (brokenGenerator == -1) {
        leaveLevel(server);
        summonCutwire2(server);
        return;
    }

    Position playerPos = objects._playerPos;
    moveToPositionCutwire2(server, playerPos, objects._generators[brokenGenerator], false);
    server->sendAction(interact);
    Sleep(2*ONEFRAMEMS);
    server->sendAction(noop);
    Sleep(2*ONEFRAMEMS);
    Sleep(100);
    server->getGameState(gameState);
    int componentWanted = gameState.getGenerator(brokenGenerator)._want1;
    if (componentWanted == -1) {
        leaveLevel(server);
        summonCutwire2(server);
        return;
    }

    moveToPositionCutwire2(server, playerPos, objects._components[componentWanted], false);
    server->sendAction(interact);
    Sleep(2*ONEFRAMEMS);
    server->sendAction(noop);
    Sleep(2*ONEFRAMEMS);

    moveToPositionCutwire2(server, playerPos, objects._generators[brokenGenerator], false);
    server->sendAction(interact);
    Sleep(2*ONEFRAMEMS);
    server->sendAction(noop);
    Sleep(100);
    server->getGameState(gameState);
    if (gameState.getGenerator(brokenGenerator)._broken) {
        leaveLevel(server);
        summonCutwire2(server);
        return;
    }
        
    Sleep(3000);
}

void actionMacros::summonCutwire1(InterfaceServer* server) {
    enterSpecificLevel(server, 3);
    server->sendAction(moveNorthwest);
    Sleep(600);
    server->sendAction(noop);
    Sleep(150);
    server->sendAction(interact);
    Sleep(2*ONEFRAMEMS);
    server->sendAction(moveSoutheast);
    Sleep(1000);
    server->sendAction(noop);
    Sleep(150);
    server->sendAction(moveEast);
    Sleep(300);
    server->sendAction(noop);
    Sleep(150);
    server->sendAction(moveNorth);
    Sleep(100);
    server->sendAction(noop);
    Sleep(150);
    server->sendAction(interact);
    Sleep(2*ONEFRAMEMS);
    server->sendAction(noop);
    Sleep(6000);
    server->sendAction(interact);
    Sleep(2*ONEFRAMEMS);
    server->sendAction(noop);
    Sleep(2*ONEFRAMEMS);
    StatePacket tempPacket;
    server->getGameState(tempPacket);
    int brokenGenerator = -1;
    for (int i = 0; i < 4; i++) {
        if (tempPacket.getGenerator(i)._broken) {
            brokenGenerator = i;
            break;
        }
    }
    if (brokenGenerator == 2) {
        server->sendAction(moveWest);
        Sleep(300);
        server->sendAction(noop);
        Sleep(150);
        server->sendAction(moveNorth);
        Sleep(1000);
    }
    else if (brokenGenerator == 1) {
        server->sendAction(moveWest);
        Sleep(300);
        server->sendAction(noop);
        Sleep(150);
        server->sendAction(moveNorth);
        Sleep(200);
        server->sendAction(noop);
        Sleep(150);
        server->sendAction(moveWest);
        Sleep(1200);
    }
    else
        MyExcept::error("Invalid generator");
    server->sendAction(noop);
    Sleep(150);
    server->sendAction(interact);
    Sleep(2*ONEFRAMEMS);
    server->sendAction(noop);
    Sleep(2*ONEFRAMEMS);
    server->sendAction(interact);
    Sleep(2*ONEFRAMEMS);
    Sleep(100);
    StatePacket gameState;
    server->getGameState(gameState);
    if (gameState.getGenerator(brokenGenerator)._broken) {
        waitForIntro(server);
        leaveLevel(server);
        summonCutwire1(server);
        return;
    }
    waitForIntro(server);
}

void actionMacros::summonStriga(InterfaceServer* server) {
    enterSpecificLevel(server, 11);
    server->sendAction(moveEast);
    Sleep(2 * ONEFRAMEMS);
    server->sendAction(noop);
    Sleep(150);
    server->sendAction(moveWest);
    Sleep(2 * ONEFRAMEMS);
    server->sendAction(noop);
    Sleep(150);
    waitForIntro(server);
}

void actionMacros::summonBull(InterfaceServer* server) {
    enterSpecificLevel(server, 12);
    breakGenerator(server);
    Sleep(2500);
}

void actionMacros::pickupGearLevel11(InterfaceServer* server) {
    breakGenerator(server);
    waitForIntro(server);
    server->sendAction(moveWest);
    Sleep(400);
    server->sendAction(noop);
    Sleep(200);
    server->sendAction(moveSouth);
    Sleep(550);
    server->sendAction(interact);
    Sleep(2 * ONEFRAMEMS);
    server->sendAction(noop);
}
void actionMacros::pickupPipeLevel11(InterfaceServer* server) {
    breakGenerator(server);
    waitForIntro(server);
    server->sendAction(moveEast);
    Sleep(300);
    server->sendAction(noop);
    Sleep(200);
    server->sendAction(interact);
    Sleep(2 * ONEFRAMEMS);
    server->sendAction(noop);
}
void actionMacros::pickupRedBatteryLevel11(InterfaceServer* server) {
    breakGenerator(server);
    waitForIntro(server);
    server->sendAction(moveWest);
    Sleep(300);
    server->sendAction(noop);
    Sleep(200);
    server->sendAction(moveNorth);
    Sleep(300);
    server->sendAction(interact);
    Sleep(2 * ONEFRAMEMS);
    server->sendAction(noop);
}
void actionMacros::pickupBlueBatteryLevel11(InterfaceServer* server) {
    breakGenerator(server);
    waitForIntro(server);
    server->sendAction(moveWest);
    Sleep(500);
    server->sendAction(noop);
    Sleep(200);
    server->sendAction(moveSouth);
    Sleep(700);
    server->sendAction(interact);
    Sleep(2 * ONEFRAMEMS);
    server->sendAction(noop);
}
void actionMacros::pickupBlueThingLevel11(InterfaceServer* server) {
    breakGenerator(server);
    waitForIntro(server);
    server->sendAction(moveSoutheast);
    Sleep(300);
    server->sendAction(noop);
    Sleep(200);
    server->sendAction(moveSouth);
    Sleep(600);
    server->sendAction(interact);
    Sleep(2 * ONEFRAMEMS);
    server->sendAction(noop);
}