#pragma once
#include "CoreMinimal.h"
#include "BaseAttributeActorTemplate.h"
#include "AOEAttributeActorTemplate.generated.h"
class UAOEConfig;
class UAOEAttributeTemplate;

UCLASS(BlueprintType)
class I_LOVE_VAMPIRES_2_API UAOEAttributeActorTemplate : public UBaseAttributeActorTemplate {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Instanced, Category = "AOEAttributeActorTemplate")
	UAOEConfig* _config;
	UPROPERTY(EditAnywhere, Instanced, Category = "AOEAttributeActorTemplate")
	UAOEAttributeTemplate* _attributes;
};

