#include "StatusFactory.h"
#include "StatusEffect_Bleed.h"
#include "StatusEffect_Damage.h"
#include "StatusEffect_Burn.h"
#include "CoreMinimal.h"

#include "Definitions.h"

UStatusEffect* StatusFactory::createStatusEffect(const FEffectStruct& specs) {
	if (specs._type == _DAMAGE) {
		UStatusEffect_Damage* ret = NewObject<UStatusEffect_Damage>();
		if (!IsValid(ret)) {
			LOGERROR("StatusFactory::createStatusEffect - failed to create damage status effect");
			return nullptr;
		}
		ret->initialise_UStatusEffect_Damage(specs._magnitude, specs._chance);
		return ret;
	}
	else if(specs._type == _BLEED) {
		UStatusEffect_Bleed* ret = NewObject<UStatusEffect_Bleed>();
		if (!IsValid(ret)) {
			LOGERROR("StatusFactory::createStatusEffect - failed to create bleed status effect");
			return nullptr;
		}
		ret->initialise_UStatusEffect_Bleed(specs._magnitude, specs._duration, specs._chance);
		return ret;
	}
	else if(specs._type == _BURN) {
		UStatusEffect_Burn* ret = NewObject<UStatusEffect_Burn>();
		if (!IsValid(ret)) {
			LOGERROR("StatusFactory::createStatusEffect - failed to create burn status effect");
			return nullptr;
		}
		ret->initialise_UStatusEffect_Burn(specs._magnitude, specs._duration, specs._chance);
		return ret;
	}
	else {
		LOGERROR("StatusFactory::createStatusEffect - not implemented");
	}
	UStatusEffect* ret = NewObject<UStatusEffect>();
	if (!IsValid(ret)) {
		LOGERROR("StatusFactory::createStatusEffect - failed to create generic status effect");
		return nullptr;
	}
	ret->initialise_UStatusEffect(specs._duration, specs._magnitude, specs._chance);
	return ret;
}