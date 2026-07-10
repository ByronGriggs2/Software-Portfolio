#pragma once
#include "CoreMinimal.h"
#include "BaseAttributeActorTemplate.h"
#include "ExplosiveProjectileAttributeActorTemplate.generated.h"
class UExplosiveProjectileConfig;
class UExplosiveProjectileAttributeTemplate;

UCLASS(BlueprintType)
class I_LOVE_VAMPIRES_2_API UExplosiveProjectileAttributeActorTemplate : public UBaseAttributeActorTemplate {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Instanced, Category = "ExplosiveProjectileAttributeActorTemplate")
	UExplosiveProjectileConfig* _config;
	UPROPERTY(EditAnywhere, Instanced, Category = "ExplosiveProjectileAttributeActorTemplate")
	UExplosiveProjectileAttributeTemplate* _attributes;
};

