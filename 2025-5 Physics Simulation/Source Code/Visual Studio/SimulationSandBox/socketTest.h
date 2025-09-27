#pragma once

#include <winsock2.h>
#include "NotImplementedException.h"
#include <iostream>

namespace helpers {
	inline void testSocket(int test) {
#ifdef _DEBUG
		if (test == SOCKET_ERROR) {
			std::cout << WSAGetLastError();
			throw MyException("Socket error");
		}
#endif
	}
	inline void testSocket(SOCKET socket) {
#ifdef _DEBUG
		if (socket == INVALID_SOCKET)
			throw MyException("Invalid socket");
#endif
	}
}