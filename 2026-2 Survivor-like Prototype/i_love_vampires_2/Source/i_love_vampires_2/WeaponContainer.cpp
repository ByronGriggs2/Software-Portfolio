#include "WeaponContainer.h"
#include "Definitions.h"
#include "WeaponContainerUnreal.h"
#include "ActiveSocket.h"

bool WeaponContainer::slotValid(int index) const {
	if (index < 0 || index >= _sockets.size()) {
		LOGERROR("WeaponContainer::addWeapon - index out of bounds");
		return false;
	}
	// During destruction
	if (_sockets[index].isOccupied() && !_sockets[index].isValid())
		return false;
	return true;
}

int WeaponContainer::addWeapon(std::shared_ptr<Active> active, int index) {
	if (!slotValid(index))
		return -1;
	if (_sockets[index].isOccupied())
		return 1;
	_sockets[index].setActive(active);
	return 0;
}

int WeaponContainer::replaceWeapon(std::shared_ptr<Active> active, int index) {
	if (!slotValid(index))
		return -1;
	if (!_sockets[index].isOccupied())
	{
		LOGERROR("WeaponContainer::replaceWeapon - attempting to replace weapon in unoccupied slot");
		return -1;
	}
	_sockets[index].setActive(active);
	return 0;
}

WeaponContainer::WeaponContainer(int socketCount) : _sockets(socketCount, ActiveSocket()) {}

bool WeaponContainer::getSocketData(TArray<FSocketData>& out) const {
	out.Empty();
	bool success = true;
	for (const auto& socket : _sockets) {
		FSocketData data;
		if (!socket.getSocketData(data)) {
			LOGERROR("WeaponContainer::getSlotData - failed to get slot data for a socket");
			success = false;
		}
		out.Add(data);
	}
	return success;
}

bool WeaponContainer::hasEmptySocket() const {
	return getFirstEmptySocket() != -1;
}

int WeaponContainer::getFirstEmptySocket() const {
	for (int i = 0; i < _sockets.size(); i++) {
		if (_sockets[i].isValid() && !_sockets[i].isOccupied())
			return i;
	}
	return -1;
}

int WeaponContainer::getIndexOfActive(std::shared_ptr<Active> active) const {
	for (int i = 0; i < _sockets.size(); i++) {
		if (_sockets[i].getActive() == active)
			return i;
	}
	return -1;
}