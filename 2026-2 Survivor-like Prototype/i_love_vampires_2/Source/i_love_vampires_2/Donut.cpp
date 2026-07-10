#include "Donut.h"
#include "helpers.h"
#include "unrealHelpers.h"
#include "Components/ShapeComponent.h"

void UDonutTemplate::replaceOverrides() {
	Super::replaceOverrides();
	if (helpers::isInvalidData(_innerOuterRatio)) {
		_innerOuterRatio = _defaults._innerOuterRatio;
	}
}

DonutInitStruct DonutFactory::getDonutInit() const {
	AOEInitStruct base = getAOEInit();
	return DonutInitStruct{
		base,
		_innerOuterRatio
	};
}

DonutFactory::DonutFactory(DonutFactory&& other) : AOEFactory(std::move(other)), _innerOuterRatio(other._innerOuterRatio) {}

DonutFactory::DonutFactory(ACombatant* owner, const UDonutTemplate* temp) : AOEFactory(owner, temp) {
	if (!IsValid(temp)) {
		LOGERROR("DonutFactory::DonutFactory - received null template");
		return;
	}
	_innerOuterRatio = temp->_innerOuterRatio;
}

// This is an example- not an exception- of an AAOE that overrides AOEFactory::launchAttack_Impl(const FVector&) (the default protected version)
// such that the returned AAOE can be modified by BaseAttackFactory::launchAttack(const FVector&) (overridden by AOEFactory). If this version of the
// function runs via virtual dispatch, no alternative AOEInitStruct shenanigans should be involved.
TArray<AAOE*> DonutFactory::launchAttack_Impl(const FVector& forward) {
	ADonut* newAttack = nullptr;
	if (!unrealHelpers::spawnActorOnTopOfMeDeferred<ADonut>(_owner.Get(), newAttack)) {
		LOGERROR("DonutFactory::launchAttack - failed to create Donut");
		return {};
	}
	{
		DonutInitStruct temp = getDonutInit();
		newAttack->initialise_ADonut(temp);
	}
	if (!unrealHelpers::finishDeferredSpawn<ADonut>(_owner.Get(), newAttack)) {
		LOGERROR("DonutFactory::launchAttack - failed to finish spawning Donut");
		return {};
	}
	return { newAttack };
}

bool ADonut::shouldApplyEffect(AActor* OtherActor) const {
	if (!IsValid(OtherActor))
		return false;
	FVector myLocation = GetActorLocation();
	FVector otherLocation = OtherActor->GetActorLocation();
	const float distance = unrealHelpers::getDistance(myLocation, otherLocation);
	return distance >= getRadius() * _innerOuterRatio;
}

bool ADonut::createCollider() {
	_collider = makeDefaultSphereCollider();
	return IsValid(_collider);
}

bool ADonut::initialise_ADonut(const DonutInitStruct& temp) {
	initialise_AAOE(temp._base);
	_innerOuterRatio = temp._innerOuterRatio;
	return true;
}