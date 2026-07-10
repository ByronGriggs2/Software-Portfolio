#include "BaseAttributeSet.h"

/*
1. Set _final = _base
2. Modify _final based on the static functions in the respective UBaseAttributes subclasses. UBaseAttributeComponent subclasses are responsible for typing and 
calling.
3. Apply all status effects
4. Delete expired status effects
5. Execute callbacks
*/
void ABaseAttributeSet::tick(float delta) {
	TArray<UBaseAttributeComponent*> components;
	GetComponents(components, false);
	for (auto& component : components) {
		if (!IsValid(component))
			continue;
		component->resetFinal();
		// The UBaseAttribues inheritance hierarchy should only be 1 layer deep. Each component is responsible for overriding modifyAttributes and knowing what 
		// modifiers its attributes need.
		component->modifyAttributes(this);
	}
	for (const auto& statusEffect : _statusEffects) {
		if (IsValid(statusEffect)) {
			statusEffect->prebonusStep(delta, this);
		}
	}
	for (const auto& statusEffect : _statusEffects) {
		if (IsValid(statusEffect)) {
			statusEffect->multiplierStep(delta, this);
		}
	}
	for (const auto& statusEffect : _statusEffects) {
		if (IsValid(statusEffect)) {
			statusEffect->postbonusStep(delta, this);
		}
	}
	for (auto i = _statusEffects.Num() - 1; i > -1; i--)
	{
		if (!IsValid(_statusEffects[i]) || _statusEffects[i]->getDuration() <= 0) {
			_statusEffects.RemoveAt(i);
		}
	}
	for (auto it = _callbacks.begin(); it != _callbacks.end();) {
		CallbackBase* callback = (*it).get();
		if (callback == nullptr) {
			LOGERROR("ABaseAttributeSet::tick - callback (a non-UCLASS) is null");
			it = _callbacks.erase(it);
			continue;
		}
		if (!callback->tick()) {
			it = _callbacks.erase(it);
		}
		else {
			it++;
		}
	}
}

void ABaseAttributeSet::initialise_ABaseAttributeSet(AActor* caller, UBaseAttributes* mods) {
	if (!IsValid(caller)) {
		LOGERROR("ABaseAttributeSet::initialise_ABaseAttributeSet - caller is not valid");
		return;
	}
	_owner = TWeakObjectPtr<AActor>(caller);
	if (mods != nullptr) {
		if (!IsValid(mods)) {
			LOGERROR("ABaseAttributeSet::initialise_ABaseAttributeSet - mods is not valid and also not null");
			return;
		}
		_modifiers = TWeakObjectPtr<UBaseAttributes>(mods);
	}
	_initialised = true;
}

void ABaseAttributeSet::Tick(float delta) {
	if (!_initialised)
		return;
	if (!_owner.IsValid()) {
		Destroy();
		return;
	}
}