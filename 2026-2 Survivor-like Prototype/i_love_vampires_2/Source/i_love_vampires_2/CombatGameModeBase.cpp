#include "CombatGameModeBase.h"
#include "CombatantManager.h"
#include "EnemySpawner.h"
#include "SpriteSorter.h"
#include "Definitions.h"

ACombatGameModeBase::ACombatGameModeBase() : AGameModeBase() {
	_enemySpawner = CreateDefaultSubobject<UEnemySpawner>(TEXT("EnemySpawner"));
	_combatantManager = CreateDefaultSubobject<UCombatantManager>(TEXT("CombatantManager"));
	_spriteSorter = CreateDefaultSubobject<USpriteSorter>(TEXT("SpriteSorter"));
}

void ACombatGameModeBase::BeginPlay() {
	Super::BeginPlay();
	if (!IsValid(_enemySpawner)) {
		LOGERROR("ACombatGameModeBase::BeginPlay - _enemySpawner is not valid");
		return;
	}
	if (!IsValid(_combatantManager)) {
		LOGERROR("ACombatGameModeBase::BeginPlay - _combatantManager is not valid");
		return;
	}
}

void ACombatGameModeBase::initialise_ACombatGameModeBase(UMapTemplate* map) {
	if (!IsValid(_enemySpawner)) {
		LOGERROR("ACombatGameModeBase::initialise_ACombatGameModeBase - _enemySpawner is not valid");
		return;
	}
	_enemySpawner->initialise_UEnemySpawner(map);
}

void ACombatGameModeBase::Tick(float delta) {
	if (!isReady())
		return;
	Super::Tick(delta);
	_enemySpawner->tick(delta);
	_combatantManager->tick(delta);
}

void ACombatGameModeBase::setGameReady(bool val) {
	_combatantManager->setGameReady(val);
	_enemySpawner->setGameReady(val);
}

bool ACombatGameModeBase::isReady_Implementation() const {
	if (!IsValid(_enemySpawner) || !IsValid(_combatantManager) || !IsValid(_spriteSorter)) {
		return false;
	}
	return _enemySpawner->getGameReady() && _combatantManager->getGameReady();
}
