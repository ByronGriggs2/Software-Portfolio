#include "WeaponContainerUnreal.h"
#include "Definitions.h"

void USword::initialise_USword() {
	_container = std::make_unique<Sword>(SOCKETS_PER_CONTAINER);
}
void UGun::initialise_UGun() {
	_container = std::make_unique<Gun>(SOCKETS_PER_CONTAINER);
}

void UWeaponContainer::BeginPlay() {
	Super::BeginPlay();
	if (_container == nullptr) {
		LOGERROR("UWeaponContainerUnreal::BeginPlay - _container is null");
		return;
	}
}

bool UWeaponContainer::isFull() {
	if (_container == nullptr) {
		LOGWARNING("UWeaponContainer::isFull - container is null");
		return true;
	}
	for (const auto& slot : _container->_sockets)
		if (!slot.isOccupied())
			return false;
	return true;
}