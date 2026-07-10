#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include <memory>
#include "EnemySpawnLogic.h"
#include "EnemySpawner.generated.h"
class ACombatant;
class UCombatantTemplate;
class EnemySpawnLogic;
class UMapTemplate;

UCLASS(BlueprintType)
class I_LOVE_VAMPIRES_2_API UEnemySpawner : public UObject
{
	GENERATED_BODY()

	std::unique_ptr<EnemySpawnLogic> _spawnLogic;

	bool _gameReady = false;
	bool spawnTestEnemy(ACombatant*& ret);
	bool spawnEnemy(const FVector& location, UCombatantTemplate* data, ACombatant*& ret);
	FVector getRandomSpawnLocation() const;

public:
	UEnemySpawner();
	void initialise_UEnemySpawner(UMapTemplate* map);
	void tick(float DeltaTime);
	void setGameReady(bool val) { _gameReady = val; }
	bool getGameReady() const { return _gameReady; }
};