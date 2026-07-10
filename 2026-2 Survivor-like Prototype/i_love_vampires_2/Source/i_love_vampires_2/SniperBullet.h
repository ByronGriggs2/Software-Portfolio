#pragma once
#include "CoreMinimal.h"
#include "Projectile.h"
#include "ProjectileEnum.h"
#include "SniperBullet.generated.h"

UCLASS()
class I_LOVE_VAMPIRES_2_API ASniperBullet : public AProjectile
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
};