#include "Combatant.h"
//#include <functional>
#include "MyGameplayStatics.h"
#include "PaperFlipbookComponent.h"
#include "Active.h"
//#include "SpriteManager.h"
#include "AssetRefs.h"
#include "Definitions.h"
#include "Engine/AssetManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "unrealHelpers.h"
#include "helpers.h"
#include "CombatantManager.h"
#include "Passive.h"
#include "Trail.h"
#include "TagManager.h"
#include "TestingDefinitions.h"

///////////////////////////////////////////////////////////////////////////////
// ACombatant
// Lifecycle
ACombatant::ACombatant()
{
	PrimaryActorTick.bCanEverTick = true;
	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	}
	{
		_combatantFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("CombatantFlipbook"));
		_combatantFlipbook->SetupAttachment(RootComponent);
	}
	_passives = std::make_shared<std::vector<Passive>>();
	//_passiveContainer = CreateDefaultSubobject<UPassiveContainer>(TEXT("PassiveContainer"));
	_overlayFlipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("OverlayFlipbook"));
	_overlayFlipbook->SetupAttachment(RootComponent);
	_tagManager = CreateDefaultSubobject<UTagManager>(TEXT("TagManager"));
}

// preserves shared ptrs and weak ptrs to member
void ACombatant::replacePassiveList(const std::vector<Passive>& newPassives) {
	*_passives = newPassives;
	//_passives = std::make_shared<std::vector<Passive>>(newPassives);
}

void ACombatant::initialise_ACombatant(UCombatantTemplate* diskVal) {
	if (!IsValid(diskVal) || !IsValid(diskVal->_config) || !IsValid(diskVal->_attributes)) {
		LOGERROR("ACombatant::initialise_ACombatant - parameter not valid");
		return;
	}
	const UCombatantTemplate* temp = unrealHelpers::getDynamicTemplate<UCombatantTemplate>(this, diskVal);
	if (!IsValid(temp)) {
		LOGERROR("ACombatant::initialise_ACombatant - failed to get dynamic template");
		return;
	}
	_config = TObjectPtr<const UCombatantConfig>(temp->_config);
	replaceAttributeData(temp->_attributes);
	setupFlipbook();
}

bool ACombatant::initialise_ACombatant_noDynamic(const UCombatantConfig* config, const UCombatantAttributeData* data) {
	if (!IsValid(config)) {
		LOGERROR("ACombatant::initialise_ACombatant_configOnly - parameter not valid");
		return false;
	}
	_config = config;
	if (IsValid(data)){
		replaceAttributeData(data);
	}
	return true;
}

void ACombatant::replaceAttributeData(const UCombatantAttributeData* data) {
	if (!IsValid(data)) {
		LOGERROR("ACombatant::replaceAttributeData - parameter not valid");
		return;
	}
	if (_attributeSet != nullptr) {
		_attributeSet->changeBaseValues(data);
	}
	else
		_attributeSet = std::make_unique<CombatantAttributeSet>(this, data);
}

void ACombatant::BeginPlay() {
	Super::BeginPlay();
	if (!IsValid(_config.Get()) || _attributeSet.get() == nullptr) {
		LOGERROR("ACombatant::BeginPlay - _config or _attributeSet not valid");
		return;
	}
	for (const auto& data : _config->_startingWeapons) {
		if (canGiveWeapon(data))
			giveWeapon_safe(data);
	}
	for (const auto& data : _config->_startingPassives) {
		if (canGivePassive())
			givePassive_safe(data);
	}
	unrealHelpers::initFlipbook(_config->_sprite.Get(), _combatantFlipbook);
	_myForwardVector = GetActorForwardVector();

	UAssetRefs* refs = nullptr;
	if (!MyGameplayStatics::getAssetRefs(refs)) {
		LOGERROR("ACombatant::ACombatant - failed to get asset refs");
		return;
	}
	unrealHelpers::initFlipbook(refs->getTestOverlay(), _overlayFlipbook);
	_overlayFlipbook->SetHiddenInGame(true);
}
UE_DISABLE_OPTIMIZATION
void ACombatant::Tick(float DeltaTime) {
	TRACE_CPUPROFILER_EVENT_SCOPE(ACombatant_Tick);
	Super::Tick(DeltaTime);
	float oldHP = _attributeSet->getMember(&CombatantAttributes::_currentHP);
	_attributeSet->tick(DeltaTime);
	_attributeSet->handlePoison(this);
	float newHP = _attributeSet->getMember(&CombatantAttributes::_currentHP);
	if (!helpers::nearEq(oldHP, newHP)) {
		if (onCurrentHPChanged(oldHP, newHP))
			return;
	}
	for (auto& active : _activeAbilities) {
		if (active.get() == nullptr) {
			LOGERROR("ACombatant::Tick - uninitialized active");
			continue;
		}
		active->tick(DeltaTime, _myForwardVector);
	}
	SetActorScale3D(FVector(SPRITE_SCALE,SPRITE_SCALE,SPRITE_SCALE) * _attributeSet->getMember(&CombatantAttributes::_selfSize));
	updateTemporaryAppearanceChanges();
}
UE_ENABLE_OPTIMIZATION

void ACombatant::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// Getters
float ACombatant::getAttributeMember(Stat CombatantAttributes::* member) const {
	return _attributeSet->getMember(member);
}
std::shared_ptr<const CombatantAttributes> ACombatant::getAttributes() const { return _attributeSet->getAttributeWrapper().getCore(); }
float ACombatant::getHP() const { return getAttributeMember(&CombatantAttributes::_currentHP); }
float ACombatant::getMaxHP() const { return getAttributeMember(&CombatantAttributes::_maxHP); }
bool ACombatant::canGiveWeapon(const UWeaponTemplate* weapon) const {
	return _activeAbilities.size() < _MAX_WEAPONS;
}
bool ACombatant::canUpgradeWeapon() const {
	for (const auto& active : _activeAbilities) {
		if (active.get() == nullptr) {
			LOGERROR("ACombatant::canUpgradeWeapon - uninitialized active");
			continue;
		}
		if (active->isUpgradable())
			return true;
	}
	return false;
}
bool ACombatant::canUpgradePassive() const {
	//return _passiveContainer->_passives.Num() > 0;
	for (const auto& passive : *_passives) {
		if (passive.isUpgradable())
			return true;
	}
	return false;
}
TArray<UWeaponTemplate*> ACombatant::getUpgradableWeapons() const {
	TArray<UWeaponTemplate*> result;
	for (const auto& active : _activeAbilities) {
		if (active.get() == nullptr) {
			LOGERROR("ACombatant::getUpgradableWeapons - uninitialized active");
			continue;
		}
		if (active->isUpgradable()) {
			result.Add(active->getDiskTemplate());
		}
	}
	return result;
}

TArray<UPassiveData*> ACombatant::getUpgradablePassives() const {
	//return _passiveContainer->getDiskPassives();
	TArray<UPassiveData*> result;
	for (const auto& passive : *_passives) {
		if (passive.isUpgradable()) {
			result.Add(passive.getDiskData());
		}
	}
	return result;
}
std::shared_ptr<Active> ACombatant::getActive(int index) const {
	if (index < 0 || index >= static_cast<int>(_activeAbilities.size())) {
		LOGERROR("ACombatant::getActive - index out of bounds");
		return nullptr;
	}
	return _activeAbilities[index];
}
//TArray<UCombatantPassive*> UPassiveContainer::getDiskPassives() const {
//	UDynamicAssetManager* manager = nullptr;
//	if (!MyGameplayStatics::getDynamicAssetManager(this, manager)) {
//		LOGERROR("ACombatant::getUpgradablePassives - failed to get dynamic asset manager");
//		return TArray<UCombatantPassive*>();
//	}
//	TArray<UCombatantPassive*> result;
//	for (const auto& passive : _passives) {
//		UCombatantPassive* temp = manager->getKey(passive.Get());
//		if (temp != nullptr)
//			result.Add(temp);
//	}
//	return result;
//}

TArray<UWeaponTemplate*> ACombatant::getMaxedWeapons() const {
	TArray<UWeaponTemplate*> result;
	for (const auto& active : _activeAbilities) {
		if (active.get() == nullptr) {
			LOGERROR("ACombatant::getMaxedWeapons - uninitialized active");
			continue;
		}
		if (!active->isUpgradable()) {
			result.Add(active->getDiskTemplate());
		}
	}
	return result;
}
TArray<UWeaponTemplate*> ACombatant::getAllWeapons() const {
	TArray<UWeaponTemplate*> result;
	for (const auto& active : _activeAbilities) {
		if (active.get() == nullptr) {
			 LOGERROR("ACombatant::getAllWeapons - uninitialized active");
			 continue;
		}
		result.Add(active->getDiskTemplate());
	}
	return result;
}
int ACombatant::getWeaponLevel(UWeaponTemplate* data) const {
	for (const auto& weapon : _activeAbilities) {
		if (weapon.get() == nullptr) {
			LOGERROR("ACombatant::getWeaponLevel - uninitialized active");
			continue;
		}
		if ((*weapon) == data)
			return weapon->getLevel();
	}
	return -1;
}
int ACombatant::getPassiveLevel(UPassiveData* data) const {
	for (const auto& passive : *_passives) {
		if (passive == data)
			return passive.getLevel();
	}
	return -1;
}
TArray<UPassiveData*> ACombatant::getAllPassives() const {
	//return _passiveContainer->getDiskPassives();
	TArray<UPassiveData*> result;
	for (const auto& passive : *_passives) {
		result.Add(passive.getDiskData());
	}
	return result;
}
// Mutators
void ACombatant::giveWeapon(UWeaponTemplate* temp, int socket) {
	if (!canGiveWeapon(temp)) {
		LOGERROR("ACombatant::giveWeapon - cannot give weapon");
		return;
	}
	const UWeaponTemplate* dynamic = unrealHelpers::getDynamicTemplate<UWeaponTemplate>(this, temp);
	giveWeapon_safe(dynamic, socket);
}
void ACombatant::giveWeapon_safe(const UWeaponTemplate* temp, int socket) {
	if (!IsValid(temp)) {
		LOGERROR("ACombatant::giveWeapon_safe - parameter not valid");
		return;
	}
	if (IsValid(Cast<UWeaponTemplate_Base>(temp)))
		_activeAbilities.push_back(std::make_shared<Active_WeaponBase>(this, Cast<UWeaponTemplate_Base>(temp)));
	else
		_activeAbilities.push_back(std::make_shared<Active>(this, temp));
}
bool ACombatant::setWeaponLevel(UWeaponTemplate* temp, int newLevel) {
	if (!IsValid(temp)) {
		LOGERROR("ACombatant::upgradeWeapon - parameter not valid");
		return false;
	}
	for (auto& active : _activeAbilities) {
		if (active.get() == nullptr) {
			LOGERROR("ACombatant::upgradeWeapon - uninitialized active");
			continue;
		}
		if ((*active) == temp) {
			if (!active->setLevel(newLevel)) {
				LOGERROR("ACombatant::upgradeWeapon - failed to set level");
				continue;
			}
			return true;
		}
	}
	LOGERROR("ACombatant::upgradeWeapon - no active ability upgraded");
	return false;
}
void ACombatant::upgradePassive(UPassiveData* temp) {
	if (!IsValid(temp)) {
		LOGERROR("ACombatant::upgradePassive - parameter not valid");
		return;
	}
	for (auto& passive : *_passives) {
		if (passive == temp) {
			passive.upgrade();
			return;
		}
	}
	LOGERROR("ACombatant::upgradeWeapon - no active passive upgraded");
}

void ACombatant::givePassive(UPassiveData* temp) {
	const UPassiveData* dynamic = unrealHelpers::getDynamicTemplate<UPassiveData>(this, temp);
	givePassive_safe(dynamic);
}
void ACombatant::givePassive_safe(const UPassiveData* temp) {
	if (!IsValid(temp)) {
		LOGERROR("ACombatant::givePassive_safe - parameter not valid");
		return;
	}
	_passives->push_back(Passive(temp));
}

void ACombatant::inflictStatus(const FEffectStruct& status) {
	// Hexproof
	if (helpers::nearEq(getAttributeMember(&CombatantAttributes::_damageReduction_percent), 999))
		return;
	for (auto& active : _activeAbilities) {
		if (active.get() == nullptr) {
			LOGERROR("ACombatant::inflictStatus - uninitialized active");
			continue;
		}
		active->inflictStatus(status);
	}
	_attributeSet->inflictStatus(status);
}

// bleed is handled asynchronously, but ostensibly this function "owns" it
void ACombatant::updateTemporaryAppearanceChanges() {
	bool hasAfflictionStatus = false;
	bool hasBleedStatus = false;
	float bleedDuration = -1;
	for (const auto& status : _attributeSet->getStatusEffectsCopy()) {
		hasAfflictionStatus |= status._duration > EPSILON;
		if (status._type == _BLEED) {
			hasBleedStatus = true;
			bleedDuration = std::max(bleedDuration, status._duration);
		}
	}
	_overlayFlipbook->SetHiddenInGame(!hasAfflictionStatus);
	if (hasBleedStatus) {
		if (!_activeChanges._bleed)
			startBleeding(bleedDuration);
	}
	else {
		_activeChanges._bleed = false;
	}
}

bool ACombatant::startBleeding(float totalDuration) {
	const static float BLEED_SCALE = 1.0f;
	const static float BLEED_PERIOD = 0.1f;
	const static float BLEED_SEGMENT_LIFETIME = 2;

	UAssetRefs* refs = nullptr;
	if (!MyGameplayStatics::getAssetRefs(refs)) {
		LOGERROR("ACombatant::inflictStatus_bleedCheck - failed to get asset refs");
		return false;
	}
	TrailSegmentCosmeticInitStruct init;
	init._totalDuration = totalDuration;
	init._sprite = refs->getBleedSprite();
	init._scale = BLEED_SCALE * getAttributeMember(&CombatantAttributes::_selfSize);
	init._duration = BLEED_SEGMENT_LIFETIME;

	ATrailSegmentFactory* trail = nullptr;
	if (!unrealHelpers::spawnActorOnTopOfMeDeferred<ATrailSegmentFactory>(this, trail)) {
		LOGERROR("ACombatant::inflictStatus_bleedCheck - failed to spawn trail segment factory");
		return false;
	}
	if (!trail->initialise_ATrailSegmentFactory(BLEED_PERIOD, init, this)) {
		LOGERROR("ACombatant::inflictStatus_bleedCheck - init failed");
		trail->Destroy();
		return false;
	}
	if (!unrealHelpers::finishDeferredSpawn<ATrailSegmentFactory>(this, trail)) {
		LOGERROR("ACombatant::inflictStatus_bleedCheck - deferred spawn failed");
		trail->Destroy();
		return false;
	}
	_activeChanges._bleed = true;
	return true;
}

void ACombatant::lookAtDirection(float X, float Z) {
	const FRotator rotation = UKismetMathLibrary::FindLookAtRotation(FVector(0, 0, 0), FVector(X, 0, Z));
	_myForwardVector = rotation.RotateVector(FVector(1, 0, 0));
}

bool ACombatant::hasStatus(EStatus status) const {
	return _attributeSet->hasStatus(status);
}

void ACombatant::exchangeContactDamage(ACombatant* left, ACombatant* right) {
	if (!IsValid(left) || !IsValid(right)) {
		LOGERROR("ACombatant::exchangeContactDamage - parameter not valid");
		return;
	}
	if (left->hasStatus(_CONTACT_IMMUNE) || right->hasStatus(_CONTACT_IMMUNE))
		return;
	const float leftThreat = left->_attributeSet->getMember(&CombatantAttributes::_contactDamage);
	const float rightThreat = right->_attributeSet->getMember(&CombatantAttributes::_contactDamage);
	FEffectStruct leftEffect = FEffectStruct(_DAMAGE, leftThreat, 0, 1);
	FEffectStruct rightEffect = FEffectStruct(_DAMAGE, rightThreat, 0, 1);
	left->inflictStatus(rightEffect);
	right->inflictStatus(leftEffect);
}

// Callbacks
bool ACombatant::onCurrentHPChanged(float oldVal, float newVal)
{
	{
		const Stat& HP = _attributeSet->getMember(&CombatantAttributes::_currentHP);
		if (!helpers::nearEq(HP._modifier, 0) || !helpers::nearEq(HP._postbonus, 0) || !helpers::nearEq(HP._multiplier, 0)) {
			LOGERROR("ACombatant::onCurrentHPChanged - for simplicity, all values of currentHP should be 0 except for base and offset- it's essentially a derived stat, exempt from direct effects from status effects and passives. Prebonus is an exception because it is used to track the passive effect on maxHP without drifting.");
		}
	}

	const float diff = oldVal - newVal;
	if (SHOW_ALL_NUMBERS || abs(diff) > _DAMAGE_THRESHOLD) {
		const float currentScale = GetActorScale3D().X;
		const float myHeight = currentScale * SPRITE_RADIUS * 2;
		unrealHelpers::spawnDamageNumberNearMe(this, FVector(0, 0, myHeight * 0.7), diff);
	}
	// Using epsilon is important because instant death sets health to 0
	if (newVal <= EPSILON) {
		onKilled();
		return true;
	}
	return false;
}

void ACombatant::onKilled() {
	Destroy();
}
///////////////////////////////////////////////////////////////////////////////
// CombatantAttributes
CombatantAttributes::CombatantAttributes(const UCombatantAttributeData* attr, std::shared_ptr<const std::vector<Passive>> passives) :
	BaseAttributes(attr),
	//_maxHP(attr->_maxHP),
	//_damageReduction_flat(attr->_damageReduction_flat),
	//_damageReduction_percent(attr->_damageReduction_percent),
	//_healthRegen_flat(attr->_healthRegen_flat),
	//_healthRegen_percent(attr->_healthRegen_percent),
	//_critChance(attr->_critChance),
	//_critMultiplier(attr->_critMultiplier),
	//_attackSpeed(attr->_attackSpeed),
	//_bonusBounces(attr->_bonusBounces),
	//_bonusPierce(attr->_bonusPierce),
	//_bonusProjectiles(attr->_bonusProjectiles),
	//_projectileSpeed(attr->_projectileSpeed),
	//_projectileSize(attr->_projectileSize),
	//_movementSpeed(attr->_movementSpeed),
	//_range(attr->_range),
	//_contactDamage(attr->_contactDamage),
	//_selfSize(attr->_selfSize),
	//_iFrameDuration(attr->_iFrameDuration),
	_passives(passives)
{
	baseInit(attr);
	// I recommend setting maxHP and currentHP to the same values for clarity, but it's enforced here just in case
	_currentHP.setBase(_maxHP.getBase());
}

CombatantAttributes::CombatantAttributes(const UCombatantAttributeData* attr) :
	BaseAttributes(attr),
	_passives()
{
	baseInit(attr);
	// I recommend setting maxHP and currentHP to the same values for clarity, but it's enforced here just in case
	_currentHP.setBase(_maxHP.getBase());
}

CombatantAttributes::CombatantAttributes(const CombatantAttributes& other) :
	BaseAttributes(other),
	/*_maxHP(other._maxHP),
	_currentHP(other._currentHP),
	_damageReduction_flat(other._damageReduction_flat),
	_damageReduction_percent(other._damageReduction_percent),
	_healthRegen_flat(other._healthRegen_flat),
	_healthRegen_percent(other._healthRegen_percent),
	_critChance(other._critChance),
	_critMultiplier(other._critMultiplier),
	_attackSpeed(other._attackSpeed),
	_bonusBounces(other._bonusBounces),
	_bonusPierce(other._bonusPierce),
	_bonusProjectiles(other._bonusProjectiles),
	_projectileSpeed(other._projectileSpeed),
	_projectileSize(other._projectileSize),
	_movementSpeed(other._movementSpeed),
	_range(other._range),
	_contactDamage(other._contactDamage),
	_selfSize(other._selfSize),
	_iFrameDuration(other._iFrameDuration),*/
	_passives(other._passives)
{
	baseInit(other);
}

CombatantAttributes::CombatantAttributes(CombatantAttributes&& other) :
	BaseAttributes(std::move(other)),
	//_maxHP(std::move(other._maxHP)),
	//_currentHP(std::move(other._currentHP)),
	//_damageReduction_flat(std::move(other._damageReduction_flat)),
	//_damageReduction_percent(std::move(other._damageReduction_percent)),
	//_healthRegen_flat(std::move(other._healthRegen_flat)),
	//_healthRegen_percent(std::move(other._healthRegen_percent)),
	//_critChance(std::move(other._critChance)),
	//_critMultiplier(std::move(other._critMultiplier)),
	//_attackSpeed(std::move(other._attackSpeed)),
	//_bonusBounces(std::move(other._bonusBounces)),
	//_bonusPierce(std::move(other._bonusPierce)),
	//_bonusProjectiles(std::move(other._bonusProjectiles)),
	//_projectileSpeed(std::move(other._projectileSpeed)),
	//_projectileSize(std::move(other._projectileSize)),
	//_movementSpeed(std::move(other._movementSpeed)),
	//_range(std::move(other._range)),
	//_contactDamage(std::move(other._contactDamage)),
	//_selfSize(std::move(other._selfSize)),
	//_iFrameDuration(std::move(other._iFrameDuration)),
	_passives(other._passives)
{
	baseInit(std::move(other));
	other._passives.reset();
}

void CombatantAttributes::tick(UObject* context, float delta, const TArray<FEffectStruct>& statusEffects) {
	auto tempPassives = _passives.lock();
	if (!tempPassives)
		return;
	BaseAttributes::softReset();
	// So that raw HP is accurate. Uses all stats for forward compatability
	applyToAllStats([](Stat& stat) { Stat::calculateFinal(stat); });
	float rawMaxHP = _maxHP.getFinal();
	for (const auto& passive : *tempPassives) {
		const auto prebonus = passive.getPrebonus();
		const auto postbonus = passive.getPostbonus();
		const auto multiplier = passive.getMultiplier();
		if (!IsValid(prebonus) || !IsValid(postbonus) || !IsValid(multiplier))
			continue;
		prebonusAdd(prebonus);
		postbonusAdd(postbonus);
		multiplierAdd(multiplier);
	}
	// When you increase your maxHP by any means other than passives, your current HP stays the same. 
	// When you increase your maxHP by passives, your current HP increases by the same amount.
	// When you decrease your maxHP by any means, your currentHP stays the same but is capped at maxHP (this is treated as transient damage)
	// To simplify matters, currentHP should only be changed via base, offset, and prebonus meaning that it's only changed by damage, healing, and right here.
	
	// So that passive HP is accurate. Uses all stats for forward compatability
	applyToAllStats([](Stat& stat) { Stat::calculateFinal(stat); });
	_currentHP._prebonus = _maxHP.getFinal() - rawMaxHP;

	// calculate derived stats or stat effects
	_currentHP._offset += _healthRegen_flat.getFinal() * delta;
	_currentHP._offset += 0.01 * _healthRegen_percent.getFinal() * _maxHP.getFinal() * delta;

	BaseAttributes::tick_internal(context, delta, statusEffects);
	if (_currentHP.getFinal() > _maxHP.getFinal()) {
		_currentHP._offset -= _currentHP.getFinal() - _maxHP.getFinal();
	}
}

void CombatantAttributes::discretizeFull() {
	_bonusBounces.discretize();
	_bonusPierce.discretize();
	_bonusProjectiles.discretize();
}

void CombatantAttributes::applyStatus(UObject* context, const FEffectStruct& status, float delta) {
	// Require nothing
	if (status._type == _DAMAGE) {
		_currentHP._offset -= status._magnitude;
	}
	else if (status._type == _BLEED)
		_currentHP._offset -= status._magnitude * delta;
	else if (status._type == _EXECUTE) {
		if (_currentHP.getFinal() / _maxHP.getFinal() <= status._magnitude / 100.0f)
			zeroStat(&_currentHP);
	}
	else if (status._type == _CHILL || status._type == _SLOW) {
		_movementSpeed._multiplier -= status._magnitude / 100.0f;
	}
	else if (status._type == _DECAY_INSTANT) {
		_currentHP._offset -= (_maxHP.getFinal() - _currentHP.getFinal()) * (status._magnitude / 100.0f);
	}
	else if (status._type == _DEATH) {
		zeroStat(&_currentHP);
	}
	// Require combatant manager
	else if (status._type == _BURN || status._type == _POISON || status._type == _DECAY) {
		UCombatantManager* manager = nullptr;
		if (!MyGameplayStatics::getCombatantManager(context, manager)) {
			LOGERROR("CombatantAttributes::applyStatus - could not get combatant manager for burn damage");
			return;
		}
		if (status._type == _BURN) {
			if (manager->getBurnThisFrame())
				_currentHP._offset -= (status._magnitude / 100.0f) * _maxHP.getFinal();
		}
		else if (status._type == _POISON) {
			//magnitude multiplier is handled by BaseAttributeSet
			if (manager->getPoisonThisFrame())
				_currentHP._offset -= status._magnitude;
		}
		else if (status._type == _DECAY) {
			if (manager->getBurnThisFrame())
				_currentHP._offset -= (status._magnitude / 100.0f) * (_maxHP.getFinal() - _currentHP.getFinal());
		}
	}
	else
		return;
}

CombatantAttributes& CombatantAttributes::prebonusAdd(const UCombatantAttributeData* other) {
	const std::vector<float> otherStats = other->getStatVector();
	int i = 0;
	applyToAllStats([&otherStats, &i](Stat& stat) {
		stat._prebonus += otherStats[i++];
		});
	return *this;
}
CombatantAttributes& CombatantAttributes::postbonusAdd(const UCombatantAttributeData* other) {
	const std::vector<float> otherStats = other->getStatVector();
	int i = 0;
	applyToAllStats([&otherStats, &i](Stat& stat) {
		stat._postbonus += otherStats[i++];
		});
	return *this;
}
CombatantAttributes& CombatantAttributes::multiplierAdd(const UCombatantAttributeData* other) {
	const std::vector<float> otherStats = other->getStatVector();
	int i = 0;
	applyToAllStats([&otherStats, &i](Stat& stat) {
		stat._multiplier += otherStats[i++];
		});
	return *this;
}
std::vector<float> UCombatantAttributeData::getStatVector() const {
	std::vector<float> stats;
	applyToAllStats([&stats](const float& stat) {
		stats.push_back(stat);
		});
	return stats;
}
///////////////////////////////////////////////////////////////////////////////
// CombatantAttributeSet
CombatantAttributeSet::CombatantAttributeSet(ACombatant* owner, const UCombatantAttributeData* data) {
	auto temp = std::make_shared<CombatantAttributes>(data, owner->getPassives());
	_attributes = std::make_unique<BaseAttributeWrapper<CombatantAttributes>>(owner, temp);
}

CombatantAttributeSet::CombatantAttributeSet(CombatantAttributeSet&& other) :
	_attributes(std::move(other._attributes))
{
	other._attributes = nullptr;
}

void CombatantAttributeSet::tick(float delta) {
	if (_attributes.get() == nullptr) {
		LOGERROR("CombatantAttributeSet::tick - attributes not initialized");
		return;
	}
	
	// If in iFrames, remove flat damage effects
	if (_iFrameTimeRemaining > EPSILON) {
		removeStatusesOfType(_DAMAGE);
	}
	// If not in iFrames, begin iFrames if there is at least 1 flat damage effect, then apply all flat damage effects that are queued this frame
	else {
		for (const auto& effect : getStatusEffects()) {
			if (effect._type == _DAMAGE) {
				_iFrameTimeRemaining = _attributes->getMember(&CombatantAttributes::_iFrameDuration);
				break;
			}
		}
	}
	_attributes->tick(delta, getStatusEffects());
	if (_iFrameTimeRemaining > -EPSILON)
		_iFrameTimeRemaining -= delta;
	BaseAttributeSet::tick(delta);
}
///////////////////////////////////////////////////////////////////////
// UCombatantAttributeData
//void UCombatantAttributeData::replaceOverrides() {
//	if (helpers::isInvalidData(_maxHP))
//		_maxHP = _defaults._maxHP;
//	if (helpers::isInvalidData(_currentHP))
//		_currentHP = _defaults._currentHP;
//	if (helpers::isInvalidData(_damageReduction_flat))
//		_damageReduction_flat = _defaults._damageReduction_flat;
//	if (helpers::isInvalidData(_damageReduction_percent))
//		_damageReduction_percent = _defaults._damageReduction_percent;
//	if (helpers::isInvalidData(_healthRegen_flat))
//		_healthRegen_flat = _defaults._healthRegen_flat;
//	if (helpers::isInvalidData(_healthRegen_percent))
//		_healthRegen_percent = _defaults._healthRegen_percent;
//	if (helpers::isInvalidData(_critChance))
//		_critChance = _defaults._critChance;
//	if (helpers::isInvalidData(_critMultiplier))
//		_critMultiplier = _defaults._critMultiplier;
//	if (helpers::isInvalidData(_attackSpeed))
//		_attackSpeed = _defaults._attackSpeed;
//	if (helpers::isInvalidData(_bonusBounces))
//		_bonusBounces = _defaults._bonusBounces;
//	if (helpers::isInvalidData(_bonusPierce))
//		_bonusPierce = _defaults._bonusPierce;
//	if (helpers::isInvalidData(_bonusProjectiles))
//		_bonusProjectiles = _defaults._bonusProjectiles;
//	if (helpers::isInvalidData(_projectileSpeed))
//		_projectileSpeed = _defaults._projectileSpeed;
//	if (helpers::isInvalidData(_projectileSize))
//		_projectileSize = _defaults._projectileSize;
//	if (helpers::isInvalidData(_movementSpeed))
//		_movementSpeed = _defaults._movementSpeed;
//	if (helpers::isInvalidData(_range))
//		_range = _defaults._range;
//	if (helpers::isInvalidData(_contactDamage))
//		_contactDamage = _defaults._contactDamage;
//	if (helpers::isInvalidData(_selfSize))
//		_selfSize = _defaults._selfSize;
//	if (helpers::isInvalidData(_iFrameDuration))
//		_iFrameDuration = _defaults._iFrameDuration;
//};
void UCombatantAttributeData::replaceOverrides() {
	for (const auto& [memberPtr, defaultVal] : DefaultProxy<UCombatantAttributeData>::get()) {
		BASEATTRIBUTES_OVERRIDE(memberPtr, defaultVal);
	}
	_maxHP *= COMBATANT_HEALTH_MULTIPLIER;
	_currentHP = _maxHP;
}
///////////////////////////////////////////////////////////////////////////////
// UCombatantConfig
void UCombatantConfig::replaceOverrides() {
	if (unrealHelpers::isInvalidData(_name))
		_name = _defaults._name;
	if (unrealHelpers::isInvalidData<ACombatant>(_combatantClass))
		_combatantClass = _defaults._combatantClass;
	for (auto& data : _startingPassives)
	{
		if (!IsValid(data)) {
			LOGERROR("UCombatantConfig::replaceOverrides - invalid passive in starting passives");
			continue;
		}
		data->replaceOverrides();
	}
	for (auto& data : _startingWeapons) {
		if (!IsValid(data)) {
			LOGERROR("UCombatantConfig::replaceOverrides - invalid weapon in starting weapons");
			continue;
		}
		data->replaceOverrides();
	}
}
void UCombatantConfig::dynamicDeepCopy(UObject* context) {
	for (const auto& weapon : _startingWeapons) {
		if (!IsValid(weapon)) {
			LOGERROR("UCombatantConfig::dynamicDeepCopy - invalid weapon in weapons");
			return;
		}
	}
	for (const auto& passive : _startingPassives) {
		if (!IsValid(passive)) {
			LOGERROR("UCombatantConfig::dynamicDeepCopy - invalid passive in passives");
			return;
		}
	}
	for (auto& weaponPtr : _startingWeapons) {
		weaponPtr = unrealHelpers::getDynamicTemplate(context, weaponPtr.Get());
	}
	for (auto& passivePtr : _startingPassives) {
		passivePtr = unrealHelpers::getDynamicTemplate(context, passivePtr.Get());
	}
}

std::shared_ptr<Active> ACombatant::getActiveBaseFromEvolution(const UWeaponTemplate_Evolution* evo) const {
	for (auto& active : _activeAbilities) {
		Active_WeaponBase* casted = dynamic_cast<Active_WeaponBase*>(active.get());
		if (casted != nullptr && casted->getEvolution() == evo) {
			return active;
		}
	}
	return nullptr;
}
TMap<UWeaponTemplate_Evolution*, bool> ACombatant::getEligibleEvolutionList() {
	TMap<UWeaponTemplate_Evolution*, bool> ret;
	for (const auto& active : _activeAbilities) {
		Active_WeaponBase* casted = dynamic_cast<Active_WeaponBase*>(active.get());
		if (casted == nullptr)
			continue;
		if (!casted->isEvolutionEligible())
			continue;
		ret.Add(casted->getEvolution(), casted->wasTurnedDown());
	}
	return ret;
}

bool ACombatant::evolveWeapon(UWeaponTemplate_Evolution* evolution) {
	for (auto it = _activeAbilities.begin(); it != _activeAbilities.end(); it++) {
		Active_WeaponBase* casted = dynamic_cast<Active_WeaponBase*>((*it).get());
		if (casted == nullptr)
			continue;
		if (casted->getEvolution() == evolution) {
			auto dynamic = unrealHelpers::getDynamicTemplate<UWeaponTemplate_Evolution>(this, evolution);
			_activeAbilities.erase(it);
			_activeAbilities.push_back(std::make_shared<Active>(this, dynamic));
			return true;
		}
	}
	LOGERROR("ACombatant::evolveWeapon - no active ability found for evolution");
	return false;
}

bool ACombatant::turnDownEvolution(UWeaponTemplate_Evolution* evolution) {
	for (const auto& active : _activeAbilities) {
		Active_WeaponBase* casted = dynamic_cast<Active_WeaponBase*>(active.get());
		if (casted == nullptr)
			continue;
		if (casted->getEvolution() == evolution) {
			casted->turnDownEvolution();
			return true;
		}
	}
	LOGERROR("ACombatant::turnDownEvolution - no active ability found for evolution");
	return false;
}

void CombatantAttributes::getSummonPassives(std::vector<Passive>& ret) const {
	// for now, no combatant stats are relevant to summons
	ret.clear();
}

void CombatantAttributeSet::changeBaseValues(const UCombatantAttributeData* newBase) {
	if (_attributes.get() == nullptr) {
		LOGERROR("CombatantAttributeSet::changeBaseValues - attributes not initialized");
		return;
	}
	_attributes->changeBaseValues<UCombatantAttributeData>(newBase);
}

bool ACombatant::moveTowardsDirection(const FVector& direction, float delta, float customSpeedMultiplier) {
	if (_attributeSet->hasStatus(_ROOT))
		return true;
	//TRACE_CPUPROFILER_EVENT_SCOPE(ACombatant_Move);
	if (_attributeSet.get() == nullptr) {
		LOGERROR("ACombatant::moveTowardsDirection - attribute set not initialized");
		return false;
	}
	const float multiplier = delta * _MOVEMENT_MULTIPLIER * _attributeSet->getMember(&CombatantAttributes::_movementSpeed) * customSpeedMultiplier;
	AddActorWorldOffset(direction * multiplier, false, nullptr, ETeleportType::TeleportPhysics);
	return true;
}

bool ACombatant::getMoveSpeed(float& ret) const {
	if (_attributeSet.get() == nullptr) {
		LOGERROR("ACombatant::getMoveSpeed - attribute set not initialized");
		return false;
	}
	ret = _attributeSet->getMember(&CombatantAttributes::_movementSpeed) * _MOVEMENT_MULTIPLIER;
	return true;
}

FVector ACombatant::getSpriteBoundary(const FVector& forward) const {
	const float size = SPRITE_RADIUS * _attributeSet->getMember(&CombatantAttributes::_selfSize);
	return GetActorLocation() + forward * size;
}

void CombatantAttributes::postTick(float delta) {
	BaseAttributes::postTick(delta);
	if (_currentHP.getFinal() > _maxHP.getFinal()) {
		_currentHP._offset -= _currentHP.getFinal() - _maxHP.getFinal();
		Stat::calculateFinal(_currentHP);
	}
}

int ACombatant::inflictPermanentStatus(const FEffectStruct& status) {
	return _attributeSet->inflictPermanentStatus(status);
}
bool ACombatant::removePermanentStatus(int id) {
	return _attributeSet->removePermanentStatus(id);
}

void ACombatant::addCollider(UShapeComponent* collider, std::function<void(ACombatant*, ACombatant*)> callback) {
	_collideCallbacks.Add(collider, callback);
	collider->Attach
}