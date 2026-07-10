#include "EnemySpawner.h"
#include "Math/UnrealMathUtility.h"
#include "Definitions.h"
#include "Combatant.h"
#include <cmath>
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"
#include "Engine/AssetManager.h"
#include "BaseAttributeSet.h"
#include "AssetRefs.h"
#include "MyGameplayStatics.h"

UEnemySpawner::UEnemySpawner() {
}

void UEnemySpawner::initialise_UEnemySpawner(UMapTemplate* map) {
	_spawnLogic = std::make_unique<EnemySpawnLogic>(map);
}

bool UEnemySpawner::spawnTestEnemy(ACombatant*& ret) {
	UAssetRefs* refs = nullptr;
	if (!MyGameplayStatics::getAssetRefs(refs)) {
		LOGERROR("UEnemySpawner::spawnTestEnemy - failed to get asset refs");
		return false;
	}
	UCombatantTemplate* testEnemy = refs->getTestEnemy();
	return spawnEnemy(getRandomSpawnLocation(), testEnemy, ret);
}

FVector UEnemySpawner::getRandomSpawnLocation() const {
	const float spawnDistance = 100.0f;
	APawn* player = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!IsValid(player)) {
		// This can be called while player is dead (because a function should be able to know about getRandomSpawnLocation without knowing about player)
		//LOGERROR("player is invalid");
		return FVector::ZeroVector;
	}
	FVector playerPos = player->K2_GetActorLocation();
	double angle = FMath::FRandRange(0, 2 * PI);
	double sin = std::sin(angle);
	double cos = std::cos(angle);
	return FVector(cos * spawnDistance + playerPos.X, playerPos.Y, sin * spawnDistance + playerPos.Z);
}

// Combatant creates the dynamic version itself
bool UEnemySpawner::spawnEnemy(const FVector& spawnLocation, UCombatantTemplate* rawData, ACombatant*& ret) {
	if (!IsValid(rawData) || !IsValid(rawData->_config)) {
		LOGERROR("UEnemySpawner::spawnEnemy - invalid rawData");
		return false;
	}

	const TSubclassOf<ACombatant> enemyClass = rawData->_config->_combatantClass;
	const FRotator spawnRotation(0, 0, 0);
	const FVector spawnScale(1, 1, 1);
	const FTransform spawnTransform(spawnRotation, spawnLocation, spawnScale);
	const ESpawnActorScaleMethod scaleMethod = ESpawnActorScaleMethod::MultiplyWithRoot;

	UWorld* world = GetWorld();
	if (!IsValid(world)) {
		LOGERROR("UEnemySpawner::spawnEnemy - world is invalid");
		return false;
	}
	ret = world->SpawnActorDeferred<ACombatant>(
		enemyClass,
		spawnTransform,
		nullptr,
		nullptr,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn,
		scaleMethod
	);
	if (!IsValid(ret)) {
		LOGERROR("UEnemySpawner::spawnEnemy - failed to spawn enemy");
		return false;
	}
	ret->myInitialise(rawData);
	UGameplayStatics::FinishSpawningActor(ret, spawnTransform, scaleMethod);
	if (!IsValid(ret)) {
		LOGERROR("UEnemySpawner::spawnEnemy - failed to finish spawning enemy");
		return false;
	}
	return true;
}

void UEnemySpawner::tick(float DeltaTime) {
	if (!_gameReady || _spawnLogic == nullptr)
		return;
	const float time = GetWorld()->TimeSeconds;
	
	TArray<UCombatantTemplate*> enemiesToSpawn = _spawnLogic->getEnemiesForTick(time);
	for (auto& enemyTemplate : enemiesToSpawn) {
		if (!IsValid(enemyTemplate)) {
			LOGWARNING("UEnemySpawner::tick - Enemy Template invalid. Most likely it is not set in the map.");
			continue;
		}
		ACombatant* spawnedEnemy = nullptr;
		const auto newLocation = getRandomSpawnLocation();
		if (newLocation == FVector::ZeroVector)
			return;
		if (!spawnEnemy(newLocation, enemyTemplate, spawnedEnemy)) {
			LOGERROR("UEnemySpawner::tick - failed to spawn enemy");
		}
	}
}