#include "pch.h"
#include "InterfaceServer.h"
#include "definitions.h"
#include "exceptions.h"

using namespace helpers;
int InterfaceServer::initialize() {
#ifdef _DEBUG
	static_assert(sizeof(InputsPacket) == 11, "InputsPacket must be 11 bytes");
#endif
	// Excplitly set security policy
	//SECURITY_DESCRIPTOR sd;
	//InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
	//SetSecurityDescriptorDacl(&sd, TRUE, NULL, FALSE);
	//SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES) };
	//sa.lpSecurityDescriptor = &sd;
	//sa.bInheritHandle = FALSE;

	LPCWSTR pipeName = L"\\\\.\\pipe\\AwariaControlPipe";
	_pipe = CreateNamedPipeW(
		pipeName,
		PIPE_ACCESS_OUTBOUND, // transmit only
		PIPE_TYPE_BYTE |
		PIPE_READMODE_BYTE | // ignored
		PIPE_WAIT, // blocking
		1, // 1 instance
		sizeof(InputsPacket),
		sizeof(InputsPacket), // ignored
		0, // 50 ms
		nullptr
	);
	if (_pipe == INVALID_HANDLE_VALUE)
		return -1;

	std::cout << "Waiting for client to connect" << std::endl;
	BOOL connected = ConnectNamedPipe(_pipe, nullptr) ? TRUE : GetLastError() == ERROR_PIPE_CONNECTED;
	if (!connected) {
		CloseHandle(_pipe);
		return -1;
	}
	std::cout << "Client connected" << std::endl;
	return 0;
}

void InterfaceServer::messageLoop() {
	testInputs();
}

void InterfaceServer::testInputs() {
	static int callCount = 0;
	if (callCount == 0) {
		Sleep(3000);
	}
	else if (callCount < 4) {
		sendAction(interact);
		Sleep(500);
	}
	else {
		sendAction(moveWest);
		Sleep(250);
		sendAction(moveNorth);
		Sleep(250);
		sendAction(moveEast);
		Sleep(250);
		sendAction(moveSouth);
		Sleep(250);
	}
	callCount++;
}

void InterfaceServer::sendAction(playerAction action) {
	InputsPacket currentPacket;
	switch (action) {
	case noop:
		break;
	case moveNorth:
		currentPacket._y = 0.5f;
		break;
	case moveNortheast:
		currentPacket._x = 0.5f;
		currentPacket._y = 0.5f;
		break;
	case moveEast:
		currentPacket._x = 0.5f;
		break;
	case moveSoutheast:
		currentPacket._x = 0.5f;
		currentPacket._y = -0.5f;
		break;
	case moveSouth:
		currentPacket._y = -0.5f;
		break;
	case moveSouthwest:
		currentPacket._x = -0.5f;
		currentPacket._y = -0.5f;
		break;
	case moveWest:
		currentPacket._x = -0.5f;
		break;
	case moveNorthwest:
		currentPacket._x = -0.5f;
		currentPacket._y = 0.5f;
		break;
	case dash:
		currentPacket._secondary = 1;
		break;
	case interact:
		currentPacket._primary = 1;
		break;
#ifdef _DEBUG
	default:
		throw NotImplementedException();
		break;
	}
#endif
	sendPacket(currentPacket);
}

void InterfaceServer::sendPacket(const InputsPacket& packet) {
	DWORD bytesWritten = 0;
	BOOL ok = WriteFile(
		_pipe,
		&packet,
		sizeof(packet),
		&bytesWritten,
		nullptr
	);
#ifdef _DEBUG
	if (!ok || bytesWritten != sizeof(packet))
		throw ShouldNotRunException();
#endif
}