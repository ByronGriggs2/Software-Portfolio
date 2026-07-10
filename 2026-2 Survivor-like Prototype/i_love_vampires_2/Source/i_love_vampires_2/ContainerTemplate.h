#pragma once
#include "BaseTemplate.h"
#include "BaseConfig.h"
#include "ActiveEnum.h"
#include "ContainerTemplate.generated.h"

UCLASS(BlueprintType, EditInlineNew)
class I_LOVE_VAMPIRES_2_API USlotConfig : public UBaseConfig {
	GENERATED_BODY()

public:
	virtual void replaceOverrides() override {}
	virtual void dynamicDeepCopy(UObject* context) override {}
};

UCLASS(BlueprintType)
class I_LOVE_VAMPIRES_2_API UContainerTemplate : public UBaseTemplate
{
	GENERATED_BODY()

	struct defaults {
		EWeaponType _type = static_cast<EWeaponType>(uint8(0));
	};
	const static inline defaults _defaults;
	bool slotsAreValid() const;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FString _name = "_invalid_";
	UPROPERTY(EditAnywhere, BlueprintReadOnly) EWeaponType _type = static_cast<EWeaponType>(uint8(255));
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly) TObjectPtr<UBaseConfig> _slot0;
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly) TObjectPtr<UBaseConfig> _slot1;
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly) TObjectPtr<UBaseConfig> _slot2;
	
	UContainerTemplate(const FObjectInitializer& init) : Super(init) {
		_slot0 = init.CreateDefaultSubobject<USlotConfig>(this, "_slot0");
		_slot1 = init.CreateDefaultSubobject<USlotConfig>(this, "_slot1");
		_slot2 = init.CreateDefaultSubobject<USlotConfig>(this, "_slot2");
	}
	virtual void replaceOverrides() override;
	virtual void dynamicDeepCopy(UObject* context) override;
};