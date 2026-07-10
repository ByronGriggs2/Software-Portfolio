#pragma once
#include "CoreMinimal.h"
#include "StatusEnum.h"
#include "EffectStruct.generated.h"

USTRUCT(BlueprintType)
struct FEffectStruct {
	GENERATED_USTRUCT_BODY()

	FEffectStruct() = default;
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EffectStruct")
	EStatus _type = static_cast<EStatus>(0);
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EffectStruct")
	float _magnitude = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EffectStruct")
	float _duration = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EffectStruct")
	float _chance = 1;
	//Rediculous I have to add this myself but whatever
	FEffectStruct(EStatus type, float mag, float dur, float cha) : _type(type), _magnitude(mag), _duration(dur), _chance(cha) {}
	bool operator==(const FEffectStruct& other) const;
};
