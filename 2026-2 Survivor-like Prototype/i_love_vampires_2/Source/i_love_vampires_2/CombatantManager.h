#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CombatantManager.generated.h"
class ACombatant;
class AEnemyBase;

UCLASS(BlueprintType)
class I_LOVE_VAMPIRES_2_API UCombatantManager : public UObject
{
	GENERATED_BODY()

	const static inline float _POISON_PERIOD = 1.0f;

	int _nextKey = 0;
	bool _burnThisFrame = false;
	bool _ignoreNextBurn = false;
	bool _poisonThisFrame = false;
	float _poisonTimer = 0.0f;
	bool _gameReady = false;
	UPROPERTY()
	TMap<int, TWeakObjectPtr<AEnemyBase>> _enemyReferences;
	UPROPERTY()
	TWeakObjectPtr<ACombatant> _playerRef = nullptr;
public:
	UCombatantManager() = default;
	virtual void tick(float DeltaTime);

	AEnemyBase* getRandomEnemyPtr(const AEnemyBase* excluded) const;
	AEnemyBase* getNearestEnemyPtr(const AActor* caller) const;
	AEnemyBase* getHighestHPEnemy() const;
	bool getPoisonThisFrame() const { return _poisonThisFrame; }
	UFUNCTION(BlueprintPure) int getEnemyCount() const { return _enemyReferences.Num(); }
	bool getBurnThisFrame() const { return _burnThisFrame; }

	void setPlayerRef(ACombatant* playerRef);
	void setGameReady(bool val) { _gameReady = val; }
	bool getGameReady() const { return _gameReady; }
	int registerEnemy(AEnemyBase* enemy);
	void removeFromRegister(int key);
};