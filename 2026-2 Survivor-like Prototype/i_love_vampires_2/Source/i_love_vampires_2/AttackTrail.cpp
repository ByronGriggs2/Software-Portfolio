#include "AttackTrail.h"
#include "Math/UnrealMathUtility.h"
#include "unrealHelpers.h"
#include "TagManager.h"
#include "Definitions.h"

AttackTrailFactory::AttackTrailFactory(ACombatant* owner, const UAOETemplate* temp) : AOEFactory(owner, temp) {}

TrailSegmentAttackInitStruct AttackTrailFactory::getTrailSegmentInit(bool randomizeRotation, const FVector& forward) const {
	AOEInitStruct tempInit = getAOEInit();
	TrailSegmentAttackInitStruct ret(tempInit);
	ret._rotation = randomizeRotation ? FMath::FRandRange(0, 2 * PI) : unrealHelpers::getYaw(forward);
	return ret;
}

// assumes default settings for getTrailSegmentInit for now
// also assumes a period of 0.1f
TArray<AAOE*> AttackTrailFactory::launchAttack_Impl(const FVector& forward) {
	const static float PERIOD = 0.1f;

	if (!_owner.IsValid())
		return {};
	ATrailSegmentFactory* attack = nullptr;
	if (!unrealHelpers::spawnActorOnTopOfMeDeferred<ATrailSegmentFactory>(_owner.Get(), attack)) {
		LOGERROR("AttackTrailFactory::launchAttack - failed to spawn ATrailSegmentFactory");
		return {};
	}
	auto init = getTrailSegmentInit();
	attack->initialise_ATrailSegmentFactory(PERIOD, init, _owner.Get());
	if (!unrealHelpers::finishDeferredSpawn<ATrailSegmentFactory>(_owner.Get(), attack)) {
		LOGERROR("AttackTrailFactory::launchAttack - failed to finish spawning ATrailSegmentFactory");
		return {};
	}
	return {};
}

bool AAttackTrail::canEffectTarget(ACombatant* target) const {
	if (!IsValid(target)) {
		LOGERROR("AAttackTrail::canEffectTarget - invalid target");
		return false;
	}
	auto tags = target->getTagManager()->getTags<TrailTag>();
	for (const auto& tag : tags) {
		if (tag._attack.IsValid() && tag._attack.Get() == _trailOwner.Get())
			return false;
	}
	return true;
}

std::unique_ptr<TrailTag> AAttackTrail::getTrailTag() const {
	if (!IsValid(_AOEConfig)) {
		LOGERROR("AAttackTrail::getTrailTag - invalid AOEConfig");
		return nullptr;
	}
	if (!_trailOwner.IsValid())
		return nullptr;
	float duration = _AOEConfig->_tickInterval;
	return std::make_unique<TrailTag>(_trailOwner.Get(), duration);
}

bool AAttackTrail::initialise_AAttackTrail(ATrailSegmentFactory* owner, const AOEInitStruct& init) {
	if (!IsValid(owner)) {
		LOGERROR("AAttackTrail::initialise_AAttackTrail - invalid owner");
		return false;
	}
	_trailOwner = owner;
	initialise_AAOE(init);
	return true;
}

void AAttackTrail::applyEffect(ACombatant* target) {
	if (!IsValid(target)) {
		LOGERROR("AAttackTrail::applyEffect - invalid target");
		return;
	}
	if (!_trailOwner.IsValid())
		return;
	if (!canEffectTarget(target))
		return;
	Super::applyEffect(target);
	target->getTagManagerNonconst()->addTag(getTrailTag());
}