#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MetaProgressionEnums.h"
#include "Input/Reply.h"
#include "TechWidget.generated.h"
class UTechnology;
class UImage;

UCLASS(BlueprintType)
class I_LOVE_VAMPIRES_2_API UTechWidget : public UUserWidget
{
	GENERATED_BODY()

	struct StateDetails {
		bool _visible = true;
		bool _grayscale = false;
		bool _isDramatic = false;
	};

	UPROPERTY()
	TObjectPtr<UTechnology> _diskData = nullptr;
	UPROPERTY()
	UImage* _renderedIcon = nullptr;

	ETechnologyState _state = ETechnologyState::INVALID;
	StateDetails _currentDetails;
	int _level = 0;
	int _ID = -1;

	UTexture2D* getDiskTexture(bool& success) const;
	void setState(ETechnologyState newState);
	void setVisible(bool);
	void setGrayscale(bool);
	void setDramatic(bool);
	void createTooltip() const {}

public:
	UTechWidget(const FObjectInitializer& init);
	bool initialise_UTechWidget(UTechnology* diskData, ETechnologyState state);

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLeftClick, int, techID);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRightClick, int, techID);
	UPROPERTY(BlueprintAssignable, Category = "Events") FOnLeftClick _leftClick;
	UPROPERTY(BlueprintAssignable, Category = "Events") FOnRightClick _rightClick;

	UTechnology* getDiskData() const;
	ETechnologyState getState() const { return _state; }
	int getLevel() const { return _level; }
	void upgrade() { _level++; }
	bool prerequisitesMet() const { return true; }
	void giveID(int newID) { _ID = newID; }
};