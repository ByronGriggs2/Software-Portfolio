#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include <vector>
#include "UObject/WeakObjectPtr.h"
#include "EnemySubsystem.generated.h"
class AActor;
struct MyStruct;

UCLASS()
class I_LOVE_VAMPIRES_2_API UEnemySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	UEnemySubsystem();

	std::vector<MyStruct> _enemyReferences;
	int _nextKey = 0;

public:
	UFUNCTION(BlueprintCallable)
	int registerEnemy(const AActor* enemy);

	UFUNCTION(BlueprintCallable)
	void removeFromRegister(int key);

	UFUNCTION(BlueprintCallable)
	int getEnemyCount() {
		return _enemyReferences.size();
	}

	TWeakObjectPtr<const AActor> getRandomEnemyPtr();
};

struct MyStruct {
	int key = 0;
	TWeakObjectPtr<const AActor> value;
};