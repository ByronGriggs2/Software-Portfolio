#include "BaseAttributes.h"
#include "EffectStruct.h"
#include "Kismet/KismetMathLibrary.h"
#include "Combatant.h"
#include "Definitions.h"
#include "BaseAttributeData.h"
#include "Suplex.h"

void Stat::discretize() {
	float rng = FMath::FRand();
	float value = getFinal();
	_final = rng < (value-floor(value+EPSILON)) ? ceil(value-EPSILON) : floor(value+EPSILON);
}
void Stat::modify(float newVal) {
	if (!_softReset)
		LOGERROR("Stat::modify - modifying a stat that hasn't been soft reset this frame");
	_modifier = newVal;
	_softReset = false;
}

void Stat::softReset(Stat& stat) {
	stat._prebonus = 0;
	stat._postbonus = 0;
	stat._multiplier = 0;
	stat._final = 0;
	// The starting value of a Stat each frame is set by ModifyAttributes. Setting _final=_base is
// just default behaviour for subclasses that implement ModifyAttributes as noop.
	stat._modifier = stat._base;
	stat._softReset = true;
}

void BaseAttributes::tick(UObject* context, float delta, const TArray<FEffectStruct>& statusEffects) {
	softReset();
	tick_internal(context, delta, statusEffects);
}

void BaseAttributes::tick_internal(UObject* context, float delta, const TArray<FEffectStruct>& statusEffects) {
	for (const auto& status : statusEffects) {
		applyStatus(context, status, delta);
	}
	applyToAllStats(Stat::calculateFinal);
	postTick(delta);
	for (auto& stat : _zeroedAttributes) {
		if (stat)
			stat->zeroStat();
	}
	_zeroedAttributes.clear();
}

void BaseAttributes::softReset() {
	applyToAllStats(Stat::softReset);
}

void BaseAttributes::baseInit(const BaseAttributes& other) {
	std::vector<const Stat*> otherStats;
	other.applyToAllStats([&otherStats](const Stat& stat) {
		otherStats.push_back(&stat);
		});
	int i = 0;
	applyToAllStats([&otherStats, &i](Stat& stat) {
		stat = *otherStats[i];
		i++;
		});
}
void BaseAttributes::baseInit (BaseAttributes&& other) {
	std::vector<const Stat*> otherStats;
	other.applyToAllStats([&otherStats](const Stat& stat) {
		otherStats.push_back(&stat);
		});
	int i = 0;
	applyToAllStats([&otherStats, &i](Stat& stat) {
		stat = std::move(*otherStats[i]);
		i++;
		});
}
void BaseAttributes::baseInit(const UBaseAttributeData* data) {
	if (!IsValid(data)) {
		LOGERROR("BaseAttributes::baseInit - invalid data");
		return;
	}
	std::vector<float> bases;
	data->applyToAllStats([&bases](const float& base) {
		bases.push_back(base);
		});
	int i = 0;
	applyToAllStats([&bases, &i](Stat& stat) {
		stat = Stat(bases[i]);
		i++;
		});
}