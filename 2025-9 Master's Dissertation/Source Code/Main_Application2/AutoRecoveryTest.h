#pragma once
#include "InterfaceServer.h"
class AutoRecoveryTest final
{
	InterfaceServer* _server;

public:
	AutoRecoveryTest(InterfaceServer* server) : _server(server) {}
	AutoRecoveryTest() = delete;
	std::string test();
};

