#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <memory>
#include "WeaponContainer.h"
#include "WeaponContainerUnreal.generated.h"
class UGun;
class USword;
class UContainerTemplate;

UCLASS(BlueprintType)
class I_LOVE_VAMPIRES_2_API UWeaponContainer : public UActorComponent
{
	GENERATED_BODY()

protected:
	std::unique_ptr<WeaponContainer> _container;

public:
	void initialise_UWeaponContainer(const UContainerTemplate*) {}
	virtual void BeginPlay() override;
	WeaponContainer* getContainer() const { return _container.get(); }
	bool isFull();
};

UCLASS(BlueprintType)
class I_LOVE_VAMPIRES_2_API USword : public UWeaponContainer{
	GENERATED_BODY()

public:
	void initialise_USword();
};

UCLASS(BlueprintType)
class I_LOVE_VAMPIRES_2_API UGun : public UWeaponContainer{
	GENERATED_BODY()

public:
	void initialise_UGun();
};