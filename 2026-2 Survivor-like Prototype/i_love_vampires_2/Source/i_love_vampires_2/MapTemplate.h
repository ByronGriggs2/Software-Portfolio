#pragma once
#include "CoreMinimal.h"
#include "BaseTemplate.h"
#include "MapTemplate.generated.h"

class UCombatantTemplate;
class UPaperFlipbook;

// These are archetypes rather than individual enemies, though in the final game there may only be one enemy per archetype.
// lighter is weaker
// triangle is swarmer
// circle is standard
// square is bulky
UENUM(BlueprintType)
enum class EEnemyType : uint8 {
	// 16x16 light green circle
	weakling UMETA(DisplayName = "Weakling"), // 1 HP
	// full size dark green circle
	grunt UMETA(DisplayName = "Grunt"), // generic melee
	// darker green chevron
	veteran UMETA(DisplayName = "Veteran"), // direct upgrade of grunt, visually distinct to show upgrade
	// blood red square
	minotaur UMETA(DisplayName = "Minotaur"), // charges
	// dark blue circle
	ranger UMETA(DisplayName = "Ranger"), // stops moving and shoots a single projectile
	// light blue triangle
	ranger_swarming UMETA(DisplayName = "Ranger Swarming"), // much squishier, shows up in groups, shoots a faster projectile with less damage and windup
	// white star
	suicide UMETA(DisplayName = "Suicide"), // runs at player and explodes on death. Friendly fire?
	// dark gray square
	knight UMETA(DisplayName = "Knight"), // High damage, high health, long range?
	// dark turqoise square
	golem UMETA(DisplayName = "Golem"), // Like a knight but much higher health and in fewer number. Perhaps lower range.
	// lighter red pentagon
	fencer UMETA(DisplayName = "Fencer"), // An elite enemy with some sort of AOE melee attack
	// purple hexagon
	barbarian UMETA(DisplayName = "Barbarian"), // A fast enemy with moderate hp and high damage
	// yellow triangle
	swarmer UMETA(DisplayName = "Swarmer"), // small, fast, spawns in large groups
	// gold star
	boss1 UMETA(DisplayName = "Boss 1"),
	// gold star
	boss2 UMETA(DisplayName = "Boss 2"),
	// gold star
	boss3 UMETA(DisplayName = "Boss 3")
};

UCLASS(BlueprintType)
class I_LOVE_VAMPIRES_2_API UMapTemplate : public UBaseTemplate {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly) TMap<EEnemyType, TObjectPtr<UCombatantTemplate>> _enemyTemplates;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) TObjectPtr<UCombatantTemplate> _boss1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) TObjectPtr<UCombatantTemplate> _boss2;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) TObjectPtr<UCombatantTemplate> _boss3;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) TObjectPtr<UPaperFlipbook> _background;

	virtual void replaceOverrides() override;
	virtual void dynamicDeepCopy(UObject*) override;
	UCombatantTemplate* getEnemyTemplate(EEnemyType type) const {
		if (type == EEnemyType::boss1) {
			return _boss1;
		}
		if (type == EEnemyType::boss2) {
			return _boss2;
		}
		if (type == EEnemyType::boss3) {
			return _boss3;
		}
		return _enemyTemplates.Contains(type) ? _enemyTemplates[type] : nullptr;
	}
};