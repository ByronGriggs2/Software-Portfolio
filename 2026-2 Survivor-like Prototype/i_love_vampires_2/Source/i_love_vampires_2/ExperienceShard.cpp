#include "ExperienceShard.h"
#include "unrealHelpers.h"
#include "PaperFlipbookComponent.h"
#include "MyPlayer.h"
#include "Components/SphereComponent.h"
#include "Definitions.h"
#include "AssetRefs.h"
#include "PaperFlipbook.h"
#include "MyGameplayStatics.h"
#include "SpriteSorter.h"
#include "CustomChannels.h"

void AExperienceShard::suicide() {
	Destroy();
}

void AExperienceShard::initialise_AExperienceShard(float val) {
	_experienceValue = val;
}

AExperienceShard::AExperienceShard() {
	PrimaryActorTick.bCanEverTick = true;
	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	}
	{
		_flipbook = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("flipbook"));
		_flipbook->SetupAttachment(RootComponent);
	}
	{
		_collider = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
		_collider->InitSphereRadius(_MAGNETISM_RADIUS);
		_collider->SetupAttachment(RootComponent);
	}
}

void AExperienceShard::BeginPlay() {
	auto end = [this]() {
		Super::BeginPlay();
		return;
		};

	USpriteSorter* sorter = nullptr;
	if (!MyGameplayStatics::getSpriteSorter(this, sorter)) {
		LOGERROR("AExperienceShard::BeginPlay - failed to get sprite sorter");
		end();
	}
	if (_experienceValue < EPSILON) {
		LOGERROR("AExperienceShard::BeginPlay - invalid experienceValue");
		end();
	}
	{
		if (!CustomChannels::setupCollision<AExperienceShard>(_flipbook, true)) {
			LOGERROR("AExperienceShard::Beginplay - failed to construct flipbook");
			return;
		}
		const FVector currentScale = GetActorScale3D();
		SetActorScale3D(currentScale * _SIZE_MULTIPLIER);
		//Override CustomChannels collision enabled to disable collision
		_flipbook->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		if (!CustomChannels::setupCollision<AExperienceShard>(_collider)) {
			LOGERROR("AExperienceShard::AExperienceShard - failed to construct collider");
			return;
		}
	}
	{
		UAssetRefs* refs = nullptr;
		if (!MyGameplayStatics::getAssetRefs(refs)) {
			LOGERROR("AExperienceShard::AExperienceShard - invalid UAssetRefs");
			end();
		}
		UPaperFlipbook* sprite = refs->getExperienceSprite();
		if (!IsValid(sprite)) {
			LOGERROR("AExperienceShard::AExperienceShard - invalid flipbook");
			end();
		}
		if (!unrealHelpers::initFlipbook(sprite, _flipbook))
			end();
		sorter->sortSprite<AExperienceShard>(TArray<UPaperFlipbookComponent*>({ _flipbook }));
	}
	end();
}

void AExperienceShard::Tick(float delta) {
	Super::Tick(delta);
	if (!_magnetismEnabled)
		return;
	accelerateTowardsPlayer(delta);
}

void AExperienceShard::accelerateTowardsPlayer(float delta) {
	if (!_playerRef.IsValid()) {
		suicide();
		return;
	}
	const FVector myPos = GetActorLocation();
	const FVector playerPos = _playerRef->GetActorLocation();
	const FVector difference = playerPos - myPos;
	const FVector direction = difference.GetSafeNormal(EPSILON, FVector(1, 0, 0));
	const FVector endPos = myPos + direction * _speed * delta;
	// This block is one of two ways the experience shard can be collected by the player. To ensure that the shard doesn't teleport over the player, a sweep is performed, modelling the shard
	// as a sphere. 
	// The other way it can be collected is if the Player's sprite is overlapping the shard's sprite during AMyPlayer::Tick(float)
	{
		TArray<struct FHitResult> OutHits;
		if (!unrealHelpers::performSweep(
			this,
			CustomChannels::getType<AExperienceShard>(),
			myPos,
			endPos,
			FCollisionShape::MakeSphere(SPRITE_RADIUS),
			OutHits)
			) {
			LOGERROR("AExperienceShard::accelerateTowardsPlayer - performSweep failed");
			return;
		}
		if (OutHits.Num() > 0) {
			if (OutHits[0].GetActor() != _playerRef.Get()) {
				LOGERROR("AExperienceShard::accelerateTowardsPlayer - performSweep hit something that is not the player");
				return;
			}
			_playerRef->handleExperienceShardCollision(this);
		}
		
	}
	{
		const FVector displacement = direction * _speed * delta;
		FHitResult* throwaway = nullptr;
		AddActorWorldOffset(displacement, false, throwaway, ETeleportType::TeleportPhysics);
	}
	_speed += _ACCELERATION * delta;
}

void AExperienceShard::beginMagnetism(AMyPlayer* player) {
	if (!IsValid(player)) {
		LOGERROR("AExperienceShard::beginMagnetism - player is not valid");
		return;
	}
	_flipbook->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	_collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	_magnetismEnabled = true;
	_playerRef = TWeakObjectPtr<AMyPlayer>(player);
	_speed = _STARTING_SPEED;
}