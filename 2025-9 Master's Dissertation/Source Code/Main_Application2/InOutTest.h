#pragma once
#include "StatePacket.h"
#include "windows.h"
class InterfaceServer;

class InOutTest final {
	// Owned by Control
	InterfaceServer* _server;
	StatePacket _gameState;
	bool _testFailed = false;
	
	void subtest1();
	void subtest2();
	void subtest2_2();
	void subtest3(bool fullTest = true);
	void subtest4();
	void subtest5();
	void subtest6();
	void subtest7();
	void subtest8();
	void subtestX(LARGE_INTEGER&, LARGE_INTEGER&);
	void outputGeneratorStatus(const StatePacket&, int generator) const;

public:
	InOutTest() = delete;
	InOutTest(InterfaceServer* serv) : _server(serv), _gameState() {}
	bool test1Funct();
};