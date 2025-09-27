#pragma once

#include <winsock2.h>
#include "inaddr.h"
#include <string>
#pragma comment(lib, "ws2_32.lib") 

namespace helpers {
	enum peerColor;
}

class UDPBroadcastReceive final
{
	SOCKET _mySocket = INVALID_SOCKET;
	std::vector<IN_ADDR> _myIPs;
	char _buffer[4096] = {};

	bool isMyAddress(sockaddr_in& addr);
public:
	UDPBroadcastReceive();
	~UDPBroadcastReceive() { closesocket(_mySocket); }
	std::string myReceive(sockaddr_in& senderAddr);
};

