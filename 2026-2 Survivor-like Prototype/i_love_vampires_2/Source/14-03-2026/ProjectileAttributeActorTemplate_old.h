#pragma once
#include "CoreMinimal.h"
#include "BaseAttributeActorTemplate.h"
#include "ProjectileAttributeActorTemplate.generated.h"
class UProjectileConfig;
class UProjectileAttributeTemplate;

UCLASS(BlueprintType)
class I_LOVE_VAMPIRES_2_API UProjectileAttributeActorTemplate : public UBaseAttributeActorTemplate {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Instanced, Category = "ProjectileAttributeActorTemplate")
	UProjectileConfig* _config;
	UPROPERTY(EditAnywhere, Instanced, Category = "ProjectileAttributeActorTemplate")
	UProjectileAttributeTemplate* _attributes;
};

