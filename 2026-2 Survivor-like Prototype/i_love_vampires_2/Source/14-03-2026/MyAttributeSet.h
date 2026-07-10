#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BaseAttributes.h"
#include <functional>
#include <memory>
#include "StatusEffectInterface.h"
#include "MyAttributeSet.generated.h"
class UStatusEffect;

UCLASS()
class I_LOVE_VAMPIRES_2_API UMyAttributeSet : public UObject {
	GENERATED_BODY()

	size_t _rowSize = 1;
	TArray<UStatusEffect*>_instantStatusQueue;
	UPROPERTY()
	TArray<UStatusEffect*> _statusEffects;
	TMap<size_t, std::function<void(float, float)>> _callbacks;
	std::shared_ptr<StatusEffectInterface> _statusEffectInterface = std::make_shared<StatusEffectInterface>();

	UPROPERTY()
	UBaseAttributes* _baseAttributes;
	
protected:
	TSubclassOf<UBaseAttributes> _attributeClass = UBaseAttributes::StaticClass();
	UBaseAttributes* _finalAttributes = nullptr;
	UPROPERTY()
	UBaseAttributes* _attributeOffsets;

	//probably out of date text
	
	// Status effects that are recalculated every frame, including those that never change (such as a passive that gives +25% damage) are deleted and reapplied inside reconstruct().
	// 
	// Certain status effects (particularly damage dealt) are intended to apply once and then be destroyed, meaning they will not be able to reapply their effect in future frames.
	// Such status effects should instead apply their effect to _attributeOffsets, which is never reset. During reconstruction, status effects will be passed both _finalAttributes
	// and _attributeOffsets. It is their responsibility to modify the correct one.
	// 
	// For example, a burn effect would apply its damage to _attributeOffsets, despite being persistent, because it calculates its damage based on the current state of the game, and cannot calculate it 
	// again in later frames without storing that state. 
	// 
	// From a performance standpoint, a persistent status effect that gives a permanent +25% damage only needs to store 1 float, and during reconstruction only needs to apply 1 float. If a bleed
	// effect were to store the damage it's dealt in every previous frame, it would need to store and apply thousands of floats.
	

private:
	const float* getMember(size_t offset, const uint8* base) const;
	void reconstruct(float delta);
	float getFinalValue(size_t offset) const;
	float getBaseValue(size_t offset) const;

protected:
	bool initialise_UMyAttributeSet(const UBaseAttributes* rowReference, const TSubclassOf<UBaseAttributes>& attributeType);
	// Inheritor is responsible for initialising these two
	TWeakObjectPtr<UBaseAttributes> _modifiers = nullptr;
	// This should overwrite the values of _finalAttributes using baseAttributes. It is used in reconstruct.
	virtual bool updateFromModifiers() { return true; }
	
	// calculates size_t from address
	static size_t calculateOffset(const UBaseAttributes* structRef, const float& memberRef);
	void addCallback(size_t offset, std::function<void(float,float)> callback);
	// not automatic
	virtual void tick(float delta);
	const TArray<UStatusEffect*> getStatusEffects() const { return _statusEffects; }

public:
	void inflictInstantStatus(UStatusEffect*);
	void inflictStatus(UStatusEffect*);
	const UBaseAttributes* getAttributes() const { return _finalAttributes.Get(); }
};