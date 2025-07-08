#pragma once
#include <iostream>
namespace helpers {
	inline void clearConsole() {
#ifdef TEST
		return;
#endif
#ifdef _WIN32
		system("CLS");
#else
		system("clear");
#endif
	}
}