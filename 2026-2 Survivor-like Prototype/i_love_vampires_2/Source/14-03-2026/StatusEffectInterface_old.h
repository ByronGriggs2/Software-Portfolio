#pragma once
class UMyAttributeSet;
class StatusEffectInterface {
	bool _modificationPermitted = false;
public:
	friend class UMyAttributeSet;
	bool isModificationPermitted() const { return _modificationPermitted; }
};