#pragma once
#include "StatusEffect.h"
#include <memory>
#include "EffectStruct.h"
class UObject;

class StatusFactory {
	const static EStatus _DAMAGE = EStatus::damage;
	const static EStatus _BLEED = EStatus::bleed;
	const static EStatus _BURN = EStatus::burn;

public:
	static UStatusEffect* createStatusEffect(const FEffectStruct& effectStruct);
};