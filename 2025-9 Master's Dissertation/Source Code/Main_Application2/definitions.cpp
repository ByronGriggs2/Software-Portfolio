#include "pch.h"
#include "definitions.h"

std::string helpers::componentToStr(component comp) {
	switch (comp) {
	case none:
		return "none";
		break;
	case gear:
		return "gear";
		break;
	case pipe:
		return "pipe";
		break;
	case redBattery:
		return "red battery";
		break;
	case blueBattery:
		return "blue battery";
		break;
	case blueThing:
		return "blue thing";
		break;
	case repaired:
		return "repaired";
		break;
	default:
		return "Invalid component index";
		break;
	}
}

std::string helpers::statusToStr(machineStatus stat) {
	switch (stat) {
	case empty:
		return "empty";
		break;
	case working:
		return "working";
		break;
	case ready:
		return "ready";
		break;
	default:
		return "Invalid machine status";
		break;
	}
}