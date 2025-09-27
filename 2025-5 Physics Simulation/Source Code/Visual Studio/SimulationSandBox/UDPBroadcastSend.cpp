#include "pch.h"
#include "UDPBroadcastSend.h"
#include <WS2tcpip.h>

using namespace helpers;

UDPBroadcastSend::UDPBroadcastSend() {
	// Create transfer socket
	_mySocket = socket(AF_INET, SOCK_DGRAM, 0);
	testSocket(_mySocket);
	// Enable broadcast option
	BOOL broadcastEnable = TRUE;
	testSocket(setsockopt(_mySocket, SOL_SOCKET, SO_BROADCAST, (char*)&broadcastEnable, sizeof(broadcastEnable)));
	// Allow address reuse
	BOOL reuseAddr = TRUE;
	testSocket(setsockopt(_mySocket, SOL_SOCKET, SO_REUSEADDR, (char*)&reuseAddr, sizeof(reuseAddr)));
	// let os choose port and bind
	;
	// Set target address
	_broadcastAddr.sin_family = AF_INET;
	_broadcastAddr.sin_port = htons(LANBROADRECEIVE);
	testSocket(inet_pton(_broadcastAddr.sin_family, _targetIP, &_broadcastAddr.sin_addr));
}

// Should not block from programmer's perspective (still does at os level)
void UDPBroadcastSend::broadcast(const char* message) const {
	testSocket(_mySocket);
	testSocket(sendto(_mySocket, message, static_cast<int>(strlen(message)), 0, (sockaddr*)&_broadcastAddr, sizeof(_broadcastAddr)));
}