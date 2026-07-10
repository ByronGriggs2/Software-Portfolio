#include "AOE.h"
#include "Definitions.h"
#include "Kismet/KismetMathLibrary.h"

#include "Components/ShapeComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Combatant.h"
#include "unrealHelpers.h"
#include "helpers.h"
#include "AOESpawner.h"
#include "ExplosiveAOE.h"
#include "ProjectileSword.h"
#include "AttackTrail.h"
#include "Backstab.h"
#include "MyPlayer.h"
#include "CustomChannels.h"
///////////////////////////////////////////////////////////////////////////////
// AAOE
void AAOE::initialise_AAOE(const AOEInitStruct& temp) 
{
	AAttackActor::initialise_AAttackActor(temp._attack);
	_AOEConfig = temp._AOEConfig;
	_AOEAttributes = std::make_unique<AOEAttributes>(temp._AOEAttributes);
	_initialisedWithDelay = temp._delayFullConstruction;
	unrealHelpers::lookAtDirection(this, temp._arcShape_forwardVector);
	if (temp._delayFullConstruction) {
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
		SetActorTickEnabled(false);
	}
}

void AAOE::BeginPlay() {
	Super::BeginPlay();
	if (!IsValid(_AOEConfig.Get()) || _AOEAttributes.get() == nullptr || !IsValid(RootComponent)) {
		LOGERROR("AAOE::BeginPlay - invalid parameters");
		return;
	}
	if (!_initialisedWithDelay) {
		initShape();
		SetActorTickEnabled(true);
	}
}

void AAOE::completeDelayedConstruction() {
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);
	_initialisedWithDelay = false;
	initShape();
}

void AAOE::Tick(float delta) {
	if (_AOEConfig->_shape == _ARC || _AOEConfig->_targeting == _FOLLOW_INSTIGATOR) {
		reorientSlash();
	}
	if (_AOEAttributes->_duration.getFinal() <= 0) {
		if (_consumedDuration >= 0.25) {
			Destroy();
			return;
		}
		if (_consumedDuration > 0)
			_isAfterimage = true;
	}
	else if (_consumedDuration >= _AOEAttributes->_duration.getFinal()) {
		Destroy();
		return;
	}
	else if (_AOEConfig->_effectApplication == _TICK) {
		if (helpers::nearEq(_nextTick, 999)) {
			LOGERROR("AAOE::Tick - _nextTick was not set during construction");
			return;
		}
		_nextTick -= delta;
		if (_nextTick <= 0) {
			collideWithAll();
			_nextTick = _AOEConfig->_tickInterval;
		}
	}
	_consumedDuration += delta;
	AAttackActor::Tick(delta);
}

UShapeComponent* AAOE::makeDefaultSphereCollider() {
	auto myLocation = GetActorLocation();
	auto collider = NewObject<USphereComponent>(this);
	// The radius modifier in AttackAttributes is applied to the entire actor, including this collider, in AAttackActor::Tick
	const float radius = SPRITE_RADIUS;
	Cast<USphereComponent>(collider)->InitSphereRadius(radius);
	return collider;
}

bool AAOE::createCollider() {
	//////////
	// Create shape
	if (_AOEConfig->_shape == _CIRCLE || _AOEConfig->_shape == _ARC) {
		_collider = makeDefaultSphereCollider();
	}
	else if (_AOEConfig->_shape == _LINE) {
		_collider = NewObject<UBoxComponent>(this);
		const float width = 2 * SPRITE_RADIUS;
		const float length = SPRITE_RADIUS;
		Cast<UBoxComponent>(_collider)->SetBoxExtent(FVector(length, 1, width));
		_collider->AddWorldOffset(FVector(length, 0, 0), false, nullptr, ETeleportType::TeleportPhysics);
	}
	else {
		LOGERROR("AAOE::initShape - shape not implemented");
		return false;
	}
	return IsValid(_collider);
}

// The dimensions of the shapes (apart from simple geometric ratios) should be controlled via the Actor's scale rather than the collider's settings
void AAOE::initShape() {
	if (!createCollider())
		return;
	_collider->SetupAttachment(RootComponent);
	////////////
	// Set up collision
	bool automaticHitDetection;
	if (_AOEConfig->_effectApplication == _ONCE) {
		automaticHitDetection = true;
	}
	else if (_AOEConfig->_effectApplication == _TICK) {
		_nextTick = _AOEConfig->_tickInterval;
	}
	else {
		LOGERROR("AAOE::initShape - effect application method not implemented");
		return;
	}
	auto type = isFriendly() ? collisionType::friendlyAttack : collisionType::enemyAttack;
	if (!CustomChannels::setupCollision(_collider, type, automaticHitDetection)) {
		LOGERROR("AAOE initShape - could not setup collision");
		return;
	}

	if (automaticHitDetection)
		_collider->OnComponentBeginOverlap.AddDynamic(this, &AAOE::OnOverlapBegin);

	/////////////////
	// Hit everything that is in the area on spawn
	_collider->RegisterComponent();
	//_collider->UpdateOverlaps();
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &AAOE::collideWithAll);
	//collideWithAll();
}

void AAOE::collideWithAll() {
	TSet<AActor*> OverlappingActors;
	//_collider->GetOverlappingActors(OverlappingActors, APawn::StaticClass());
	if (!unrealHelpers::getOverlappingActors(this, _collider->GetCollisionObjectType(), _collider, OverlappingActors, {})) {
		LOGERROR("AAOE::collideWithAll - could not get overlapping actors");
		return;
	}
	for (AActor* Actor : OverlappingActors)
	{
		OnOverlapBegin(_collider, Actor, nullptr, 0, false, FHitResult());
	}
}

void AAOE::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
) {
	if (!IsValid(OtherActor)) {
		LOGERROR("AAOE::OnOverlapBegin - OtherActor is not valid");
		return;
	}
	if (_isAfterimage)
		return;
	if (_AOEConfig->_effectApplication == _ONCE) {
	}
	else if (_AOEConfig->_effectApplication == _TICK) {
		_effectedPawns.Empty();
	}
	else {
		LOGERROR("AAOE::OnOverlapBegin - effect application method not implemented");
		return;
	}
	auto casted = Cast<ACombatant>(OtherActor);
	if (!IsValid(casted)) {
		LOGERROR("AAOE::OnOverlapBegin - OtherActor is not a valid ACombatant");
		return;
	}
	if (shouldApplyEffect(OtherActor)) {
		applyEffect(casted);
	}
}

// For now, slash attacks just have a single instantaneous hitbox, so moving the slash is just cosmetic, unless it's on the very first frame, in which case
// the hitbox isn't active when the slash is moved.
void AAOE::reorientSlash() {
	if (!_pawnRef.IsValid())
		return;
	//if (_FOLLOW_AIM) {
	//	const FVector& newForward = _pawnRef->myGetForwardVector();
	//	const FRotator rotation = UKismetMathLibrary::FindLookAtRotation(FVector(0, 0, 0), FVector(newForward.X, 0, newForward.Z));
	//	SetActorRotation(rotation, ETeleportType::TeleportPhysics);
	//}
	FVector newLocation;
	if (_AOEConfig->_shape != _ARC) {
		newLocation = _pawnRef->GetActorLocation();
	}
	else {
		newLocation = _pawnRef->getSpriteBoundary(GetActorForwardVector());
	}
	SetActorLocation(newLocation, false, nullptr, ETeleportType::TeleportPhysics);
}
///////////////////////////////////////////////////////////////////////////////
// AOEFactory
AOEFactory::AOEFactory(ACombatant* owner, const UAOETemplate* myTemplate) :
	AttackFactory(owner, myTemplate)
{
	if (
		!IsValid(myTemplate->_AOEConfig) || 
		_levels.Num() == 0 || 
		getLevel() < 0 || 
		getLevel() > _levels.Num() - 1 || 
		!IsValid(_levels[getLevel()])
		) 
	{
		LOGERROR("AOEFactory::AOEFactory - invalid parameter");
		return;
	}
	_AOEConfig = myTemplate->_AOEConfig;
	const auto casted = Cast<UAOELevel>(_levels[getLevel()]);
	if (!IsValid(casted)) {
		LOGERROR("AOEFactory::AOEFactory - attempted to initialize with incompatible level");
		return;
	}
	auto temp = std::make_shared<AOEAttributes>(casted->_AOEOffsets, owner->getAttributes());
	_AOEAttributes = std::make_unique<BaseAttributeWrapper<AOEAttributes>>(owner, temp);
}

AOEFactory::AOEFactory(AOEFactory&& other) :
	AttackFactory(std::move(other)),
	_AOEConfig(other._AOEConfig),
	_AOEAttributes(std::move(other._AOEAttributes))
{
	other._AOEAttributes = nullptr;
}

void AOEFactory::finishUpgrade(const UAttackLevel* newLevel) {
	AttackFactory::finishUpgrade(newLevel);
	const auto casted = Cast<UAOELevel>(newLevel);
	if (!IsValid(casted)) {
		LOGERROR("AOEFactory::finishUpgrade - newLevel is not a UAOELevel");
		return;
	}
	_AOEAttributes->changeBaseValues<UAOEAttributeData>(casted->_AOEOffsets.Get());
}

bool AOEFactory::shouldFollowAim() const {
	return AOEEnumSpace::followsAim(_AOEConfig->_shape, _AOEConfig->_targeting);
}

void AOEFactory::tick(float delta) {
	if (!_AOEAttributes) {
		LOGERROR("AOEFactory::tick - AOEAttributes not initialized");
		return;
	}
	_AOEAttributes->tick(delta, getStatusEffects());
	AttackFactory::tick(delta);
}
// Protected nonvirtual function that allows a basic AOE to be created with an alternate init struct. Is called explicitly.
void AOEFactory::launchAttack(const FVector& forward, const AOEInitStruct* init) {
	TArray<AAOE*> newAttacks;
	// If init is specified, call the private nonvirtual implementation
	if (init)
		newAttacks = launchAttack_Impl(forward, init);
	// If init is not specified, call the protected virtual implementation
	else
		newAttacks = launchAttack_Impl(forward);
	for (auto& attack : newAttacks) {
		// Sometimes the factory will spawn something other than an AAttackActor, in which case they return nullptr
		if (!IsValid(attack))
			continue;
		if (shouldFollowAim())
			unrealHelpers::lookAtDirection(attack, forward);
		else
			unrealHelpers::lookAtDirection(attack, FVector(1, 0, 0));
	}
}
// Virtual function from BaseAttackFactory. Mirrors the architecture of launchAttack_impl in that it calls the specialized implementation to avoid 
// code duplication
void AOEFactory::launchAttack(const FVector& forward) {
	launchAttack(forward, nullptr);
}

// Virtual AOEFactory function, called by the virtual BaseAttackFactory function launchAttack(const FVector& forward) overloaded by AOEFactory.
// The separation allows AOEFactory to make changes to the AOE after launch without exposing implementation.
// An additional feature is provided in launchAttack(const FVector& forward, const AOEInitStruct* init), which calls 
// launchAttack_Impl(const FVector& forward, const AOEInitStruct* init), allowing an alternate AOEInitStruct. The default behaviour (not using an
// alternate struct) is achieved here by passing a nullptr.
TArray <AAOE*> AOEFactory::launchAttack_Impl(const FVector& forward) {
	return launchAttack_Impl(forward, nullptr);
}

// Private nonvirtual function, exists primarily to be called by the protected nonvirtual function launchAttack(const FVector&, const AOEInitStruct*),
// but the default AOE launch function (AOEFactory::launchAttack_Impl(const FVector& forward)) also calls it to avoid code duplication.
TArray<AAOE*> AOEFactory::launchAttack_Impl(const FVector& forward, const AOEInitStruct* init) {
	if (!IsValid(_AOEConfig)) {
		LOGERROR("AAOEFactory::launchAttack - Config not valid");
		return {};
	}
	APawn* player = UGameplayStatics::GetPlayerPawn(_owner.Get(), 0);
	AMyPlayer* castedPlayer = Cast<AMyPlayer>(player);
	if (!IsValid(castedPlayer))
		return {};

	AAOE* newAttack = nullptr;
	bool spawnSuccess = false;
	// Does not spawn an AAOE subclass
	if (AOEEnumHelpers::isSpawner(_AOEConfig->_targeting)) {
		if (!_owner.IsValid()) {
			LOGERROR("AAOEFactory::launchAttack - owner not valid");
			return {};
		}
		AAOESpawner* temp = nullptr;
		spawnSuccess = unrealHelpers::spawnActorOnTopOfMeDeferred<AAOESpawner>(_owner.Get(), temp) && IsValid(temp);
		if (spawnSuccess) {
			AOEInitStruct tempStruct = init ? *init : getAOEInit();
			temp->initialise_AAOESpawner(tempStruct);
			spawnSuccess = unrealHelpers::finishDeferredSpawn<AAOESpawner>(_owner.Get(), temp);
		}
		if (!spawnSuccess) {
			LOGERROR("AAOEFactory::launchAttack - failed to spawn AOESpawner");
			return {};
		}
	}
	else if (AOEEnumSpace::instigatorSpawn(_AOEConfig->_targeting) || _AOEConfig->_targeting == _RANDOM) {
		spawnSuccess = unrealHelpers::spawnActorOnTopOfMeDeferred<AAOE>(_owner.Get(), newAttack);
	}
	else if (_AOEConfig->_targeting == _BACKSTAB) {
		ABackstab* temp = nullptr;
		spawnSuccess = unrealHelpers::spawnActorOnTopOfMeDeferred<ABackstab>(_owner.Get(), temp);
		newAttack = temp;
	}
	else {
		LOGERROR("AAOEFactory::launchAttack - unrecognized targeting");
		return {};
	}
	spawnSuccess = spawnSuccess && IsValid(newAttack);
	if (!spawnSuccess){
		LOGERROR("AAOEFactory::launchAttack - failed to spawn AAOE");
		return {};
	}

	AOEInitStruct temp = init ? *init : getAOEInit();
	newAttack->initialise_AAOE(temp);
	if (!unrealHelpers::finishDeferredSpawn<AAOE>(_owner.Get(), newAttack)) {
		LOGERROR("AOEFactory::launchAttack - failed to finish spawning AAOE");
		return {};
	}

	FVector newLocation(0, -1, 0);
	if (AOEEnumSpace::instigatorSpawn(_AOEConfig->_targeting) || _AOEConfig->_targeting == _BACKSTAB) {
		if (_AOEConfig->_shape == _ARC)
			newLocation = _owner->getSpriteBoundary(forward);
	}
	else if (_AOEConfig->_targeting == _RANDOM) {
		newLocation = unrealHelpers::getRandomLocationOnScreen(castedPlayer);
	}
	else {
		LOGERROR("AAOEFactory::launchAttack - unrecognized targeting");
		return {};
	}

	if (helpers::nearEq(newLocation.Y, 100)) {
		newAttack->SetActorLocation(newLocation, false, nullptr, ETeleportType::TeleportPhysics);
	}
	return { newAttack };
}

AOEInitStruct AOEFactory::getAOEInit() const {
	AOEAttributes temp(*(_AOEAttributes->getCore()));
	temp.discretizeFull();
	//Owner is guaranteed to be valid
	FVector forward = _owner->myGetForwardVector();
	AOEInitStruct ret(AttackFactory::getAttackInit(), _AOEConfig.Get(), temp, false, forward);
	return ret;
}
///////////////////////////////////////////////////////////////////////////////
// UAOEAttributeData
//void UAOEAttributeData::replaceOverrides() {
//	if (helpers::isInvalidData(_duration))
//		_duration = _defaults._duration;
//}
void UAOEAttributeData::replaceOverrides() {
	for (const auto& [memberPtr, defaultVal] : DefaultProxy<UAOEAttributeData>::get()) {
		BASEATTRIBUTES_OVERRIDE(memberPtr, defaultVal);
	}
}
///////////////////////////////////////////////////////////////////////////////
// UAOEAttributes
AOEAttributes::AOEAttributes(const AOEAttributes& other) : 
	BaseAttributes(other),  
	//_duration(other._duration), 
	//_arcShape_angle(other._arcShape_angle),
	_attrRef(other._attrRef)
{
	baseInit(other);
}

AOEAttributes::AOEAttributes(AOEAttributes&& other) : 
	BaseAttributes(std::move(other)),
	//_duration(std::move(other._duration)), 
	//_arcShape_angle(std::move(other._arcShape_angle)),
	_attrRef(other._attrRef)
{
	baseInit(other);
	other._attrRef.reset();
}

AOEAttributes::AOEAttributes(const UAOEAttributeData* attr, std::shared_ptr<const CombatantAttributes> attrRef) : 
	BaseAttributes(attr), 
	//_duration(attr->_duration), 
	//_arcShape_angle(attr->_arcShape_angle),
	_attrRef(attrRef)
{
	baseInit(attr);
}

void AOEAttributes::tick(UObject* context, float delta, const TArray<FEffectStruct>& statusEffects) {
	auto temp = _attrRef.lock();
	if (temp.get() == nullptr)
		return;
	softReset();
	modifyAttributes(temp);
	tick_internal(context, delta, statusEffects);
}

void AOEAttributes::modifyAttributes(const std::shared_ptr<const CombatantAttributes>& attr) {
	if (attr.get() == nullptr)
		return;
	_range.modify(_range.getBase() * attr->_projectileSize.getFinal());
}
///////////////////////////////////////////////////////////////////////////////
// UAOEConfig
void UAOEConfig::replaceOverrides() {
	if (unrealHelpers::isInvalidData(_shape))
		_shape = _defaults._shape;
	if (unrealHelpers::isInvalidData(_targeting))
		_targeting = _defaults._targeting;
	if (unrealHelpers::isInvalidData(_effectApplication))
		_effectApplication = _defaults._effectApplication;
	if (helpers::isInvalidData(_tickInterval))
		_tickInterval = _defaults._tickInterval;
}

///////////////////////////////////////////////////////////////////////////////
// UAOETemplate
std::unique_ptr<BaseAttackFactory> UAOETemplate::createFactory(ACombatant* owner) const {
	if (!IsValid(_AOEConfig)) {
		LOGERROR("UAOETemplate::createFactory - invalid AOEConfig");
		return nullptr;
	}
	if (_AOEConfig->_targeting == _TRAIL)
		return std::make_unique<AttackTrailFactory>(owner, this);
	else
		return std::make_unique<AOEFactory>(owner, this);
}




float AAOE::getRemainingDuration() const {
	if (_AOEAttributes.get() == nullptr || !IsValid(_AOEConfig)) {
		LOGERROR("AAOE::getRemainingDuration - invalid parameters");
		return -1;
	}
	float duration;
	if (AOEEnumHelpers::isSpawner(_AOEConfig->_targeting))
		duration = _AOEAttributes->_duration_spawner.getFinal();
	else
		duration = _AOEAttributes->_duration.getFinal();
	return duration - _consumedDuration;
}
float AAOE::getTotalDuration() const { return _AOEAttributes.get() == nullptr ? -1 : _AOEAttributes->_duration.getFinal(); }

void AAOE::updateActorScale() {
	if (!IsValid(_AOEConfig) || _AOEAttributes.get() == nullptr) {
		LOGERROR("AAOE::updateActorScale - invalid parameters");
		return;
	}
	if (_AOEConfig->_shape == _LINE) {
		const float lengthMultiplier = _AOEAttributes->_range.getFinal() * _attackAttributes->_radius.getFinal();
		const float widthMultiplier = _attackAttributes->_radius.getFinal();
		SetActorScale3D(FVector(lengthMultiplier, 1, widthMultiplier));
	}
	else
		Super::updateActorScale();
}

bool AAOE::shouldApplyEffect(AActor* OtherActor) const {
	if (_AOEConfig->_shape == _ARC) {
		const FVector forward = GetActorForwardVector();
		const float angle = unrealHelpers::getAngleBetweenVectors(forward, OtherActor->GetActorLocation() - GetActorLocation());
		if (angle > _AOEAttributes->_arcShape_angle.getFinal() / 2)
			return false;
	}
	ACombatant* combatantActor = Cast<ACombatant>(OtherActor);
	if (!IsValid(combatantActor)) {
		LOGERROR("AAOE::OnOverlapBegin - OtherActor is not a combatant");
		return false;
	}
	return true;
}