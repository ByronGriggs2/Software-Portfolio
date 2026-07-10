#include "KeybindSection.h"
#include "Definitions.h"

#define SET_KEYBIND(enumType, mapType, reverseMapType) \
void KeybindSection::setKeybind(enumType input, FKey key, bool isPrimary) { \
	FKeymap oldKeymap; \
	if (mapType.Contains(input)) { \
		oldKeymap = mapType[input]; \
		mapType[input] = isPrimary ? FKeymap{key, oldKeymap._secondaryKey} : FKeymap{oldKeymap._primaryKey, key}; \
	} \
	else { \
		oldKeymap = FKeymap{ FKey(), FKey() }; \
		mapType.Add(input, (isPrimary ? FKeymap{key, FKey()} : FKeymap{FKey(), key})); \
	} \
	if (isPrimary && reverseMapType.Contains(oldKeymap._primaryKey)) { \
		reverseMapType[oldKeymap._primaryKey].Remove(input); \
		if (reverseMapType[oldKeymap._primaryKey].Num() == 0) \
			reverseMapType.Remove(oldKeymap._primaryKey); \
	} \
	else if (!isPrimary && reverseMapType.Contains(oldKeymap._secondaryKey)) { \
		reverseMapType[oldKeymap._secondaryKey].Remove(input); \
		if (reverseMapType[oldKeymap._secondaryKey].Num() == 0) \
			reverseMapType.Remove(oldKeymap._secondaryKey); \
	} \
	if (reverseMapType.Contains(key)) { \
		if (!reverseMapType[key].Contains(input)) \
			reverseMapType[key].Add(input); \
	} \
	else { \
		reverseMapType.Add(key, TArray<enumType>{ input }); \
	} \
}

SET_KEYBIND(EInput_UI, _UI, _UIReverse)
SET_KEYBIND(EInput_Action, _action, _actionReverse)

void KeybindSection::resetKeybind(EInput_UI input) {
	setKeybind(input, getDefaultKey(input), true);
	setKeybind(input, FKey(), false);
}
void KeybindSection::resetKeybind(EInput_Action input) {
	setKeybind(input, getDefaultKey(input), true);
	setKeybind(input, FKey(), false);
}

FKey KeybindSection::getDefaultKey(EInput_UI input) {
	const static TMap<EInput_UI, FKey> defaults = {
		{ EInput_UI::up, EKeys::W },
		{ EInput_UI::down, EKeys::S },
		{ EInput_UI::left, EKeys::A },
		{ EInput_UI::right, EKeys::D },
		{ EInput_UI::confirm, EKeys::Enter },
		{ EInput_UI::cancel, EKeys::Escape }
	};
	if (defaults.Contains(input))
		return defaults[input];
	else {
		LOGERROR("FKeymap::getDefaultKey - invalid input enum");
		return FKey();
	}
}
FKey KeybindSection::getDefaultKey(EInput_Action input) {
	const static TMap<EInput_Action, FKey> defaults = {
	};
	if (defaults.Contains(input))
		return defaults[input];
	else {
		//LOGERROR("FKeymap::getDefaultKey - invalid input enum");
		return FKey();
	}
}
KeybindSection::KeybindSection() {
	for (int i = 0; i < static_cast<int>(EInput_UI::num); i++) {
		resetKeybind(static_cast<EInput_UI>(i));
	}
	for (int i = 0; i < static_cast<int>(EInput_Action::num); i++) {
		resetKeybind(static_cast<EInput_Action>(i));
	}
}

void KeybindSection::loadFromSaveData(const FKeybindSaveData& data) {
	for (const auto& [input, keymap] : data._UI) {
		setKeybind(input, keymap._primaryKey, true);
		setKeybind(input, keymap._secondaryKey, false);
	}
	for (const auto& [input, keymap] : data._action) {
		setKeybind(input, keymap._primaryKey, true);
		setKeybind(input, keymap._secondaryKey, false);
	}
}