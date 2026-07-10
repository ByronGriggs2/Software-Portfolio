#pragma once
#include "CoreMinimal.h"
#include "InputCoreTypes.h"
#include "InputEnum.h"
#include "KeybindSection.generated.h"

USTRUCT()
struct I_LOVE_VAMPIRES_2_API FKeymap {
	GENERATED_USTRUCT_BODY()

	UPROPERTY() FKey _primaryKey;
	UPROPERTY() FKey _secondaryKey;
};

USTRUCT(BlueprintType)
struct I_LOVE_VAMPIRES_2_API FKeybindSaveData {
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	TMap<EInput_UI, FKeymap> _UI;
	UPROPERTY()
	TMap<EInput_Action, FKeymap> _action;
};

class KeybindSection {
	TMap<EInput_UI, FKeymap> _UI;
	TMap<FKey, TArray<EInput_UI>> _UIReverse;
	TMap<EInput_Action, FKeymap> _action;
	TMap<FKey, TArray<EInput_Action>> _actionReverse;

	static FKey getDefaultKey(EInput_UI input);
	static FKey getDefaultKey(EInput_Action input);

public:
	KeybindSection();
	void setKeybind(EInput_UI input, FKey key, bool isPrimary);
	void setKeybind(EInput_Action input, FKey key, bool isPrimary);
	void resetKeybind(EInput_UI input);
	void resetKeybind(EInput_Action input);
	FKeybindSaveData getSaveData() const { return FKeybindSaveData{ _UI, _action }; }
	void loadFromSaveData(const FKeybindSaveData& data);

	const TMap<FKey, TArray<EInput_UI>>& getUIReverseMap() const { return _UIReverse; }
	const TMap<FKey, TArray<EInput_Action>>& getActionReverseMap() const { return _actionReverse; }
};