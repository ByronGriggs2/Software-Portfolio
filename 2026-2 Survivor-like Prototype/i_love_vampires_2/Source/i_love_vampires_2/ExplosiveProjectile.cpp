#include "ExplosiveProjectile.h"
#include "Definitions.h"
#include "AOE.h"
#include "Combatant.h"
#include "unrealHelpers.h"
#include "PaperFlipbook.h"

///////////////////////////////////////////////////////////////////////////////
// AExplosiveProjectile
void AExplosiveProjectile::initialise_AExplosiveProjectile(const ExplosiveProjectileInitStruct& temp) {
	AProjectile::initialise_AProjectile(temp._projectile);
	_AOE = temp._AOE;
	_explosiveProjectileConfig = temp._explosiveProjectileConfig;
	_explosiveProjectileAttributes = std::make_unique<ExplosiveProjectileAttributes>(temp._explosiveProjectileAttributes);
}

void AExplosiveProjectile::BeginPlay() {
	if (!IsValid(_explosiveProjectileConfig.Get()) || _explosiveProjectileAttributes.get() == nullptr || !IsValid(_AOE)) {
		LOGERROR("AExplosiveProjectile::initialise_AExplosiveProjectile - invalid explosive projectile config or attributes");
		return;
	}
	Super::BeginPlay();
}

void AExplosiveProjectile::bulletDeath() {
	if (!IsValid(_AOE)) {
		LOGERROR("AExplosiveProjectile::bulletDeath - aoe is not valid");
		AProjectile::bulletDeath();
		return;
	}
	_AOE->SetActorLocation(GetActorLocation());
	_AOE->completeDelayedConstruction();
	AProjectile::bulletDeath();
}
///////////////////////////////////////////////////////////////////////////////
// ExplosiveProjectileFactory
ExplosiveProjectileFactory::ExplosiveProjectileFactory(ACombatant* pawn, const UExplosiveProjectileTemplate* myTemplate) : ProjectileFactory(pawn, myTemplate)
{
	if (
		!IsValid(myTemplate) ||
		!IsValid(myTemplate->_explosiveProjectileConfig) ||
		!IsValid(myTemplate->_AOEConfig) ||
		!IsValid(myTemplate->_AOEConfig_attack) ||
		_levels.Num() == 0 ||
		getLevel() < 0 ||
		getLevel() > _levels.Num() - 1 ||
		!IsValid(_levels[getLevel()])
		)
	{
		LOGERROR("AExplosiveProjectileFactory::AExplosiveProjectileFactory - invalid parameter");
		return;
	}
	_explosiveProjectileConfig = myTemplate->_explosiveProjectileConfig;
	_AOEConfig = myTemplate->_AOEConfig;
	_AOEConfig_attack = myTemplate->_AOEConfig_attack;
	const auto casted = Cast<UExplosiveProjectileLevel>(_levels[getLevel()]);
	{
		auto temp = std::make_shared<ExplosiveProjectileAttributes>(casted->_explosiveProjectileAttributeOffsets, pawn->getAttributes());
		_explosiveProjectileAttributes = std::make_unique<BaseAttributeWrapper<ExplosiveProjectileAttributes>>(pawn, temp);
	}
	{
		auto temp = std::make_shared<AOEAttributes>(casted->_AOEAttributeOffsets, pawn->getAttributes());
		_AOEAttributes = std::make_unique<BaseAttributeWrapper<AOEAttributes>>(pawn, temp);
	}
	{
		auto temp = std::make_shared<AttackAttributes>(casted->_AOEAttributeOffsets_attack, pawn->getAttributes());
		_AOEAttributes_attack = std::make_unique<BaseAttributeWrapper<AttackAttributes>>(pawn, temp);
	}
}

ExplosiveProjectileFactory::ExplosiveProjectileFactory(ExplosiveProjectileFactory&& other) :
	ProjectileFactory(std::move(other)),
	_tempAOE(other._tempAOE),
	_explosiveProjectileConfig(other._explosiveProjectileConfig),
	_explosiveProjectileAttributes(std::move(other._explosiveProjectileAttributes)),
	_AOEConfig(other._AOEConfig),
	_AOEAttributes(std::move(other._AOEAttributes)),
	_AOEConfig_attack(other._AOEConfig_attack),
	_AOEAttributes_attack(std::move(other._AOEAttributes_attack))
{
	//other._AOEConfig = nullptr;
	//other._explosiveProjectileConfig = nullptr;
	other._tempAOE = nullptr;
	other._AOEAttributes = nullptr;
	other._AOEAttributes_attack = nullptr;
	other._explosiveProjectileAttributes = nullptr;
}

void ExplosiveProjectileFactory::tick(float delta) {
	if (!_AOEAttributes || !_explosiveProjectileAttributes) {
		LOGERROR("AExplosiveProjectileFactory::tick - attributes not initialized");
		return;
	}
	_explosiveProjectileAttributes->tick(delta, getStatusEffects());
	_AOEAttributes->tick(delta, getStatusEffects());
	ProjectileFactory::tick(delta);
}

AProjectile* ExplosiveProjectileFactory::launchSingleProjectile(const FVector& direction) {
	if (_tempAOE.IsValid()) {
		LOGERROR("AExplosiveProjectileFactory::launchAttack - tempAOE is already valid when launching attack");
		return nullptr;
	}
	setDirectionX(direction.X);
	setDirectionZ(direction.Z);
	// Note that this circumvents the typical way of spawning AOEs as it does not use a factory. This is fine as long as you don't expect any of the
	// functionality provided by the factory, such as automatically orienting the AAOE
	{
		AAOE* aoe = nullptr;
		if (!unrealHelpers::spawnActorOnTopOfMeDeferred<AAOE>(_owner.Get(), aoe)) {
			LOGERROR("AExplosiveProjectileFactory::launchAttack - failed to spawn aoe");
			return nullptr;
		}
		_tempAOE = TWeakObjectPtr<AAOE>(aoe);
	}
	AExplosiveProjectile* newAttack = nullptr;
	if (!unrealHelpers::spawnActorOnTopOfMeDeferred<AExplosiveProjectile>(_owner.Get(), newAttack)) {
		LOGERROR("AExplosiveProjectileFactory::launchAttack - failed to spawn explosive projectile");
		// Important! GC will not collect this
		_tempAOE->Destroy();
		_tempAOE = nullptr;
		return nullptr;
	}
	{
		AOEInitStruct temp = getAOEInit();
		_tempAOE->initialise_AAOE(temp);
	}
	unrealHelpers::finishDeferredSpawn(_owner.Get(), _tempAOE.Get());
	{
		ExplosiveProjectileInitStruct temp = getExplosiveProjectileInit();
		newAttack->initialise_AExplosiveProjectile(temp);
	}
	_tempAOE = nullptr;
	unrealHelpers::finishDeferredSpawn<AExplosiveProjectile>(_owner.Get(), newAttack);
	return newAttack;
}

ExplosiveProjectileInitStruct ExplosiveProjectileFactory::getExplosiveProjectileInit() const {
	ExplosiveProjectileAttributes temp(*(_explosiveProjectileAttributes->getCore()));
	temp.discretizeFull();
	ExplosiveProjectileInitStruct ret(ProjectileFactory::getProjectileInit(), _tempAOE.Get(), _explosiveProjectileConfig.Get(), temp);
	return ret;
}

AOEInitStruct ExplosiveProjectileFactory::getAOEInit() const {
	TArray<FEffectStruct> status;
	if (_levels.Num() == 0 || getLevel() < 0 || getLevel() > _levels.Num() - 1 || !IsValid(_levels[getLevel()])) {
		LOGERROR("AExplosiveProjectileFactory::getAOEInit - invalid level");
		status = {};
	}
	else {
		const auto& casted = Cast<UExplosiveProjectileLevel>(_levels[getLevel()]);
		if (!IsValid(casted)) {
			LOGERROR("AExplosiveProjectileFactory::getAOEInit - level is not a UExplosiveProjectileLevel");
			status = {};
		}
		status = casted->_AOEStatusEffects;
	}
	AttackAttributes tempAttackAttr(*(_AOEAttributes_attack->getCore()));
	tempAttackAttr.discretizeFull();
	AttackInitStruct AOEAttackInit(_owner.Get(), _AOEConfig_attack.Get(), tempAttackAttr, status);
	AOEAttributes tempAOEAttr(*(_AOEAttributes->getCore()));
	tempAOEAttr.discretizeFull();
	AOEInitStruct ret(AOEAttackInit, _AOEConfig.Get(), tempAOEAttr, true, FVector(0,0,0));
	return ret;
}

void ExplosiveProjectileFactory::finishUpgrade(const UAttackLevel* newLevel) {
	ProjectileFactory::finishUpgrade(newLevel);
	const auto casted = Cast<UExplosiveProjectileLevel>(newLevel);
	if (!IsValid(casted)) {
		LOGERROR("ExplosiveProjectileFactory::finishUpgrade - newLevel is not a UExplosiveProjectileLevel");
		return;
	}
	_explosiveProjectileAttributes->changeBaseValues<UExplosiveProjectileAttributeData>(casted->_explosiveProjectileAttributeOffsets.Get());
	_AOEAttributes->changeBaseValues<UAOEAttributeData>(casted->_AOEAttributeOffsets.Get());
	_AOEAttributes_attack->changeBaseValues<UAttackAttributeData>(casted->_AOEAttributeOffsets_attack.Get());
}
///////////////////////////////////////////////////////////////////////////////
// ExplosiveProjectileAttributes
void ExplosiveProjectileAttributes::tick(UObject* context, float delta, const TArray<FEffectStruct>& statusEffects) {
	auto temp = _attrRef.lock();
	if (temp.get() == nullptr)
		return;
	softReset();
	modifyAttributes(temp);
	tick_internal(context, delta, statusEffects);
}

void ExplosiveProjectileAttributes::modifyAttributes(const std::shared_ptr<const CombatantAttributes>& attr) {
	if (attr.get() == nullptr)
		return;
}
///////////////////////////////////////////////////////////////////////////////
// UExplosiveProjectileTemplate
std::unique_ptr<BaseAttackFactory> UExplosiveProjectileTemplate::createFactory(ACombatant* owner) const {
	return std::make_unique<ExplosiveProjectileFactory>(owner, this);
}