#include "pch.h"
#include "peerColor.h"
#include <string>

using namespace helpers;

peerColor helpers::stringToColor(const char* str) {
	if (strcmp(str, "red") == 0)
		return red;
	if (strcmp(str, "yellow") == 0)
		return yellow;
	if (strcmp(str, "green") == 0)
		return green;
	if (strcmp(str, "blue") == 0)
		return blue;
	if (strcmp(str, "undef") == 0)
		return undef;
	//std::cout << str << std::endl;
	return undef;
}