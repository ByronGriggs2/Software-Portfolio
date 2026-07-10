#pragma once
#include <type_traits>
#include "AOE.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <memory>
#include "Definitions.h"
#include "AOESpawner.generated.h"

class AOESpawner;
UCLASS()
class I_LOVE_VAMPIRES_2_API AAOESpawner : public AActor {
	GENERATED_BODY()

	const static inline EAOETargeting _LINE = EAOETargeting::spawner_line;
	std::unique_ptr<AOESpawner> _spawner;
	UPROPERTY()
	TWeakObjectPtr<APawn> _pawn;
	std::unique_ptr<AOEInitStruct> _temp = nullptr;

	std::unique_ptr<AOESpawner> createPolySpawner(const AOEInitStruct& spawnTemplate);

public:
	AAOESpawner();
	void initialise_AAOESpawner(const AOEInitStruct& spawnTemplate);
	virtual void PostInitializeComponents();
	virtual void Tick(float delta) override;
	const APawn* getPawn() const { return _pawn.IsValid() ? _pawn.Get() : nullptr; }
};

class AOESpawner {
	const AOEInitStruct _spawnTemplate;
	int _totalAttacks = 0;
	int _remainingAttacks = 0;
	float _attackCooldown = 0;
	float _nextAttackTime = 0;
	bool _killMe = false;

protected:
	TWeakObjectPtr<AActor> _owner;

private:
	// Note that this is not the same as the virtual BaseAttackFactory::launchAttack. As a result, do not expect all of the same functionality.
	void launchAttack();

protected:
	int getTotalAttacks() const { return _totalAttacks; }
	int getRemainingAttacks() const { return _remainingAttacks; }
	const AOEInitStruct& getSpawnTemplate() const { return _spawnTemplate; }
	
	virtual FVector getAttackLocation() = 0;
	//virtual void initLocation() = 0;

public:
	AOESpawner() = delete;
	AOESpawner(AActor* owner, const AOEInitStruct& spawnTemplate);
	virtual ~AOESpawner() = default;
	void tick(float delta);
	bool isFinished() const { return _killMe; }
};