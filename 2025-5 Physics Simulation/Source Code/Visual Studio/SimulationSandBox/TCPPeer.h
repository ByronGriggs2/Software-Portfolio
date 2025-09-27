#pragma once

#include <memory>
#include <vector>
#include <string>
#include "TCPListeningSocket.h"
#include "TCPStreamingSocket.h"
#include "SharedMemory.h"

struct NetCompatileGuiState;

namespace helpers {
	enum peerColor;
}

class TCPPeer
{
	struct PeerConnection {
		helpers::peerColor _color;
		int _IP = 0;
		std::unique_ptr<TCPStreamingSocket> _socket = nullptr;
	};

	std::unique_ptr<TCPListeningSocket> _listen = nullptr;
	std::vector<PeerConnection> _streams;
	helpers::peerColor _myColor;
	bool _fullyConnected = false;

	bool hasConnection(int IP) {
		for (auto& stream : _streams) {
			if (stream._IP == IP)
				return true;
		}
		return false;
	}
	bool hasConnection(const sockaddr_in& addr) const {
		int IP = ntohl(addr.sin_addr.s_addr);
		for (auto& stream : _streams) {
			if (stream._IP == IP)
				return true;
		}
		return false;
	}
public:
	TCPPeer();
	// Network initialization
	bool acceptConnectionRequestIfNotConnected();
	bool sendConnectionRequestIfNotConnected(const sockaddr_in& addr);
	void assignColors();
	bool fullyConnected() const { return _fullyConnected; }
	helpers::peerColor getColor() const { return _myColor; }

#ifndef NOT_DISTRIBUTED
	void sendObjectData(const NetCompatibleGameObject& dataToSend, bool newData, helpers::peerColor color) const;
	bool connectionAlive(helpers::peerColor color) const;
	void sendImGuiState(const NetCompatibleImGuiState&, helpers::peerColor) const;
	void clearRecvBuffers();
	int recvType(helpers::peerColor color) const;
	int recvSize(helpers::peerColor color) const;
	void recvFB(std::vector<uint8_t>& buf, helpers::peerColor color) const;
#endif
};

