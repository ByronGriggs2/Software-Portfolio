#pragma once
#include "CoreMinimal.h"
#include "BaseAttributes.h"
#include "AOEAttributes.generated.h"

class I_LOVE_VAMPIRES_2_API UAOEAttributes : public UBaseAttributes
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "AOEAttributes")
	float _radius = 1.f;
	UPROPERTY(EditAnywhere, Category = "AOEAttributes")
	float _duration = 0.f;
};
