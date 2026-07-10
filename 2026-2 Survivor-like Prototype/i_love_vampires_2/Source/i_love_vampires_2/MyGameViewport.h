#pragma once
#include "CoreMinimal.h"
#include "Engine/GameViewportClient.h"
#include "MyGameViewport.generated.h"

UCLASS()
class I_LOVE_VAMPIRES_2_API UMyGameViewport : public UGameViewportClient
{
	GENERATED_BODY()

public:
	virtual void Activated(FViewport* InViewport, const FWindowActivateEvent& ActivateEvent) override;
};