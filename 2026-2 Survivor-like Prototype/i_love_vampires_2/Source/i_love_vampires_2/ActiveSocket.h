#pragma once
#include <memory>
#include "CoreMinimal.h"
#include "ActiveSocket.generated.h"
class Active;
class UWeaponTemplate;

USTRUCT(BlueprintType)
struct I_LOVE_VAMPIRES_2_API FSocketData {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly) bool _isFilled = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) TWeakObjectPtr<const UWeaponTemplate> _runtimeData;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int _level;
};

class ActiveSocket
{
	bool _occupied = false;
	std::weak_ptr<Active> _active;

public:
	void setActive(std::shared_ptr<Active> active) { _active = active; _occupied = true; }
	std::shared_ptr<Active> getActive() { return _active.lock(); }
	bool isOccupied() const { return _occupied; }
	bool isValid() const { return !(_occupied && _active.expired()); }
	const std::shared_ptr<const Active> getActive() const { return _active.lock(); }
	bool getSocketData(FSocketData& out) const;
};	