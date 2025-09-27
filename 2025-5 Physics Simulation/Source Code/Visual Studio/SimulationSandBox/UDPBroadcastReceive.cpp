#include "pch.h"
#include "UDPBroadcastReceive.h"
#include "WS2tcpip.h"

using namespace helpers;

UDPBroadcastReceive::UDPBroadcastReceive() {
	// Create transfer socket
	_mySocket = socket(AF_INET, SOCK_DGRAM, 0);
	testSocket(_mySocket);
	// Allow address reuse
	BOOL reuseAddr = TRUE;
	testSocket(setsockopt(_mySocket, SOL_SOCKET, SO_REUSEADDR, (char*)&reuseAddr, sizeof(reuseAddr)));
	// Make socket non blocking
	u_long mode = 1;
	ioctlsocket(_mySocket, FIONBIO, &mode);
	// Enable broadcast receive
	BOOL enableBroadcast = TRUE;
	testSocket(setsockopt(_mySocket, SOL_SOCKET, SO_BROADCAST, (char*)&enableBroadcast, sizeof(enableBroadcast)));
	// Bind the socket to the broadcast port
	sockaddr_in recvAddr{};
	recvAddr.sin_family = AF_INET;
	recvAddr.sin_port = htons(LANBROADRECEIVE);
	recvAddr.sin_addr.s_addr = INADDR_ANY;
	testSocket(bind(_mySocket, (sockaddr*)&recvAddr, sizeof(recvAddr)));
	// Copilot block
	{
		// Get my IP address
		char hostname[256];
		testSocket(gethostname(hostname, sizeof(hostname)));
		// Resolve the hostname to get the IP addresses
		addrinfo hints = {};
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		addrinfo* result = nullptr;
		testSocket(getaddrinfo(hostname, nullptr, &hints, &result) != 0);

		// Transfer raw pointer 'result' to a unique_ptr with a custom deleter
		std::unique_ptr<addrinfo, decltype(&freeaddrinfo)> resultPtr(result, freeaddrinfo);

#ifdef _DEBUG
		std::cout << "Broadcast receive created with IP addresses ";
#endif
		// Iterate through the results and store the IP addresses
		for (addrinfo* ptr = resultPtr.get(); ptr != nullptr; ptr = ptr->ai_next) {
			if (ptr->ai_family == AF_INET) { // IPv4
				sockaddr_in* ipv4 = reinterpret_cast<sockaddr_in*>(ptr->ai_addr);
				_myIPs.push_back(ipv4->sin_addr);
#ifdef _DEBUG
				char ipString[INET6_ADDRSTRLEN] = {};
				inet_ntop(AF_INET, &ipv4->sin_addr, ipString, sizeof(ipString));
				std::cout << ipString << " ";
#endif;
			}
		}
#ifdef _DEBUG
		std::cout << std::endl;
#endif
	}
}

std::string UDPBroadcastReceive::myReceive(sockaddr_in& senderAddr) {
	testSocket(_mySocket);
	int senderAddrSize = sizeof(senderAddr);

	// Repeatedly call receive until there is no data in the buffer or there is data from a different IP
	int bytesReceived = -1;
	do {
		 bytesReceived = recvfrom(_mySocket, _buffer, sizeof(_buffer) - 1, 0,
			(sockaddr*)&senderAddr, &senderAddrSize);
	} while (isMyAddress(senderAddr) && bytesReceived != -1);

	if (bytesReceived == -1)
		return std::string(1, '\0');

#ifdef VERBOSE
	char ipString[INET_ADDRSTRLEN] = {};
	inet_ntop(AF_INET, &(senderAddr.sin_addr), ipString, sizeof(ipString));
	std::cout << "Received from: " << ipString << " Bytes: " << bytesReceived << std::endl;
#endif
	return std::string(_buffer, bytesReceived);
}

bool UDPBroadcastReceive::isMyAddress(sockaddr_in& addr) {
	for (auto& myAddress : _myIPs) {
		if (myAddress.S_un.S_addr == addr.sin_addr.S_un.S_addr) {
			return true;
		}
	}
	return false;
}