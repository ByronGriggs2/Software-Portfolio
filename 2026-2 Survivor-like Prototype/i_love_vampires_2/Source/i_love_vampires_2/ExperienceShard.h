#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "SpriteEnum.h"
#include "ExperienceShard.generated.h"
class UPaperFlipbookComponent;
class AMyPlayer;
class USphereComponent;

UCLASS()
class I_LOVE_VAMPIRES_2_API AExperienceShard : public AActor
{
	GENERATED_BODY()

	//const static inline ESprite _DEFAULT_SPRITE = ESprite::testShard;
	const static inline float _MAGNETISM_RADIUS = 500;
	const static inline float _STARTING_SPEED = -500;
	const static inline float _ACCELERATION = 2500;
	const static inline float _SIZE_MULTIPLIER = 0.5f;
	//afaik it's not possible to perform a sweep with the flipbook, so I model it as a sphere to guarantee that it doesn't teleport over the player. 
	
	float _experienceValue = -1;
	bool _magnetismEnabled = false;
	float _speed = 0;
	TWeakObjectPtr<AMyPlayer> _playerRef = nullptr;
	UPROPERTY()
	USphereComponent* _collider = nullptr;

public:
	UPROPERTY(VisibleAnywhere)
	UPaperFlipbookComponent* _flipbook = nullptr;

private:
	void accelerateTowardsPlayer(float delta);

public:
	void initialise_AExperienceShard(float experienceValue);
	AExperienceShard();
	virtual void BeginPlay() override;
	virtual void Tick(float delta) override;

	void beginMagnetism(AMyPlayer*);
	float getExperienceValue() const { return _experienceValue; }
	bool isMagnetised() const { return _magnetismEnabled; }
	void suicide();
};