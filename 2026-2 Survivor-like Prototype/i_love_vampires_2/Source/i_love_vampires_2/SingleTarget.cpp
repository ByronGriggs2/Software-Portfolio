#include "SingleTarget.h"
#include "MyGameplayStatics.h"
#include "CombatantManager.h"
#include "unrealHelpers.h"
#include "helpers.h"

SingleTargetFactory::SingleTargetFactory(ACombatant* owner, const UAttackTemplate* temp) : AttackFactory(owner, temp) {
	if (
		!IsValid(temp) ||
		temp->_levels.Num() == 0 ||
		getLevel() < 0 ||
		getLevel() > temp->_levels.Num() - 1 ||
		!IsValid(_levels[getLevel()])
		)
	{
		LOGERROR("SingleTargetFactory::SingleTargetFactory - invalid parameter");
		return;
	}
	const auto casted = Cast<USingleTargetLevel>(temp->_levels[getLevel()]);
	if (!IsValid(casted)) {
		LOGERROR("AProjectileFactory::initialise_AProjectileFactory - attempted to initialize with incompatible level");
		return;
	}
	auto temp2 = std::make_shared<SingleTargetAttributes>(casted->_singleTargetOffsets, owner->getAttributes());
	_singleTargetAttributes = std::make_unique<BaseAttributeWrapper<SingleTargetAttributes>>(owner, temp2);
}

void USingleTargetAttributeData::replaceOverrides() {
	for (const auto& [memberPtr, defaultVal] : DefaultProxy<USingleTargetAttributeData>::get())
		BASEATTRIBUTES_OVERRIDE(memberPtr, defaultVal);
}

bool SingleTargetFactory::canFire() const {
	AEnemyBase* target = nullptr;
	return getTarget(target);
}

float SingleTargetFactory::getRange() const {
	return _singleTargetAttributes->getMember(&SingleTargetAttributes::_range);
}

SingleTargetAttributes::SingleTargetAttributes(const USingleTargetAttributeData* attr, std::shared_ptr<const CombatantAttributes> attrRef) :
	BaseAttributes(attr),
	_attrRef(attrRef)
{
	baseInit(attr);
}

void SingleTargetAttributes::tick(UObject* context, float delta, const TArray<FEffectStruct>& statusEffects) {
	auto temp = _attrRef.lock();
	if (temp.get() == nullptr)
		return;
	softReset();
	modifyAttributes(temp);
	tick_internal(context, delta, statusEffects);
}

void SingleTargetAttributes::modifyAttributes(const std::shared_ptr<const CombatantAttributes>& attr) {
	if (attr.get() == nullptr){
		LOGERROR("SingleTargetAttributes::modifyAttributes - attr is nullptr");
		return;
	}
	_range.modify(_range.getBase() * attr->_range.getFinal() * _BASE_RANGE);
}

bool SingleTargetFactory::getTarget(AEnemyBase*& ret) const {
	if (!_owner.IsValid())
		return false;
	UCombatantManager* manager = nullptr;
	if (!MyGameplayStatics::getCombatantManager(_owner.Get(), manager)) {
		LOGERROR("SingleTargetFactory::canFire - failed to get combatant manager");
		return false;
	}
	ret = manager->getNearestEnemyPtr(_owner.Get());
	if (!IsValid(ret))
		return false;
	const float distance = unrealHelpers::getDistance(_owner->GetActorLocation(), ret->GetActorLocation());
	if (distance > getRange())
		return false;
	return true;
}

void SingleTargetFactory::launchAttack(const FVector& forward) {
	launchAttack_internal(forward);
}
void SingleTargetFactory::launchAttack(AEnemyBase* target, const FVector& forward) {
	launchAttack_internal(target, forward);
}
void SingleTargetFactory::launchAttack_internal(AEnemyBase* target, const FVector& forward) {
	if (!IsValid(target)) {
		LOGERROR("SingleTargetFactory::launchAttack - target is not valid");
		return;
	}
	AAttackActor* newAttack = nullptr;
	if (!unrealHelpers::spawnActorOnTopOfMeDeferred<AAttackActor>(_owner.Get(), newAttack)) {
		LOGERROR("SingleTargetFactory::launchAttack - failed to create attack");
		return nullptr;
	}
	{
		AttackInitStruct temp = getAttackInit();
		newAttack->initialise_AAttackActor(temp);
	}
	if (!unrealHelpers::finishDeferredSpawn<AAttackActor>(_owner.Get(), newAttack)) {
		LOGERROR("SingleTargetFactory::launchAttack - failed to finish spawning attack");
		return nullptr;
	}
	newAttack->applyEffect(target);
	newAttack->SetActorLocation(target->GetActorLocation());
	addToDestroyQueue(newAttack);
}

AEnemyBase* SingleTargetFactory::launchAttack_internal(const FVector& forward) {
	AEnemyBase* target = nullptr;
	if (!getTarget(target))
		return nullptr;
	launchAttack_internal(target, forward);
	return target;
}

void SingleTargetFactory::addToDestroyQueue(AAttackActor* newAttack) {
	_destroyQueue.Add(newAttack, _AFTERIMAGE_DURATION);
}

void SingleTargetFactory::tick(float delta) {
	AttackFactory::tick(delta);
	_singleTargetAttributes->tick(delta, getStatusEffects());
	for (auto It = _destroyQueue.CreateIterator(); It; ++It)
	{
		if (!It->Key.IsValid()) {
			It.RemoveCurrent();
		}
		else if (It->Value <= 0)
			It->Key->Destroy();
		else
			It->Value -= delta;
	}
	//for (auto& pair : _destroyQueue) {
	//	if (!pair.Key.IsValid()) {
	//		_destroyQueue.Remove(pair.Key);
	//	}
	//	else if (pair.Value <= 0)
	//		pair.Key->Destroy();
	//	else
	//		pair.Value -= delta;
	//}
}