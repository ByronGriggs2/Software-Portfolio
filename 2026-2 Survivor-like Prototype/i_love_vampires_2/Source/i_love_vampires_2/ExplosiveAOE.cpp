#include "ExplosiveAOE.h"
#include "Definitions.h"
#include "unrealHelpers.h"
#include "AssetRefs.h"
#include "MyGameplayStatics.h"

void AExplosiveAOE::applyEffect(ACombatant* target) {
	bool wasHittable = isHittable(target);
	Super::applyEffect(target);
	if (!IsValid(target) || isFriendly(target) || !wasHittable)
		return;
	spawnChildren(target);
}

ExplosiveAOEInitStruct ExplosiveAOEFactory::getExplosiveAOEInit() const {
	auto primary = getAOEInit();
	TArray<AOEInitStruct> childInits;
	for (const auto& childFactory : _childFactories) {
		auto temp = childFactory->getAOEInit();
		childInits.Add(temp);
	}
	return ExplosiveAOEInitStruct{ primary, childInits };
}

TArray<AAOE*> ExplosiveAOEFactory::launchAttack_Impl(const FVector& forward) {
	if (!_owner.IsValid()) {
		return {};
	}
	AExplosiveAOE* mainAOE = nullptr;
	if (!unrealHelpers::spawnActorOnTopOfMeDeferred<AExplosiveAOE>(_owner.Get(), mainAOE)) {
		LOGERROR("ExplosiveAOEFactory::launchAttack - failed to spawn main AOE");
		return {};
	}
	ExplosiveAOEInitStruct mainInit = getExplosiveAOEInit();
	mainAOE->initialise_AExplosiveAOE(mainInit);
	if (!unrealHelpers::finishDeferredSpawn(_owner.Get(), mainAOE)) {
		LOGERROR("ExplosiveAOEFactory::launchAttack - failed to finish spawning main AOE");
		return {};
	}
	return { mainAOE };
}

bool AExplosiveAOE::initialise_AExplosiveAOE(const ExplosiveAOEInitStruct& temp) {
	UAssetRefs* assetRefs = nullptr;
	if (!MyGameplayStatics::getAssetRefs(assetRefs)) {
		LOGERROR("ExplosiveAOEFactory::getExplosiveAOEInit - failed to get UAssetRefs");
		return false;
	}
	initialise_AAOE(temp._AOE);
	setFlipbook(assetRefs->getGenericExplosion());
	_childAOEs = temp._childAOEs;
	return true;
}

void AExplosiveAOE::BeginPlay() {
	Super::BeginPlay();
}

TArray<AAOE*> AExplosiveAOE::spawnChildren(ACombatant* target) {
	if (!IsValid(target)) {
		LOGERROR("AExplosiveAOE::spawnChildren - invalid target");
		return {};
	}
	UAssetRefs* assetRefs = nullptr;
	if (!MyGameplayStatics::getAssetRefs(assetRefs)) {
		LOGERROR("ExplosiveAOEFactory::getExplosiveAOEInit - failed to get UAssetRefs");
		return {};
	}
	auto location = target->GetActorLocation();
	TArray<AAOE*> ret;
	// Note that this circumvents the typical way of spawning AOEs as it does not use a factory. This is fine as long as you don't expect any of the
	// functionality provided by the factory, such as automatically orienting the AAOE
	for (const auto& childInit : _childAOEs) {
		AAOE* childAOE = nullptr;
		if (!unrealHelpers::spawnActorOnTopOfMeDeferred<AAOE>(this, childAOE)) {
			LOGERROR("AExplosiveAOE::spawnChildren - failed to spawn child AOE");
			continue;
		}
		childAOE->initialise_AAOE(childInit);
		if (!unrealHelpers::finishDeferredSpawn<AAOE>(this, childAOE)) {
			LOGERROR("AExplosiveAOE::spawnChildren - failed to finish spawning child AOE");
			continue;
		}
		childAOE->SetActorLocation(location, false, nullptr, ETeleportType::TeleportPhysics);
		childAOE->setFlipbook(assetRefs->getGenericExplosion());
		ret.Add(childAOE);
	}
	return ret;
}