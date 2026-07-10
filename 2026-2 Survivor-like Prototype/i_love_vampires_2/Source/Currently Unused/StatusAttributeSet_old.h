// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "Definitions.h"
#include "AbilitySystemComponent.h"
#include "StatusAttributeSet.generated.h"

UCLASS()
class I_LOVE_VAMPIRES_2_API UStatusAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public :
	UStatusAttributeSet();

	// Bleed is continuous. Magnitude is dps.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UStatusAttributeSet")
	FGameplayAttributeData bleed;
	ATTRIBUTE_ACCESSORS(UStatusAttributeSet, bleed);
	// Burn is once per second. Magnitude is %max hp per tick.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UStatusAttributeSet")
	FGameplayAttributeData burn;
	ATTRIBUTE_ACCESSORS(UStatusAttributeSet, burn);
};
