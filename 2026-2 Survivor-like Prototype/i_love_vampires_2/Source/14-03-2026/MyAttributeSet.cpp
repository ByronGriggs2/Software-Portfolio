#include "MyAttributeSet.h"
#include "StatusEffect.h"
#include "Definitions.h"

template<typename attributeClass>
const float* UMyAttributeSet::getMember(const attributeClass* attr {
	if (offset >= _rowSize) {
		LOGERROR("UMyAttributeSet::getMember - offset is out of bounds");
		return nullptr;
	}
	const uint8* byte = base + offset;
	return reinterpret_cast<const float*>(byte);
}

float* getMemberRef()

float UMyAttributeSet::getBaseValue(size_t offset) const {
	if (!IsValid(_baseAttributes)) {
		LOGERROR("UMyAttributeSet::getBaseValue - _baseAttributes is not valid");
		return 0;
	}
	const float* member = getMember(offset, reinterpret_cast<uint8*>(_baseAttributes));
	if (member == nullptr)
		return 0;
	return *member;
}

// Invalid is not a fatal error because this class does not own _finalAttributes
float UMyAttributeSet::getFinalValue(size_t offset) const {
	if (!_finalAttributes.IsValid())
		return 0;
	const float* member = getMember(offset, reinterpret_cast<uint8*>(_finalAttributes.Get()));
	if (member == nullptr)
		return 0;
	return *member;
}

void UMyAttributeSet::tick(float delta) {
	TMap<size_t, float> oldValues;
	//pair is TPair
	for (const auto& pair : _callbacks) {
		oldValues[pair.Key] = getFinalValue(pair.Key);
	}
	reconstruct(delta);
	for (const auto& pair : _callbacks) {
		float newValue = getFinalValue(pair.Key);
		if (abs(newValue - oldValues[pair.Key]) > EPSILON)
			pair.Value(oldValues[pair.Key], newValue);
	}
	// UMyAttributeSet is responsible for the lifetime of status effects
	for (auto i = 0; i < _statusEffects.Num();i=i)
	{
		if (_statusEffects[i]->getDuration() <= 0)
			_statusEffects.RemoveAt(i);
		else
			i++;
	}
	_instantStatusQueue.Empty();
}

bool UMyAttributeSet::initialise_UMyAttributeSet(const UBaseAttributes* rawData, const TSubclassOf<UBaseAttributes>& attributeType) {
	if (!IsValid(rawData)) {
		LOGERROR("UMyAttributeSet::Initialise_UMyAttributeSet - rowReference is invalid");
		return false;
	}
	if (!IsValid(attributeType)) {
		LOGERROR("UMyAttributeSet::Initialise_UMyAttributeSet - attributeType is invalid");
		return false;
	}
	_attributeClass = attributeType;
	_baseAttributes = DuplicateObject<UBaseAttributes>(rawData, this);
	_finalAttributes = DuplicateObject<UBaseAttributes>(rawData, this);
	// since the data is immediately zeroed, it doesn't matter what I store here, but I use the CDO for clarity
	UBaseAttributes* temp = attributeType->GetDefaultObject<UBaseAttributes>();
	_attributeOffsets = DuplicateObject<UBaseAttributes>(temp, this);
	_rowSize = sizeof(*rawData);
	for (size_t i = 0; i < _rowSize; i += sizeof(float)) {
		uint8* byte = _attributeOffsets + i;
		float* ref = reinterpret_cast<float*>(byte);
		*ref = 0;
	}
	return true;
}

// Note that _finalValues and _attributeValues are passed by reference
void UMyAttributeSet::reconstruct(float delta) {
	if (!_finalAttributes.IsValid()) {
		return;
	}
	if (!IsValid(_baseAttributes) || !IsValid(_attributeOffsets)) {
		LOGERROR("UMyAttributeSet::reconstruct - _baseAttributes or _attributeOffsets is invalid");
		return;
	}
	if (!updateFromModifiers())
		return;
	_statusEffectInterface->_modificationAllowed = true;
	for (const auto& status : _statusEffects) {
		status->prebonusStep(delta);
	}
	for (const auto& status : _statusEffects) {
		status->multiplierStep(delta);
	}
	for (const auto& status : _statusEffects) {
		status->postbonusStep(delta);
	}
	// I cannot think of a reason why instant status effects would need a prebonus or multiplier step, so they are omitted for safety.
	for (const auto& status : _instantStatusQueue) {
		status->postbonusStep(delta);
	}
	_statusEffectInterface->_modificationAllowed = false;
}

void UMyAttributeSet::addCallback(size_t offset, std::function<void(float, float)> callback) {
	_callbacks.Add(offset, callback);
}

size_t UMyAttributeSet::calculateOffset(const UBaseAttributes* structRef, const float& memberRef) {
	const uint8* base = reinterpret_cast<const uint8*>(structRef);
	const uint8* member = reinterpret_cast<const uint8*>(&memberRef);
	size_t offset = member - base;
	if (offset >= _rowSize) {
		LOGERROR("UMyAttributeSet::calculateOffset - member reference is out of bounds");
		return 0;
	}
	return offset;
}

void UMyAttributeSet::inflictInstantStatus(UStatusEffect* statusEffect) {
	_instantStatusQueue.Add(statusEffect);
	_instantStatusQueue.Back()->initialise_secondStep(_statusEffectInterface, _finalAttributes, _attributeOffsets)
}

void UMyAttributeSet::inflictStatus(UStatusEffect* statusEffect) {
	_statusEffects.Add(statusEffect);
}

