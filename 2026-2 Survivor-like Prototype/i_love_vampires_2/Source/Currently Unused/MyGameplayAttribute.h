#include "CoreMinimal.h"
#include "MyGameplayAttribute.generated.h"

USTRUCT(BlueprintType)
struct FMyGameplayAttribute {
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MyGameplayAttribute")
	float _base = 0;
	float _prebonus = 0;
	float _multiplier = 0;
	float _postbonus = 0;

	float getFinal() const { return (_base + _prebonus) * (1 + _multiplier) + _postbonus; }
};