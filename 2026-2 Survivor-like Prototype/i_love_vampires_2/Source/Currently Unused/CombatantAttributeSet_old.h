// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Definitions.h"
#include "CombatantAttributeSet.generated.h"
struct FCombatantData;

UCLASS(BlueprintType)
class I_LOVE_VAMPIRES_2_API UCombatantAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public :
	UCombatantAttributeSet();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UCombatantAttributeSet")
	FGameplayAttributeData maxHP;
	ATTRIBUTE_ACCESSORS(UCombatantAttributeSet, maxHP);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UCombatantAttributeSet")
	FGameplayAttributeData damageReduction_flat;
	ATTRIBUTE_ACCESSORS(UCombatantAttributeSet, damageReduction_flat);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UCombatantAttributeSet")
	FGameplayAttributeData damageReduction_percent;
	ATTRIBUTE_ACCESSORS(UCombatantAttributeSet, damageReduction_percent);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UCombatantAttributeSet")
	FGameplayAttributeData healthRegen_flat;
	ATTRIBUTE_ACCESSORS(UCombatantAttributeSet, healthRegen_flat);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UCombatantAttributeSet")
	FGameplayAttributeData healthRegen_percent;
	ATTRIBUTE_ACCESSORS(UCombatantAttributeSet, healthRegen_percent);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UCombatantAttributeSet")
	FGameplayAttributeData critChance;
	ATTRIBUTE_ACCESSORS(UCombatantAttributeSet, critChance);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UCombatantAttributeSet")
	FGameplayAttributeData critMultiplier;
	ATTRIBUTE_ACCESSORS(UCombatantAttributeSet, critMultiplier);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UCombatantAttributeSet")
	FGameplayAttributeData attackSpeed;
	ATTRIBUTE_ACCESSORS(UCombatantAttributeSet, attackSpeed);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UCombatantAttributeSet")
	FGameplayAttributeData bonusBounces;
	ATTRIBUTE_ACCESSORS(UCombatantAttributeSet, bonusBounces);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UCombatantAttributeSet")
	FGameplayAttributeData bonusPierce;
	ATTRIBUTE_ACCESSORS(UCombatantAttributeSet, bonusPierce);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UCombatantAttributeSet")
	FGameplayAttributeData bonusProjectiles;
	ATTRIBUTE_ACCESSORS(UCombatantAttributeSet, bonusProjectiles);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UCombatantAttributeSet")
	FGameplayAttributeData projectileSpeed;
	ATTRIBUTE_ACCESSORS(UCombatantAttributeSet, projectileSpeed);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UCombatantAttributeSet")
	FGameplayAttributeData projectileSize;
	ATTRIBUTE_ACCESSORS(UCombatantAttributeSet, projectileSize);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UCombatantAttributeSet")
	FGameplayAttributeData AOESize;
	ATTRIBUTE_ACCESSORS(UCombatantAttributeSet, AOESize);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UCombatantAttributeSet")
	FGameplayAttributeData movementSpeed;
	ATTRIBUTE_ACCESSORS(UCombatantAttributeSet, movementSpeed);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UCombatantAttributeSet")
	FGameplayAttributeData range;
	ATTRIBUTE_ACCESSORS(UCombatantAttributeSet, range);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UCombatantAttributeSet")
	FGameplayAttributeData contactDamage;
	ATTRIBUTE_ACCESSORS(UCombatantAttributeSet, contactDamage);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UCombatantAttributeSet")
	FGameplayAttributeData selfSize;
	ATTRIBUTE_ACCESSORS(UCombatantAttributeSet, selfSize);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UCombatantAttributeSet")
	FGameplayAttributeData iFrameDuration;
	ATTRIBUTE_ACCESSORS(UCombatantAttributeSet, iFrameDuration);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UCombatantAttributeSet")
	FGameplayAttributeData currentHP;
	ATTRIBUTE_ACCESSORS(UCombatantAttributeSet, currentHP);
};
