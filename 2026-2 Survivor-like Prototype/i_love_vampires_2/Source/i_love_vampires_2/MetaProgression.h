#pragma once
#include <vector>
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MetaProgression.generated.h"
class UTechWidget;

UCLASS(BlueprintType)
class I_LOVE_VAMPIRES_2_API UMetaProgression : public UUserWidget {
	GENERATED_BODY()

	int _resourcePoints = 0;

	void notEnoughResource() const {}
	UFUNCTION() void onLeftClick(int techID);
	UFUNCTION() void onRightClick(int techID) {}
	
protected:
	// Technologies are added manually in the editor :(
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<UTechWidget>> _techWidgets;

public:
	virtual void NativeOnInitialized() override;
};