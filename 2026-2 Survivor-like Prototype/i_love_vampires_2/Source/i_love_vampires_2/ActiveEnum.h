#pragma once

UENUM(BlueprintType)
enum class EActivationType : uint8
{
	single UMETA(DisplayName = "single"),
	burst UMETA(DisplayName = "burst"),
	INVALID = 255 UMETA(DisplayName = "INVALID")
};
// Dependencies
// single, burst - Active.h
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	sword UMETA(DisplayName = "sword"),
	gun UMETA(DisplayName = "gun"),
	INVALID = 255 UMETA(DisplayName = "INVALID")
};
// Dependencies
// all - AMyPlayer, UMyPlayerTemplate, AutomaticAssetRefs
// sword - Sword
// gun - Gun

// Note: this is not included in config
UENUM()
enum class EActivationTrigger : uint8
{
	periodic UMETA(DisplayName = "periodic"),
	autocharge UMETA(DisplayName = "autocharge"),
	manual UMETA(DisplayName = "manual"),
	INVALID = 255 UMETA(DisplayName = "INVALID")
};
// Dependencies
// autocharge - AChasingSummon
// all - Active