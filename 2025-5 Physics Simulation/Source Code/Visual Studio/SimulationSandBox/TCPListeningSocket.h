#pragma once

#include <memory>
#pragma comment(lib, "ws2_32.lib") 

namespace helpers {
	enum peerColor;
}
class TCPStreamingSocket;

class TCPListeningSocket
{
	SOCKET _s;

public:
	TCPListeningSocket();
	~TCPListeningSocket() { closesocket(_s); }

	std::unique_ptr<TCPStreamingSocket> myAccept();
};

