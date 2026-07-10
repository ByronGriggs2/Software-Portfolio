#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RNG.generated.h"

UCLASS()
class I_LOVE_VAMPIRES_2_API URNG : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	URNG();
	UFUNCTION(BlueprintCallable)
	float randFloat();
	UFUNCTION(BlueprintCallable)
	float randFloatRange(float min, float max);
	UFUNCTION(BlueprintCallable)
	int randInt();
	UFUNCTION(BlueprintCallable)
	int randIntRange(int min, int max);

	static URNG* getSubsystem(UWorld* world) {
		return world->GetGameInstance()->GetSubsystem<URNG>();
	}
	static float randFloat(UWorld* world) {
		return getSubsystem(world)->randFloat();
	}
	static float randFloatRange(UWorld* world, float min, float max) {
		return getSubsystem(world)->randFloatRange(min, max);
	}
	static int randInt(UWorld* world) {
		return getSubsystem(world)->randInt();
	}
	static int randIntRange(UWorld* world, int min, int max) {
		return getSubsystem(world)->randIntRange(min, max);
	}
};