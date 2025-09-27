#pragma once
#include "StatePacket.h"
class InterfaceServer;
struct Position;
namespace actionMacros {
	void enterLevel(InterfaceServer* server);
	void enterSpecificLevel(InterfaceServer* server, int levelToEnter);
	void leaveLevel(InterfaceServer* server);
	void leaveLevelRobust(InterfaceServer* server);
	void enterLevelSelectFromWin(InterfaceServer* server, int currentLevel);
	void exitGame(InterfaceServer* server);
	void infiniteMovementLoop(InterfaceServer* server);
	void moveToCenter(InterfaceServer* server, float xOffset = 0.0f, float yOffset = 0.0f);
	int breakGenerator(InterfaceServer* server);
	
	void summonZmora1(InterfaceServer* server);
	void summonZmora2(InterfaceServer* server);
	void summonZmora3(InterfaceServer* server);
	void summonCutwire1(InterfaceServer* server);
	void summonCutwire2(InterfaceServer* server);
	void summonNikita(InterfaceServer* server);
	void summonStriga(InterfaceServer* server);
	void summonBull(InterfaceServer* server);

	void waitForIntro(InterfaceServer* server);
	void moveToPositionZmora3(InterfaceServer* server, Position& currentPos, const Position& goalPos);
	void moveToPositionCutwire2(InterfaceServer* server, Position& currentPos, const Position& goalPos, bool);
	void checkGeneratorZmora3(InterfaceServer* server, int generator);
	void repairGeneratorZmora3(InterfaceServer* server, int generator, int want);

	void pickupPipeLevel11(InterfaceServer* server);
	void pickupGearLevel11(InterfaceServer* server);
	void pickupBlueBatteryLevel11(InterfaceServer* server);
	void pickupBlueThingLevel11(InterfaceServer* server);
	void pickupRedBatteryLevel11(InterfaceServer* server);
}