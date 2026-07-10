#include "AttackActor.h"
#include "Definitions.h"
#include "Combatant.h"
#include "Kismet/KismetMathLibrary.h"
#include "unrealHelpers.h"
#include "PaperFlipbookComponent.h"
#include "Engine/World.h"
#include "helpers.h"
#include "SpriteSorter.h"
#include "EnemyBase.h"
#include "Suplex.h"
#include "SingleTarget.h"

void AAttackActor::initialise_AAttackActor(const AttackInitStruct& temp) {
	initialise_AAttackActor(temp._pawnRef, temp._attackConfig, temp.getAttackAttributes(), temp._statusEffects);
}

// AAttackActor is responsible for detecting overlaps with combatants.
// AAttackActor subclasses are responsible for creating a hitbox of some kind for this purpose.
// As of the time of this comment, for example, AProjectile performs a sweep with a temporary sphere when it moves, while AOE creates a persistent sphere
// that automatically generates events the first time an enemy enters it as well as checking each AOE tick. 
AAttackActor::AAttackActor() {
	PrimaryActorTick.bCanEverTick = true;
	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	}
	
	// AAttackActor._flipbook does not generate ANY overlap events, manually queried or otherwise. 
	_flipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Flipbook"));
	_flipbook->SetupAttachment(RootComponent);
}

bool AAttackActor::setFlipbook(UPaperFlipbook* flipbook) {
	if (!IsValid(flipbook)) {
		LOGERROR("AAttackActor::setFlipbook - parameter not valid");
		return false;
	}
	_flipbook->SetFlipbook(flipbook);
	return true;
}

void AAttackActor::Tick(float delta) {
	updateActorScale();
}

void AAttackActor::updateActorScale() {
	SetActorScale3D(FVector(SPRITE_SCALE, SPRITE_SCALE, SPRITE_SCALE) * _attackAttributes->_radius.getFinal());
}

void AAttackActor::initialise_AAttackActor(ACombatant* pawnRef, const UAttackConfig* config, const AttackAttributes& attributes, const TArray<FEffectStruct>& statusEffects) {
	if (!IsValid(config) || !IsValid(pawnRef)) {
		LOGERROR("AAttackActor::initialise_AAttackActor - parameter not valid");
		return;
	}
	_attackConfig = TObjectPtr<const UAttackConfig>(config);
	_attackAttributes = std::make_unique<AttackAttributes>(attributes);
	_pawnRef = TWeakObjectPtr<ACombatant>(pawnRef);
	_friendly = !IsValid(Cast<AEnemyBase>(pawnRef));
	_statusEffects = statusEffects;
}

void AAttackActor::BeginPlay() {
	Super::BeginPlay();
	if (!IsValid(_attackConfig) || _attackAttributes.get() == nullptr) {
		LOGERROR("AAttackActor::BeginPlay - _attackConfig is not valid");
		return;
	}

	if (!unrealHelpers::initFlipbook(_attackConfig->_sprite.Get(), _flipbook))
		return;
	USpriteSorter* sorter = nullptr;
	if (!MyGameplayStatics::getSpriteSorter(this, sorter)) {
		LOGERROR("AMyPlayer::AMyPlayer - failed to get sprite sorter");
		return;
	}
	sorter->sortSprite<AAttackActor>(TArray<UPaperFlipbookComponent*>({ _flipbook }));
}

bool AAttackActor::canHitInstigator() const {
	for (const auto& effect : _statusEffects) {
		if (effect._type == _FRIENDLY_FIRE) {
			return true;
		}
	}
	return false;
}

void AAttackActor::applyEffect(ACombatant* target) {
	if (_attackConfig->_isCosmetic)
		return;
	if (!IsValid(target)) {
		LOGERROR("AAttackActor::applyEffect - target is not valid");
		return;
	}
	if (isFriendly(target)) {
		for (const auto& effect : _statusEffects) {
			if (effect._type == _FRIENDLY_FIRE) {
				_pawnRef->inflictStatus(FEffectStruct(_DAMAGE, effect._magnitude, 0, 1));
			}
		}
		_effectedPawns.Add(TWeakObjectPtr<APawn>(target));
		return;
	}
	if (!isHittable(target))
		return;
	// Damage is (for some reason) separate from the other status effects
	// For now let's say only base damage can crit (will change later)
	{
		float damage = _attackAttributes->_damage.getFinal();
		damage *= _nextHitModifiers._powerMultiplier;
		float critChance = _attackAttributes->_critChance.getFinal();
		if (FMath::FRand() < critChance) {
			damage *= (1 + _attackAttributes->_critMultiplier.getFinal());
		}
		FEffectStruct temp = FEffectStruct(_DAMAGE, damage, 0, 1);
		target->inflictStatus(temp);
	}
	// Chance is included in the newly created effect, just in case
	for (const auto& effect : _statusEffects) {
		if (FMath::FRand() <= effect._chance) {
			float magnitude = effect._magnitude;
			if (EStatusHelpers::canBeMultiplied(effect._type)) {
				magnitude *= _nextHitModifiers._powerMultiplier;
			}
			if (effect._type == _HEAL_INSTIGATOR) {
				if (_pawnRef.IsValid()) {
					FEffectStruct healing(_DAMAGE, -magnitude, 0, 1);
					_pawnRef->inflictStatus(healing);
				}
			}
			else if (effect._type == _RANDOM) {
				target->inflictStatus(StatusStatics::getRandomNegativePersistentEffect(magnitude, effect._duration));
			}
			else {
				FEffectStruct temp = effect;
				temp._chance = 1;
				target->inflictStatus(temp);
			}
		}
	}
	{
		APawn* temp = Cast<APawn>(target);
		if (!IsValid(temp)) {
			LOGERROR("AAttackActor::applyEffect - target is not a pawn");
			return;
		}
		_effectedPawns.Add(TWeakObjectPtr<APawn>(temp));
	}
	_nextHitModifiers = effectModifiers();
}
bool AAttackActor::isHittable(const ACombatant* target) {
	if (_attackConfig->_isCosmetic)
		return false;
	for (auto i = _effectedPawns.Num() - 1; i > -1; i--)
	{
		APawn* tempPawn = _effectedPawns[i].Get();
		if (!IsValid(tempPawn))
		{
			_effectedPawns.RemoveAt(i);
		}
		else if (tempPawn == target)
		{
			return false;
		}
	}
	return true;
}
bool AAttackActor::isFriendly(const ACombatant* target) const {
	return _pawnRef.IsValid() && _pawnRef.Get() == target;
}

///////////////////////////////////////////////////////////////////////////////

void AttackFactory::launchAttack(const FVector& forward) {
	AAttackActor* newAttack = nullptr;
	if (!unrealHelpers::spawnActorOnTopOfMeDeferred<AAttackActor>(_owner.Get(), newAttack)){
		LOGERROR("AAttackFactory::launchAttack - failed to create attack");
		return;
	}
	{
		AttackInitStruct temp = getAttackInit();
		newAttack->initialise_AAttackActor(temp);
	}
	unrealHelpers::finishDeferredSpawn<AAttackActor>(_owner.Get(), newAttack);
}

namespace {
	const UAttackConfig* extractIfValid(const UAttackTemplate* temp) {
		if (!IsValid(temp))
			return nullptr;
		return temp->_attackConfig;
	}
}

AttackFactory::AttackFactory(
	ACombatant* owner,
	const UAttackTemplate* myTemplate)
	: BaseAttackFactory(owner),
	_attackConfig(extractIfValid(myTemplate))
{
	if (!IsValid(owner) || !IsValid(myTemplate) || !IsValid(myTemplate->_attackConfig) || myTemplate->_levels.Num() - 1 < _level || myTemplate->_levels.Num() == 0 || _level < 0) {
		LOGERROR("AttackFactory::AttackFactory - invalid parameters");
		return;
	}
	for (const auto& level : myTemplate->_levels) {
		if (!isCompatible(level)) {
			LOGERROR("AttackFactory::AttackFactory - invalid level in template");
			continue;
		}
		_levels.Add(level);
	}
	auto temp = std::make_shared<AttackAttributes>(_levels[_level]->_attackOffsets, owner->getAttributes());
	_attackAttributes = std::make_unique<BaseAttributeWrapper<AttackAttributes>>(owner, temp);
}

BaseAttackFactory::BaseAttackFactory(ACombatant* owner) :
	BaseAttributeSet(),
	_owner(owner)
{
}

AttackFactory::AttackFactory(AttackFactory&& other) :
	BaseAttackFactory(std::move(other)),
	_attackAttributes(std::move(other._attackAttributes)),
	_attackConfig(other._attackConfig),
	_levels(std::move(other._levels))
{
}

BaseAttackFactory::BaseAttackFactory(BaseAttackFactory&& other) :
	BaseAttributeSet(std::move(other)),
	_level(other._level),
	_owner(other._owner)
{
}

void AttackFactory::tick(float delta) {
	if (!_attackAttributes) {
		LOGERROR("AttackFactory::tick - _attackAttributes is not initialized");
		return;
	}
	_attackAttributes->tick(delta, getStatusEffects());
	BaseAttackFactory::tick(delta);
}

void AttackFactory::setLevel(int newLevel) {
	if (newLevel == getLevel())
		return;
	if (newLevel > _levels.Num() - 1 || newLevel < 0 || _attackAttributes.get() == nullptr) {
		LOGERROR("AttackFactory::upgrade - invalid parameter");
		return;
	}
	_level = newLevel;
	const UAttackLevel* upgrade = _levels[_level].Get();
	if (!isCompatible(upgrade)) {
		LOGERROR("AttackFactory::upgrade - invalid upgrade");
		return;
	}
	finishUpgrade(upgrade);
}

void AttackFactory::finishUpgrade(const UAttackLevel* upgrade) {
	if (!isCompatible(upgrade)) {
		LOGERROR("AttackFactory::finishUpgrade - invalid upgrade");
		return;
	}
	_attackAttributes->changeBaseValues<UAttackAttributeData>(upgrade->_attackOffsets.Get());
}

AttackInitStruct AttackFactory::getAttackInit() const {
	TArray<FEffectStruct> status;
	if (_levels.Num() == 0 || _level < 0 || _level > _levels.Num() - 1) {
		LOGERROR("AttackFactory::getAttackInit - invalid level");
		status = {};
	}
	else {
		status = _levels[_level]->_statusEffects;
	}
	AttackAttributes temp(*(_attackAttributes->getCore()));
	temp.discretizeFull();
	AttackInitStruct ret(_owner.Get(), _attackConfig.Get(), temp, status);
	return ret;
}

///////////////////////////////////////////////////////////////////////////////
std::unique_ptr<BaseAttackFactory> UAttackTemplate::createFactory(ACombatant* owner) const {
	if (IsValid(_attackConfig) && _attackConfig->_isSingleTarget)
		return std::make_unique<SingleTargetFactory>(owner, this);
	else
		return std::make_unique<AttackFactory>(owner, this);
}

void UAttackTemplate::replaceOverrides() {
	_attackConfig->replaceOverrides();
	//_attackAttributes->replaceOverrides();

	if (_levels.Num() > 0) {
		auto myClass = _levels[0]->GetClass();
		for (const auto& upgrade : _levels) {
			if (upgrade->GetClass() != myClass) {
				LOGERROR("UAttackTemplate::replaceOverrides - all upgrades must be of the same class");
				continue;
			}
		}
	}
	for (auto& upgrade : _levels)
		upgrade->replaceOverrides();
}

//void UAttackAttributeData::replaceOverrides() {
//	if (helpers::isInvalidData(_damage))
//		_damage = _defaults._damage;
//	if (helpers::isInvalidData(_critChance))
//		_critChance = _defaults._critChance;
//	if (helpers::isInvalidData(_critMultiplier))
//		_critMultiplier = _defaults._critMultiplier;
//	if (helpers::isInvalidData(_radius))
//		_radius = _defaults._radius;
//}

void UAttackAttributeData::replaceOverrides() {
	for (const auto& [memberPtr, defaultVal] : DefaultProxy<UAttackAttributeData>::get())
		BASEATTRIBUTES_OVERRIDE(memberPtr, defaultVal);
}


void AttackAttributes::tick(UObject* context, float delta, const TArray<FEffectStruct>& statusEffects) {
	auto temp = _attrRef.lock();
	if (temp.get() == nullptr)
		return;
	softReset();
	modifyAttributes(temp);
	tick_internal(context, delta, statusEffects);
}

void AttackAttributes::modifyAttributes(const std::shared_ptr<const CombatantAttributes>& attr) {
	if (attr.get() == nullptr)
		return;
	_critChance.modify(_critChance.getBase() + attr->_critChance.getFinal());
	_critMultiplier.modify(_critMultiplier.getBase() + attr->_critMultiplier.getFinal());
	_radius.modify(_radius.getBase() * attr->_projectileSize.getFinal());
}

AttackAttributes::AttackAttributes(const AttackAttributes& other) : 
	BaseAttributes(other), 
	//_damage(other._damage), 
	//_critChance(other._critChance), 
	//_critMultiplier(other._critMultiplier),
	//_radius(other._radius),
	_attrRef(other._attrRef)
{
	baseInit(other);
}
AttackAttributes::AttackAttributes(AttackAttributes&& other) : 
	BaseAttributes(std::move(other)), 
	//_damage(std::move(other._damage)), 
	//_critChance(std::move(other._critChance)), 
	//_critMultiplier(std::move(other._critMultiplier)),
	//_radius(other._radius),
	_attrRef(other._attrRef)
{
	baseInit(std::move(other));
	other._attrRef.reset();
}
AttackAttributes::AttackAttributes(const UAttackAttributeData* attr, std::shared_ptr<const CombatantAttributes> attrRef) : 
	BaseAttributes(attr),
	//_damage(attr->_damage), 
	//_critChance(attr->_critChance), 
	//_critMultiplier(attr->_critMultiplier),
	//_radius(attr->_radius),
	_attrRef(attrRef)
{
	baseInit(attr);
}

#if WITH_EDITOR
void UAttackTemplate::PostEditChangeProperty(FPropertyChangedEvent& event) {
	Super::PostEditChangeProperty(event);
	if (event.GetPropertyName() == GET_MEMBER_NAME_CHECKED(UAttackTemplate, _levels)
		&& event.ChangeType == EPropertyChangeType::ArrayAdd
		&& _levels.Num() >= 2) {
		UAttackLevel* Source = _levels[_levels.Num() - 2];
		if (IsValid(Source)) {
			FName NewName = MakeUniqueObjectName(this, UAttackLevel::StaticClass());
			UAttackLevel* NewLevel = DuplicateObject<UAttackLevel>(Source, this, NewName);
			_levels.Last() = NewLevel;
		}
	}
}
#endif

void UAttackTemplate::dynamicDeepCopy(UObject* context) {
	if (!IsValid(_attackConfig)) {
		LOGERROR("Invalid attack config in attack template");
		return;
	}
	for (const auto& level : _levels) {
		if (!IsValid(level)) {
			LOGERROR("Invalid level in attack template");
			return;
		}
	}
	_attackConfig->dynamicDeepCopy(context);
	for (auto& level : _levels) {
		level->dynamicDeepCopy(context);
	}
}

void UAttackConfig::replaceOverrides() {
}

float AAttackActor::getRadius() const {
	return GetActorScale3D().X * SPRITE_RADIUS;
}