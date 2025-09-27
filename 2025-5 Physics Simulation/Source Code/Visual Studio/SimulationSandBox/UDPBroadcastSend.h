#pragma once

#include <winsock2.h>
#include "definitions.h"
#pragma comment(lib, "ws2_32.lib") 

namespace helpers {
	enum peerColor;
}

class UDPBroadcastSend final {
	SOCKET _mySocket = INVALID_SOCKET;
	int _targetPort = LANBROADRECEIVE;
	const char* _targetIP = LABIP;
	sockaddr_in _broadcastAddr;
public:
	// LAN
	UDPBroadcastSend();
	// One computer
	UDPBroadcastSend(helpers::peerColor color);
	~UDPBroadcastSend() { closesocket(_mySocket); }

	void broadcast(const char* message) const;
};