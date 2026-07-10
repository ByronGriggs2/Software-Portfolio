 #pragma once
#include "GameFramework/GameModeBase.h"
#include "CombatGameModeBase.generated.h"
class UEnemySpawner;
class UCombatantManager;
class USpriteSorter;

UCLASS(Blueprintable)
class I_LOVE_VAMPIRES_2_API ACombatGameModeBase : public AGameModeBase {
	GENERATED_BODY()

	UPROPERTY()
	UEnemySpawner* _enemySpawner = nullptr;
	UPROPERTY()
	UCombatantManager* _combatantManager = nullptr;
	UPROPERTY()
	USpriteSorter* _spriteSorter = nullptr;

	bool isReady_Implementation() const;
	void onPlayerDeath_Implementation() {}

protected:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool isReady() const;

public:
	ACombatGameModeBase();
	virtual void BeginPlay() override;
	virtual void Tick(float delta) override;
	UFUNCTION(BlueprintCallable) void initialise_ACombatGameModeBase(UMapTemplate* map);
	UFUNCTION(BlueprintCallable) UEnemySpawner* getEnemySpawner() const { return _enemySpawner; }
	UFUNCTION(BlueprintCallable) UCombatantManager* getCombatantManager() const { return _combatantManager; }
	UFUNCTION(BlueprintCallable) USpriteSorter* getSpriteSorter() const { return _spriteSorter; }
	UFUNCTION(BlueprintCallable) void setGameReady(bool val);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable) void onPlayerDeath();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable) void onPlayerCancel();
};