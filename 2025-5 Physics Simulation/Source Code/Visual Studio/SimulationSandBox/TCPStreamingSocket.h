#pragma once

#include <winsock2.h>
#include "flatbuffers/flatbuffers.h"
#pragma comment(lib, "ws2_32.lib") 
namespace net {
	struct GameObject;
}

class TCPStreamingSocket
{
	SOCKET _mySocket = INVALID_SOCKET;
public:
	TCPStreamingSocket();
	// For listening socket
	TCPStreamingSocket(SOCKET& socket);
	~TCPStreamingSocket() { closesocket(_mySocket); }

	// Returns true on successful connection
	bool connectToPeer(const sockaddr_in& addr ) const;
	int getPeerName() const;
	int getMyName() const;
	bool testMe() const;
#ifndef NOT_DISTRIBUTED
	void sendGameObject(const flatbuffers::FlatBufferBuilder& builder) const;
	void sendImGuiState(const flatbuffers::FlatBufferBuilder& builder) const;
	void clearRecvBuffer();

	bool sendMagicNumber() const;
	bool waitForMagicNumber() const;

	bool waitRecv(std::vector<uint8_t>&) const;
	bool recvInt(int&);
#endif
};

