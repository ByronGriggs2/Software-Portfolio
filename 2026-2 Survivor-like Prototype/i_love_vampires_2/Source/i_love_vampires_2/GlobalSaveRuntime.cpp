#include "GlobalSaveRuntime.h"
#include "GlobalSave.h"
#include "Definitions.h"

UGlobalSave* UGlobalSaveRuntime::getGlobalSave() const {
	UGlobalSave* save = NewObject<UGlobalSave>();
	save->_keyboard = _keyboard.getSaveData();
	save->_gamepad = _gamepad.getSaveData();
	return save;
}

void UGlobalSaveRuntime::loadGlobalSave(UGlobalSave* save) {
	if (save == nullptr) {
		LOGERROR("UGlobalSaveRuntime::loadGlobalSave - null save provided, using defaults");
		return;
	}
	_keyboard.loadFromSaveData(save->_keyboard);
	_gamepad.loadFromSaveData(save->_gamepad);
}