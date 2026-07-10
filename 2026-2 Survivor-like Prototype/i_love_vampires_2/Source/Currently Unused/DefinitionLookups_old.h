#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
//#include "AttributeSet.h"
#include "DefinitionLookups.generated.h"
struct FGameplayAttribute;

UCLASS()
class I_LOVE_VAMPIRES_2_API UDefinitionLookups : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	UDefinitionLookups();

public:
	UFUNCTION(BlueprintPure)
	FGameplayAttribute getCombatantAttributeByName(const FName& name) const;
	UFUNCTION(BlueprintPure)
	FName getNameByCombatantAttribute(const FGameplayAttribute& attribute) const;
	UFUNCTION(BlueprintPure)
	TArray<FName> combatantAttributeKeys() const;
	UFUNCTION(BlueprintPure)
	TArray<FGameplayAttribute> combatantAttributeValues() const;

	static TArray<float> getFloatPropertiesAsArray(const UObject* object, bool&);
	static bool getDefinitionLookups(UDefinitionLookups*& lookups);
};