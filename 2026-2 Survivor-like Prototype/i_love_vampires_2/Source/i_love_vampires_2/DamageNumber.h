#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageNumber.generated.h"
class AActor;

UCLASS(Blueprintable)
class I_LOVE_VAMPIRES_2_API UDamageNumber : public UUserWidget
{
	GENERATED_BODY()

	TWeakObjectPtr<const AActor> _instigator;
	FVector _offset;
	FVector _lastKnownLocation;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float _damageAmount = 0;

private:
	void moveToApparentLocation(const FVector& location);

public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void initialise_UDamageNumber(AActor* instigator, const FVector& offset, float damageAmount);
	void initialise_UDamageNumber_Implementation(AActor* instigator, const FVector& offset, float damageAmount);
};