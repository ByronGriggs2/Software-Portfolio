#include "pch.h"
#include "TCPStreamingSocket.h"

using namespace helpers;

TCPStreamingSocket::TCPStreamingSocket(SOCKET& socket) : _mySocket(socket) {
	testSocket(_mySocket);
}
TCPStreamingSocket::TCPStreamingSocket() {
	// Create socket
	_mySocket = socket(AF_INET, SOCK_STREAM, 0);
	testSocket(_mySocket);
	// Set blocking time to 150 ms
	DWORD _timeout = 150;
	testSocket(setsockopt(_mySocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&_timeout, sizeof(_timeout)));
	// Allow address reuse
	BOOL reuseAddr = TRUE;
	testSocket(setsockopt(_mySocket, SOL_SOCKET, SO_REUSEADDR, (char*)&reuseAddr, sizeof(reuseAddr)));
}

// Used to assign colors
int TCPStreamingSocket::getPeerName() const {
	sockaddr_in peerAddr;
	int addrLen = sizeof(peerAddr);
	testSocket(getpeername(_mySocket, (sockaddr*)&peerAddr, &addrLen));
	return ntohl(peerAddr.sin_addr.s_addr);
}
int TCPStreamingSocket::getMyName() const {
	sockaddr_in myAddr;
	int addrLen = sizeof(myAddr);
	testSocket(getsockname(_mySocket, (sockaddr*)&myAddr, &addrLen));
	return ntohl(myAddr.sin_addr.s_addr);
}

bool TCPStreamingSocket::connectToPeer(const sockaddr_in& addr) const {
	int result = connect(_mySocket, (sockaddr*)&addr, sizeof(addr));
	if (result == 0) {
		// Connected immediately
		return true;
	}
	// ChatGPT wrote below this
	// Wait for writability using select
	fd_set writeSet;
	FD_ZERO(&writeSet);
	FD_SET(_mySocket, &writeSet);

	timeval timeout;
	timeout.tv_sec = 0;
	// Wait for 150 ms before timeout
	timeout.tv_usec = 150000;

	result = select(0, nullptr, &writeSet, nullptr, &timeout);
	if (result <= 0) {
		// Timeout or error
		return false;
	}

	// Check if the socket is truly connected
	int sockErr = 0;
	int len = sizeof(sockErr);
	if (getsockopt(_mySocket, SOL_SOCKET, SO_ERROR, (char*)&sockErr, &len) == SOCKET_ERROR || sockErr != 0) {
		return false;
	}

	return true;
}

bool TCPStreamingSocket::testMe() const {
	testSocket(_mySocket);
	return true;
}

#ifndef NOT_DISTRIBUTED
void TCPStreamingSocket::clearRecvBuffer() {
	char buffer[4096];
	u_long mode = 1;
	ioctlsocket(_mySocket, FIONBIO, &mode);
	while (true) {
		const int bytesReceived = recv(_mySocket, buffer, sizeof(buffer), 0);
		if (bytesReceived == -1) {
			if (WSAGetLastError() == WSAEWOULDBLOCK)
				break;
#ifdef _DEBUG
			myExcept("Recv error while trying to clear buffer");
#endif
		}
	}
	mode = 0;
	ioctlsocket(_mySocket, FIONBIO, &mode);
}

void TCPStreamingSocket::sendGameObject(const flatbuffers::FlatBufferBuilder& builder) const {
	uint8_t* buf = builder.GetBufferPointer();
	const size_t size = builder.GetSize();
	send(_mySocket, reinterpret_cast<const char*>(buf), static_cast<int>(size), 0);
}
void TCPStreamingSocket::sendImGuiState(const flatbuffers::FlatBufferBuilder& builder) const {
	uint8_t* buf = builder.GetBufferPointer();
	const size_t size = builder.GetSize();
	send(_mySocket, reinterpret_cast<const char*>(buf), static_cast<int>(size), 0);
}

bool TCPStreamingSocket::recvInt(int& myInt) {
	char buf[sizeof(int)];
	const int bytesReceived = recv(_mySocket, buf, sizeof(buf), 0);
	if (bytesReceived == SOCKET_ERROR) {
		if (WSAGetLastError() == WSAEWOULDBLOCK || WSAGetLastError() == WSAETIMEDOUT) {
#ifdef VERBOSE
			std::cout << "int receive timed out: " << std::to_string(WSAGetLastError()) << std::endl;
#endif
		}
#ifdef _DEBUG
		else
			throw MyException("Error on receiving int");
#endif
		return false;
	}
	myInt = *reinterpret_cast<int*>(buf);
	return true;
}

bool TCPStreamingSocket::waitRecv(std::vector<uint8_t>& buf) const {
	const int expectedSize = static_cast<int>(buf.size());
	int bytesReceived = 0;
	while (bytesReceived < expectedSize) {
		const int tempBytes = recv(_mySocket, reinterpret_cast<char*>(buf.data()+bytesReceived), expectedSize, 0);
		if (tempBytes == -1) {
			if (WSAGetLastError() == WSAEWOULDBLOCK || WSAGetLastError() == WSAETIMEDOUT) {
				return false;
			}
#ifdef _DEBUG
			else {
				myExcept("Error on receiving ImGui data: " + std::to_string(WSAGetLastError()));
				return false;
			}
#endif
		}
		bytesReceived += tempBytes;
	}
	return true;
}

bool TCPStreamingSocket::sendMagicNumber() const {
	const int magicNumber = 0xFEABBAAD;
	return send(_mySocket, reinterpret_cast<const char*>(&magicNumber), sizeof(magicNumber), 0) != SOCKET_ERROR;
}

bool TCPStreamingSocket::waitForMagicNumber() const {
	const int magicNumber = 0xFEABBAAD;
	while (true) {
		char tempBuf[4] = { '\0','\0','\0','\0' };
		const int bytesReceived = recv(_mySocket, tempBuf, sizeof(magicNumber), 0);
		if (bytesReceived == SOCKET_ERROR) {
			if (WSAGetLastError() == WSAEWOULDBLOCK || WSAGetLastError() == WSAETIMEDOUT) {
				return false;
			}
#ifdef _DEBUG
			else
				myExcept("Error on receiving magic number: " + std::to_string(WSAGetLastError()));
#endif
		}
		const int receivedMagicNumber = (static_cast<unsigned char>(tempBuf[3]) << 24) |
			(static_cast<unsigned char>(tempBuf[2]) << 16) |
			(static_cast<unsigned char>(tempBuf[1]) << 8) |
			static_cast<unsigned char>(tempBuf[0]);
		if (receivedMagicNumber == magicNumber)
			return true;
	}
}
#endif
