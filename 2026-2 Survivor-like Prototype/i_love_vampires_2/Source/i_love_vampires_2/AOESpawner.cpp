#include "AOESpawner.h"
#include "Definitions.h"
#include "helpers.h"
#include "AOESpawner_line.h"
#include "unrealHelpers.h"

std::unique_ptr<AOESpawner> AAOESpawner::createPolySpawner(const AOEInitStruct& spawnTemplate) {
	if (!IsValid(spawnTemplate._AOEConfig)) {
		LOGERROR("createPolySpawner - invalid AOEConfig in spawn template");
		return nullptr;
	}
	EAOETargeting target = spawnTemplate._AOEConfig->_targeting;
	switch (target) {
	case _LINE :
		return std::make_unique<AOESpawner_line>(this, spawnTemplate);
		break;
	default:
		LOGERROR("createPolySpawner - unrecognized spawner type");
		return nullptr;
		break;
	}
}

AAOESpawner::AAOESpawner(){ 
	PrimaryActorTick.bCanEverTick = true; 
	PrimaryActorTick.bStartWithTickEnabled = true; 
	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	}
}

void AAOESpawner::initialise_AAOESpawner(const AOEInitStruct& spawnTemplate) {
	_temp = std::make_unique<AOEInitStruct>(spawnTemplate);
}

void AAOESpawner::PostInitializeComponents() {
	Super::PostInitializeComponents();
	if (_temp == nullptr) {
		LOGERROR("AAOESpawner::BeginPlay - uninitialized spawner");
		Destroy();
		return;
	}
	_spawner = createPolySpawner(*_temp);
	_temp.reset();
	if (_spawner.get() == nullptr) {
		LOGERROR("AAOESpawner::BeginPlay - uninitialized spawner");
		Destroy();
		return;
	}
	if (_spawner->isFinished()) {
		LOGWARNING("AAOESpawner::BeginPlay - spawner is already finished on begin play");
		Destroy();
		return;
	}
}

void AAOESpawner::Tick(float delta) {
	Super::Tick(delta);
	if (_spawner.get() == nullptr) {
		LOGERROR("AAOESpawner::Tick - uninitialized spawner");
		Destroy();
		return;
	}
	if (_spawner->isFinished()) {
		Destroy();
		return;
	}
	_spawner->tick(delta);
}

AOESpawner::AOESpawner(AActor* owner, const AOEInitStruct& spawnTemplate) : 
	_spawnTemplate(spawnTemplate), 
	_totalAttacks(0),
	_remainingAttacks(0),
	_attackCooldown(0),
	_nextAttackTime(0),
	_killMe(false),
	_owner(owner)
{
	Stat temp(_spawnTemplate._AOEAttributes._count_spawner);
	temp.discretize();
	_totalAttacks = temp.getFinal();
	_remainingAttacks = _totalAttacks;
	if (_totalAttacks <= 0) {
		LOGWARNING("AOESpawner::AOESpawner - total attacks calculated to be 0 or less");
		_killMe = true;
	}
	_attackCooldown = _spawnTemplate._AOEAttributes._duration_spawner.getFinal() / (_totalAttacks - 1);
	//initLocation();
}

void AOESpawner::tick(float delta) {
	if (_remainingAttacks <= 0) {
		_killMe = true;
		return;
	}
	if (_nextAttackTime <= EPSILON) {
		launchAttack();
		_nextAttackTime = _attackCooldown;
	}
	_nextAttackTime -= delta;
}

void AOESpawner::launchAttack() {
	if (!_owner.IsValid())
		return;
	AAOE* newAttack = nullptr;
	if (!unrealHelpers::spawnActorOnTopOfMeDeferred<AAOE>(_owner.Get(), newAttack)) {
		LOGERROR("AOESpawner::launchAttack - failed to spawn aoe");
		return;
	}
	newAttack->initialise_AAOE(_spawnTemplate);
	newAttack->SetActorLocation(getAttackLocation(), false, nullptr, ETeleportType::TeleportPhysics);
	if (!unrealHelpers::finishDeferredSpawn<AAOE>(_owner.Get(), newAttack)) {
		LOGERROR("AOESpawner::launchAttack - failed to finish spawning aoe");
		return;
	}
	_remainingAttacks--;
}

//void AOESpawner::initLocation() {
//	if (_variant == _LINE) {
//		initLocationLine();
//	}
//	else {
//		LOGERROR("AOESpawner::initLocation - unrecognized targeting type");
//	}
//}
//
//FVector AOESpawner::getAttackLocation() {
//	if (_variant == _LINE) {
//		return getAttackLocationLine();
//	}
//	else {
//		LOGERROR("AOESpawner::getAttackLocation - unrecognized targeting type");
//		return GetActorLocation();
//	}
//}
//
//void AOESpawner::initLocationLine() {
//	FVector location;
//	unrealHelpers::makeRandomLineSegment(_spawnTemplate._AOEAttributes._range.getFinal(), location, _direction);
//	SetActorLocation(location, false, nullptr, ETeleportType::TeleportPhysics);
//}
//
//FVector AOESpawner::getAttackLocationLine() {
//	const float totalDistance = _spawnTemplate._AOEAttributes._range.getFinal();
//	return GetActorLocation() + _direction * totalDistance * ((_totalAttacks - _remainingAttacks) / static_cast<float>(_totalAttacks - 1));
//}