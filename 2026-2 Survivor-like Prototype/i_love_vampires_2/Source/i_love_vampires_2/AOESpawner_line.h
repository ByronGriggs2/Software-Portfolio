#pragma once
#include "AOESpawner.h"
struct AOEInitStruct;
class AActor;

class AOESpawner_line : public AOESpawner {
	FVector _direction = FVector(1, 0, 0);

protected:
	virtual FVector getAttackLocation() override;
public:
	AOESpawner_line() = delete;
	AOESpawner_line(AActor* owner, const AOEInitStruct& spawnTemplate);
	virtual ~AOESpawner_line() = default;
};