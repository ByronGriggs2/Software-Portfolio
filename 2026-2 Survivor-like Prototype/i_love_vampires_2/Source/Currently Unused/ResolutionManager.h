#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ResolutionManager.generated.h"

UCLASS()
class I_LOVE_VAMPIRES_2_API UResolutionManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	FIntPoint detectResolution() const;
	FIntPoint letterboxResolution(const FIntPoint&) const;
	void setResolution(const FIntPoint&) const;

public:
	UFUNCTION(BlueprintCallable)
	void detectAndUseResolution() const;
};