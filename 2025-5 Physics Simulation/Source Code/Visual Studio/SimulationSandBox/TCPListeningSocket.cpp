#include "pch.h"
#include "TCPListeningSocket.h"
#include "TCPStreamingSocket.h"

using namespace helpers;

// LAN
TCPListeningSocket::TCPListeningSocket() : _s(INVALID_SOCKET) {
	sockaddr_in peer;
	peer.sin_family = AF_INET;
	peer.sin_port = htons(LANLISTENING);
	peer.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	// Create
	_s = socket(AF_INET, SOCK_STREAM, 0);
	testSocket(_s);
	// Make socket non blocking, so that accept does not block
	u_long mode = 1;
	ioctlsocket(_s, FIONBIO, &mode);
	// Allow address reuse
	BOOL reuseAddr = TRUE;
	testSocket(setsockopt(_s, SOL_SOCKET, SO_REUSEADDR, (char*)&reuseAddr, sizeof(reuseAddr)));
	// Bind
	testSocket(bind(_s, (sockaddr*)&peer, sizeof(peer)));
	// Initialize listening
	testSocket(listen(_s, 5) == SOCKET_ERROR);
}

std::unique_ptr<TCPStreamingSocket> TCPListeningSocket::myAccept() {
	SOCKET s1 = accept(_s, NULL, NULL);
	
	if (s1 == INVALID_SOCKET) {
		// No pending connection requests
		if (WSAGetLastError() == WSAEWOULDBLOCK)
			return nullptr;
		// Some other error
#ifdef _DEBUG
		else
			throw MyException("Error on accepting peer connection");
#endif
	}
	return std::make_unique<TCPStreamingSocket>(s1);
}