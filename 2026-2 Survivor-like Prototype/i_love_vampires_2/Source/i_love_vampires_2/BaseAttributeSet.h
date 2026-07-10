#pragma once
#include "EffectStruct.h"
#include "CoreMinimal.h"
#include <memory>
#include <vector>

class BaseAttributeSet {
	const static inline EStatus _BURN = EStatus::burn;
	const static inline EStatus _POISON = EStatus::poison;

	TArray<FEffectStruct> _statusEffects;
	// This is used to keep track of permanent statuses. It does NOT control the status effects themselves, since they're in _statusEffects like all the
	// other ones. This record is needed so that when a status needs to be removed, the correct one can be found.
	TMap<int, FEffectStruct> _permanentStatusRecord;
	int _permanentStatusCounter = 0;

	void handleBurn();
	std::vector<int> getStatusesOfType(EStatus, float& highestMagnitude, float& longestDuration);

protected:
	const TArray<FEffectStruct>& getStatusEffects() { return _statusEffects; }
	int getStatusCount(EStatus type);
	void removeStatusesOfType(EStatus);

public:
	virtual ~BaseAttributeSet() = default;
	// not automatic
	virtual void tick(float delta);
	virtual void inflictStatus(const FEffectStruct& statusEffect) { _statusEffects.Add(statusEffect); }
	bool hasPersistentStatus() const;
	bool hasStatus(EStatus type) const;
	TArray<FEffectStruct> getStatusEffectsCopy() const { return _statusEffects; }
	void handlePoison(UObject* context);
	// This returns a UID for the status, such that it can be removed by the caller at a later time if needed.
	int inflictPermanentStatus(FEffectStruct statusEffect);
	bool removePermanentStatus(int uid);
};