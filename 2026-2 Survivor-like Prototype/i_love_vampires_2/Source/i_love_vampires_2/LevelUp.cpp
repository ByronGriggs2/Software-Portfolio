#include "LevelUp.h"
#include "Definitions.h"
#include "MyPlayer.h"
#include "Active.h"
#include "WeaponContainerUnreal.h"
#include "WeaponContainer.h"
#include "ActiveSocket.h"
#include <algorithm>

bool ULevelUp_weaponSelect::initialise_ULevelUp_weaponSelect(UWeaponTemplate* weapon, AMyPlayer* player) {
	if (!registerPlayer(player))
		return false;
	return swapToNewWeapon(weapon);
}

bool ULevelUp_weaponSelect::isReady() {
	if (!playerRegistered()) {
		LOGERROR("ULevelUp_weaponSelect::isReady - player not registered");
		return false;
	}
	if (!IsValid(_selectedWeapon)) {
		LOGERROR("ULevelUp_weaponSelect::isReady - selected weapon not set");
		return false;
	}
	if (!_validSocketData) {
		LOGERROR("ULevelUp_weaponSelect::isReady - invalid socket data");
		return false;
	}
	if (_slotIndex == -1) {
		LOGERROR("ULevelUp_weaponSelect::isReady - no empty slots");
		return false;
	}
	return true;
}

const UWeaponContainer* ULevelUp_weaponSelect::getContainer(int index) const {
	if (!playerRegistered()) {
		LOGERROR("ULevelUp_weaponSelect::getContainer - player not registered");
		return nullptr;
	}
	if (index < 0 || index > 1) {
		LOGERROR("ULevelUp_weaponSelect::getContainer - invalid index");
		return nullptr;
	}
	return _player->getWeaponContainer(index);
}

bool ULevelUp_weaponSelect::calculateSocketData(TArray<FSocketData>& leftContainer, TArray<FSocketData>& rightContainer) const {
	leftContainer.Empty();
	rightContainer.Empty();
	const auto Ucon1 = getContainer(0);
	const auto Ucon2 = getContainer(1);
	if (!IsValid(Ucon1) || !IsValid(Ucon2)) {
		LOGERROR("ULevelUp_weaponSelect::getSlotData - invalid weapon container");
		return false;
	}
	const auto con1 = Ucon1->getContainer();
	const auto con2 = Ucon2->getContainer();
	if (con1 == nullptr || con2 == nullptr) {
		LOGERROR("ULevelUp_weaponSelect::getSlotData - invalid weapon container");
		return false;
	}
	if (!con1->getSocketData(leftContainer) || !con2->getSocketData(rightContainer)) {
		LOGERROR("ULevelUp_weaponSelect::getSlotData - failed to get slot data");
		return false;
	}
	return true;
}

void ULevelUp_weaponSelect::getSocketData(TArray<FSocketData>& leftContainer, TArray<FSocketData>& rightContainer) const {
	leftContainer = _left;
	rightContainer = _right;
}

std::vector<int> ULevelUp_weaponSelect::getEmptySlots() const {
	if (!containersAreValid())
		return {};

	std::vector<int> emptySlots;

	// weapon slots incompatible with weapon provided at construction are excluded
	int current = 0;
	if (_selectedWeapon->_weaponType == getContainer(0)->getContainer()->getType()) {
		for (const auto& socket : _left) {
			if (!socket._isFilled) {
				emptySlots.push_back(current);
			}
			current++;
		}
	}
	else {
		current += _left.Num();
	}
	if (_selectedWeapon->_weaponType == getContainer(1)->getContainer()->getType()) {
		for (const auto& socket : _right) {
			if (!socket._isFilled) {
				emptySlots.push_back(current);
			}
			current++;
		}
	}
	return emptySlots;
}

bool ULevelUp_weaponSelect::cursorLeft(int& out) {
	auto emptySlots = getEmptySlots();
	if (emptySlots.empty()) {
		LOGERROR("ULevelUp_weaponSelect::cursorLeft - no empty slots");
		return false;
	}
	auto currentEmptyIndex = std::find(emptySlots.begin(), emptySlots.end(), _slotIndex);
	if (currentEmptyIndex == emptySlots.end()) {
		LOGERROR("ULevelUp_weaponSelect::cursorLeft - current slot index is not an empty slot");
		return false;
	};
	if (_slotIndex == emptySlots[0]) {
		_slotIndex = emptySlots[emptySlots.size() - 1];
	}
	else
		_slotIndex = *(--currentEmptyIndex);
	out = _slotIndex;
	return true;
}

bool ULevelUp_weaponSelect::cursorRight(int& out) {
	auto emptySlots = getEmptySlots();
	if (emptySlots.empty()) {
		LOGERROR("ULevelUp_weaponSelect::cursorRight - no empty slots");
		return false;
	}
	auto currentEmptyIndex = std::find(emptySlots.begin(), emptySlots.end(), _slotIndex);
	if (currentEmptyIndex == emptySlots.end()) {
		LOGERROR("ULevelUp_weaponSelect::cursorRight - current slot index is not an empty slot");
		return false;
	};
	if (_slotIndex == emptySlots[emptySlots.size() - 1]) {
		_slotIndex = emptySlots[0];
	}
	else
		_slotIndex = *(++currentEmptyIndex);
	out = _slotIndex;
	return true;
}
CardType ULevelUp_weaponSelect::getCardType() const {
	if (!playerRegistered()) {
		LOGERROR("ULevelUp_weaponSelect::getCardType - player not registered");
		return CardType::INVALID;
	}
	bool gun = _player->canGiveGun();
	bool sword = _player->canGiveSword();
	if (gun && sword) {
		return CardType::both;
	}
	else if (sword) {
		return CardType::swordOnly;
	}
	else if (gun) {
		return CardType::gunOnly;
	}
	else {
		LOGERROR("ULevelUp_weaponSelect::getCardType - player cannot receive either weapon type");
		return CardType::INVALID;
	}
}

bool ULevelUp_weaponSelect::containersAreValid() const {
	if (
		!playerRegistered() ||
		!IsValid(_player->getWeaponContainer(0)) ||
		_player->getWeaponContainer(0)->getContainer() == nullptr ||
		!IsValid(_player->getWeaponContainer(1)) ||
		_player->getWeaponContainer(1)->getContainer() == nullptr
		)
	{
		LOGERROR("ULevelUp_weaponSelect::getCardType - invalid player or weapon container");
		return false;
	}
	return true;
}

bool ULevelUp_weaponSelect::swapToNewWeapon(UWeaponTemplate* weapon) {
	if (!IsValid(weapon)) {
		LOGERROR("ULevelUp_weaponSelect::swapToNewWeapon - invalid parameter");
		return false;
	}
	_selectedWeapon = weapon;
	_validSocketData = calculateSocketData(_left, _right);
	auto slots = getEmptySlots();
	_slotIndex = slots.empty() ? -1 : slots[0];
	return isReady();
}

bool ULevelUp_base::registerPlayer(AMyPlayer* player) {
	if (!IsValid(player)) {
		LOGERROR("ULevelUp_weaponSelect::constructor = invalid player");
		return false;
	}
	_player = player;
	return true;
}
void ULevelUp_weaponSelect::upgradeWeapon() const { _player->upgradeWeapon(_selectedWeapon); }
void ULevelUp_weaponSelect::giveWeapon() const { _player->giveWeapon(_selectedWeapon, _slotIndex); }

bool ULevelUp_evolutionHandler::registerPlayer(AMyPlayer* player) {
	Super::registerPlayer(player);
	if (!playerRegistered()) {
		LOGERROR("ULevelUp_evolutionHandler::registerPlayer - invalid player");
		return false;
	}
	auto modEvolutionOptions = [this](const TMap<UWeaponTemplate_Evolution*, bool>& options) {
		TArray<UWeaponTemplate*> ret;
		for (const auto& pair : options) {
			if (ret.Num() == 2)
				return ret;
			if (pair.Value)
				continue;
			ret.Add(Cast<UWeaponTemplate>(pair.Key));
		}
		for (const auto& pair : options) {
			if (ret.Num() == 2)
				return ret;
			if (!pair.Value)
				continue;
			if (FMath::RandBool()) {
				ret.Add(Cast<UWeaponTemplate>(pair.Key));
			}
		}
		return ret;
		};
	_evolutionOptions = modEvolutionOptions(_player->getEligibleEvolutionList());
	return true;
}

// An evolution will always show up if eligible the first time, up to a maximum of two options. After that, each evolution has a 50% chance to show up, but non-turned-down evolutions
// have priority.
TArray<UWeaponTemplate*> ULevelUp_evolutionHandler::getModdedEvolutionOptions() const {
	return _evolutionOptions;
}

void ULevelUp_evolutionHandler::selectEvolution(UWeaponTemplate* option) {
	if (_evolutionSelected) {
		LOGERROR("ULevelUp_evolutionHandler::selectEvolution - evolution already selected");
		return;
	}
	if (!playerRegistered()) {
		LOGERROR("ULevelUp_evolutionHandler::selectEvolution - invalid player");
		return;
	}
	UWeaponTemplate_Evolution* casted = Cast<UWeaponTemplate_Evolution>(option);
	if (!IsValid(casted)) {
		LOGERROR("ULevelUp_evolutionHandler::selectEvolution - option is not valid");
		return;
	}
	if (!_evolutionOptions.Contains(option)) {
		LOGERROR("ULevelUp_evolutionHandler::selectEvolution - option not in evolution options");
		return;
	}
	_player->evolveWeapon(casted);
	for (const auto& key : _evolutionOptions) {
		if (key != option) {
			auto temp = Cast<UWeaponTemplate_Evolution>(key);
			if (IsValid(temp))
				_player->turnDownEvolution(temp);
		}
	}
	_evolutionSelected = true;
}