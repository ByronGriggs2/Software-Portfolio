#include "pch.h"
#include "AutoRecoveryTest.h"

std::string AutoRecoveryTest::test() {
	if (_server == nullptr)
		return "Server is nullptr";
	_server->defaultStartup();

	constexpr int testCount = 6;
	constexpr DWORD testDuration = 5000;

	if (!_server->newData())
		return "Server does not have new data";

	for (int i = 0; i < testCount; i++) {
		Sleep(testDuration);
		if (!_server->newData())
			return std::string("Server does not have new data, iteration ") + static_cast<char>('0' + i);
	}
	return "OK";
}