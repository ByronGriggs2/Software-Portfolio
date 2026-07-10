#include "ActiveSocket.h"
#include "Active.h"

bool ActiveSocket::getSocketData(FSocketData& out) const {
	auto setDefault = [&out]() {
		out._isFilled = false;
		out._runtimeData = nullptr;
		out._level = -1;
		};
	if (!isValid()) {
		LOGERROR("ActiveSocket::getSocketData - socket is invalid");
		setDefault();
		return false;
	}
	if (!isOccupied()) {
		setDefault();
		return true;
	}
	auto active = _active.lock();
	if (active == nullptr) {
		LOGERROR("ActiveSocket::getSocketData - socket is occupied and valid but active is null");
		setDefault();
		return false;
	}
	out._isFilled = true;
	// A copy is made to convert from const (C++ only) to non-const (blueprint compatible)
	out._runtimeData = DuplicateObject<UWeaponTemplate>(active->getRuntimeTemplate(), nullptr);
	out._level = active->getLevel();
	if (!out._runtimeData.IsValid())
		LOGWARNING("ActiveSocket::getSocketData - invalid icon");
	return true;
}