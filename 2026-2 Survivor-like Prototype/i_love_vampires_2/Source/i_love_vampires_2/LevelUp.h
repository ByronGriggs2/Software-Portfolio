#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include <vector>
#include "LevelUp.generated.h"
class AMyPlayer;
class UWeaponTemplate;
class UWeaponContainer;
struct FSocketData;

UENUM(BlueprintType)
enum class CardType : uint8 {
	swordOnly UMETA(DisplayName = "Sword Only"),
	gunOnly UMETA(DisplayName = "Gun Only"),
	both UMETA(DisplayName = "Both"),
	INVALID UMETA(DisplayName = "INVALID")
};

UCLASS(BlueprintType)
class I_LOVE_VAMPIRES_2_API ULevelUp_base : public UObject {
	GENERATED_BODY()

protected:
	TWeakObjectPtr<AMyPlayer> _player;

public:
	bool playerRegistered() const { return _player.IsValid(); }
	UFUNCTION(BlueprintCallable) virtual bool registerPlayer(AMyPlayer* player);
};

UCLASS(BlueprintType)
class I_LOVE_VAMPIRES_2_API ULevelUp_weaponSelect : public ULevelUp_base {
	GENERATED_BODY()

	bool _validSocketData = false;
	TArray<FSocketData> _left;
	TArray<FSocketData> _right;
	TObjectPtr<UWeaponTemplate> _selectedWeapon;
	int _slotIndex = -1;

	const UWeaponContainer* getContainer(int index) const;
	bool calculateSocketData(TArray<FSocketData>& leftContainer, TArray<FSocketData>& rightContainer) const;
	std::vector<int> getEmptySlots() const;
	bool containersAreValid() const;

public:
	UFUNCTION(BlueprintCallable) void upgradeWeapon() const;
	UFUNCTION(BlueprintCallable) void giveWeapon() const;
	UFUNCTION(BlueprintCallable) bool swapToNewWeapon(UWeaponTemplate* weapon);
	UFUNCTION(BlueprintCallable, meta = (ExpandBoolAsExecs = "ReturnValue")) bool isReady();
	UFUNCTION(BlueprintCallable, meta = (ExpandBoolAsExecs = "ReturnValue")) bool initialise_ULevelUp_weaponSelect(UWeaponTemplate* weapon, AMyPlayer* player);
	UFUNCTION(BlueprintCallable, meta = (ExpandBoolAsExecs = "ReturnValue")) bool cursorLeft(int& newIndex);
	UFUNCTION(BlueprintCallable, meta = (ExpandBoolAsExecs = "ReturnValue")) bool cursorRight(int& newIndex);
	UFUNCTION(BlueprintPure) void getSocketData(TArray<FSocketData>& leftContainer, TArray<FSocketData>& rightContainer) const;
	UFUNCTION(BlueprintPure) int getHoveredIndex() const { return _slotIndex; }
	UFUNCTION(BlueprintPure) CardType getCardType() const;
};

UCLASS(BlueprintType)
class I_LOVE_VAMPIRES_2_API ULevelUp_evolutionHandler : public ULevelUp_base {
	GENERATED_BODY()

	TArray<UWeaponTemplate*> _evolutionOptions;
	bool _evolutionSelected = false;

public:
	virtual bool registerPlayer(AMyPlayer* player) override;
	UFUNCTION(BlueprintCallable) TArray<UWeaponTemplate*> getModdedEvolutionOptions() const;
	UFUNCTION(BlueprintCallable) void selectEvolution(UWeaponTemplate* option);
	UFUNCTION(BlueprintCallable, meta = (ExpandBoolAsExecs = "ReturnValue")) bool initialise_ULevelUp_evolutionHandler(AMyPlayer* player) { return registerPlayer(player); }
};