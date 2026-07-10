#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Templates/SubclassOf.h"
#include "InputEnum.h"
#include "AssetRefs.generated.h"
class UInputMappingContext;
class UCombatantTemplate;
class UInputAction;
class UPaperFlipbook;
class UMaterialInterface;
class UUserWidget;
class UDamageNumber;
class UDataTable;
class UTexture2D;

UCLASS()
class I_LOVE_VAMPIRES_2_API UAssetRefs : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// Sprites
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UPaperFlipbook* _testOverlay = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UPaperFlipbook* _experienceSprite = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UPaperFlipbook* _bleedSprite = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UPaperFlipbook* _genericExplosion = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UTexture2D* _defaultIcon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) UInputAction* _moveAction = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UInputAction* _cancelAction = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) UInputMappingContext* _actionIMC = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UInputMappingContext* _menuIMC = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UCombatantTemplate* _basePlayerTemplate = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UMaterialInterface* _translucentMaterial = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TSubclassOf<UDamageNumber> _damagePopupWidgetClass = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UCombatantTemplate* _testEnemy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UMaterialInterface* _iconGrayscaleMaterial = nullptr;

	UAssetRefs();

	UPaperFlipbook* getExperienceSprite() const { return _experienceSprite; }
	UPaperFlipbook* getTestOverlay() const { return _testOverlay; }
	UPaperFlipbook* getBleedSprite() const { return _bleedSprite; }
	UPaperFlipbook* getGenericExplosion() const { return _genericExplosion; }

	UTexture2D* getDefaultIcon() const;

	UInputMappingContext* getActionContext() const;
	UInputMappingContext* getMenuContext() const;
	UCombatantTemplate* getBasePlayerTemplate() const;
	UInputAction* getMoveAction() const;
	UInputAction* getCancelAction() const;
	
	//cannot be const
	UMaterialInterface* getSpriteMaterial() const;
	TSubclassOf<UDamageNumber> getDamagePopupWidgetClass() const;
	UMaterialInterface* getIconGrayscaleMaterial() const;
	UCombatantTemplate* getTestEnemy() const;
};