#include "unrealHelpers.h"
#include "PaperFlipbookComponent.h"
#include "MyGameplayStatics.h"
//#include "SpriteManager.h"
#include "GameFramework/Actor.h"
#include "Definitions.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Pawn.h"
#include "PaperFlipbook.h"
#include "Components/ActorComponent.h"
#include "AssetRefs.h"
#include "Materials/MaterialInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include <cmath>
#include <type_traits>
#include "MyPlayer.h"
#include "DamageNumber.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "GameplayTagsManager.h"
#include "Math/UnrealMathUtility.h"
#include "Engine/OverlapResult.h"
#include "CustomChannels.h"
#include "EnemyBase.h"
#include "Summon.h"
#include "ExperienceShard.h"

float unrealHelpers::getYaw(const FVector& direction) {
	return FMath::Atan2(direction.Z, direction.X);
}

FVector unrealHelpers::makeFromYaw(float yaw) {
	return FVector(FMath::Cos(yaw), 0, FMath::Sin(yaw));
}

bool unrealHelpers::snapSprite(const AActor* caller, const UActorComponent* comp, UPaperFlipbookComponent* flipbook) {
	if (!IsValid(caller) || !IsValid(comp) || !IsValid(flipbook)) {
		LOGERROR("unrealHelpers::snapSprite - invalid parameter");
		return false;
	}
	FVector currentPos = caller->GetActorLocation();
	currentPos.X = std::round(currentPos.X);
	currentPos.Z = std::round(currentPos.Z);
	FHitResult* throwaway = nullptr;
	flipbook->SetWorldLocation(currentPos, false, throwaway, ETeleportType::TeleportPhysics);
	return true;
}

bool unrealHelpers::initFlipbook(UPaperFlipbook* sprite, UPaperFlipbookComponent* flipbook) {
	if (!IsValid(flipbook)) {
		LOGERROR("unrealHelpers::initFlipbook - parameter is not valid");
		return false;
	}
	if (!IsValid(sprite)) {
		LOGERROR("unrealHelpers::initFlipbook - sprite is invalid");
		return false;
	}
	flipbook->SetFlipbook(sprite);
	UAssetRefs* refs = nullptr;
	if (!MyGameplayStatics::getAssetRefs(refs)) {
		LOGERROR("unrealHelpers::initFlipbook - AssetRefs is invalid");
		return false;
	}
	UMaterialInterface* mat = refs->getSpriteMaterial();
	if (!IsValid(mat)) {
		LOGERROR("unrealHelpers::initFlipbook - mat is invalid");
		return false;
	}
	flipbook->SetMaterial(0, mat);
	return true;
}

//bool unrealHelpers::constructFlipbookInternal(AActor* caller, USceneComponent* rootComp, UPaperFlipbookComponent* flipbook, bool collision, bool automaticCollision) {
//	if (!IsValid(flipbook)) {
//		LOGERROR("unrealHelpers::constructFlipbook - failed to create flipbook");
//		return false;
//	}
//	flipbook->SetupAttachment(rootComp);
//	if (collision) {
//		// Sets flipbook to show up in sweeps (both manual and automatic) but not block objects. This is generally undesired in attacks because they implement their hitboxes
//		// seperately. For example, AOE has a sphere component it uses in addition to its flipbook
//		flipbook->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
//	}
//	// Sets flipbook to automatically check for overlaps. Generally not desired.
//	flipbook->SetGenerateOverlapEvents(automaticCollision);
//	return true;
//}
//
//// The code here is identical but in case 1 a raw ptr rvalue is assigned to a raw ptr lvalue, and in case 2 a raw ptr rvalue is used as a parameter to construct a TObjectPtr lvalue
//bool unrealHelpers::constructFlipbook(AActor* caller, USceneComponent* rootComp, UPaperFlipbookComponent*& flipbook, bool collision, bool automaticCollision) {
//	if (!IsValid(caller)) {
//		LOGERROR("unrealHelpers::constructFlipbook - parameter is not valid");
//		return false;
//	}
//	flipbook = caller->CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("flipbook"));
//	return constructFlipbookInternal(caller, rootComp, flipbook, collision, automaticCollision);
//}
//bool unrealHelpers::constructFlipbook(AActor* caller, USceneComponent* rootComp, TObjectPtr<UPaperFlipbookComponent>& flipbook, bool collision, bool automaticCollision) {
//	if (!IsValid(caller)) {
//		LOGERROR("unrealHelpers::constructFlipbook - parameter is not valid");
//		return false;
//	}
//	flipbook = caller->CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("flipbook"));
//	return constructFlipbookInternal(caller, rootComp, flipbook, collision, automaticCollision);
//}

template<typename T>
bool unrealHelpers::getOverlappingActors(T* caller, UPrimitiveComponent* collider, TSet<AActor*>& ret, TArray<AActor*> ignoredActors) {
	static_assert(std::is_base_of_v<AActor, T>, "unrealHelpers::getOverlappingActors - T must be a subclass of AActor");

	return getOverlappingActors(caller, CustomChannels::getType<T>(), collider, ret, ignoredActors);
}
template bool unrealHelpers::getOverlappingActors<AMyPlayer>(AMyPlayer* caller, UPrimitiveComponent* collider, TSet<AActor*>& ret, TArray<AActor*> ignoredActors);
template bool unrealHelpers::getOverlappingActors<AEnemyBase>(AEnemyBase* caller, UPrimitiveComponent* collider, TSet<AActor*>& ret, TArray<AActor*> ignoredActors);
template bool unrealHelpers::getOverlappingActors<ASummon>(ASummon* caller, UPrimitiveComponent* collider, TSet<AActor*>& ret, TArray<AActor*> ignoredActors);
template bool unrealHelpers::getOverlappingActors<AExperienceShard>(AExperienceShard* caller, UPrimitiveComponent* collider, TSet<AActor*>& ret, TArray<AActor*> ignoredActors);

template<typename T>
bool unrealHelpers::getOverlappingActors_internal(AActor* caller, T type, UPrimitiveComponent* collider, TSet<AActor*>& ret, TArray<AActor*> ignoredActors) {
	static_assert(
		std::is_same_v<T, ECollisionChannel> || std::is_same_v<T, collisionType>,
		"T must be ECollisionChannel or collisionType"
		);
	//LOGINFO("get overlapping actors called with object type" + std::to_string(static_cast<uint8>(type)));
	if (!IsValid(caller) || !IsValid(collider)) {
		LOGERROR("unrealHelpers::getOverlappingActors- invalid parameter");
		return false;
	}
	UWorld* world = caller->GetWorld();
	if (!IsValid(world)) {
		LOGERROR("unrealHelpers::performSweepAtPawn_internal - world is invalid");
		return false;
	}
	ret.Empty();
	
	FCollisionObjectQueryParams params;
	FComponentQueryParams params2;
	if (!makeParams<T>(caller, type, ignoredActors, params, &params2)) {
		LOGERROR("unrealHelpers::getOverlappingActors - failed to make collision params");
		return false;
	}
	TArray<FOverlapResult> temp;
	world->ComponentOverlapMulti(
		temp,
		collider,
		caller->GetActorLocation(),
		caller->GetActorRotation(),
		params2,
		params
	);
	for (auto& result : temp) {
		ret.Add(result.GetActor());
		//LOGINFO("result found with type" + std::to_string(static_cast<uint8>(result.Component->GetCollisionObjectType())));
	}
	return true;
}
template bool unrealHelpers::getOverlappingActors_internal<collisionType>(AActor* caller, collisionType type, UPrimitiveComponent* collider, TSet<AActor*>& ret, TArray<AActor*> ignoredActors);
template bool unrealHelpers::getOverlappingActors_internal<ECollisionChannel>(AActor* caller, ECollisionChannel type, UPrimitiveComponent* collider, TSet<AActor*>& ret, TArray<AActor*> ignoredActors);

template<typename T>
bool unrealHelpers::makeParams(UObject* caller, T type, TArray<AActor*> ignoredActors, FCollisionObjectQueryParams& outParams, FCollisionQueryParams* polymorphicParams) {
	static_assert(
		std::is_same_v<T, ECollisionChannel> || std::is_same_v<T, collisionType>,
		"T must be ECollisionChannel or collisionType"
		);
	if (polymorphicParams == nullptr) {
		LOGERROR("unrealHelpers::getOverlappingActors - polymorphicParams is null");
		return false;
	}
	if (!CustomChannels::getParams<T>(type, outParams)) {
		LOGERROR("unrealHelpers::getOverlappingActors - failed to get collision params");
		return false;
	}
	polymorphicParams->bFindInitialOverlaps = true;
	auto casted = Cast<AActor>(caller);
	if (IsValid(casted))
		polymorphicParams->AddIgnoredActor(casted);
	for (const auto& act : ignoredActors) {
		if (IsValid(act))
			polymorphicParams->AddIgnoredActor(act);
	}
	return true;
}

bool unrealHelpers::performSweep(UObject* caller, collisionType type, const FVector& startPos, const FVector& endPos, const FCollisionShape& shape, TArray<FHitResult>& OutHits, const TArray<AActor*>& ignoredActors) {
	if (!IsValid(caller)) {
		LOGERROR("unrealHelpers::performSweepAtPawn_Sphere - caller is not valid");
		return false;
	}
	UWorld* world = caller->GetWorld();
	if (!IsValid(world)) {
		LOGERROR("unrealHelpers::performSweepAtPawn_internal - world is invalid");
		return false;
	}

	FCollisionObjectQueryParams params;
	FCollisionQueryParams params2;
	if (!makeParams(caller, type, ignoredActors, params, &params2)) {
		LOGERROR("unrealHelpers::performSweepAtPawn_Sphere - failed to make collision params");
		return false;
	}
	world->SweepMultiByObjectType(OutHits, startPos, endPos, FQuat::Identity, params, shape, params2);
	return true;
}

bool unrealHelpers::getActorSpawnTransform(AActor* caller, FTransform& ret) {
	if (!IsValid(caller)) {
		LOGERROR("unrealHelpers::spawnActorOnTopOfMe - caller is not valid");
		return false;
	}

	FVector location = caller->GetActorLocation();
	FRotator rot = caller->GetActorRotation();
	FVector scale(1, 1, 1);
	ret = FTransform(rot, location, scale);
	return true;
}

float unrealHelpers::getAngleBetweenVectors(const FVector& from, const FVector& to) {
	const float num = FVector::DotProduct(from.GetSafeNormal(), to.GetSafeNormal());
	//const float den = from.Size() * to.Size();
	return std::acos(num) * (360 / (2 * PI));
}

void unrealHelpers::lookAtDirection(AActor* caller, float X, float Z) {
	const FRotator rotation = UKismetMathLibrary::FindLookAtRotation(FVector(0, 0, 0), FVector(X,0,Z));
	caller->SetActorRotation(rotation, ETeleportType::TeleportPhysics);
}

float unrealHelpers::getOrthoWidth(AMyPlayer* caller) {
	if (!IsValid(caller))
		return 0;
	return caller->getOrthoWidth();
}

bool unrealHelpers::spawnDamageNumberNearMe(AActor* caller, const FVector& offset, float damageAmount) {
	if (!IsValid(caller)) {
		LOGERROR("unrealHelpers::spawnDamageNumberNearMe - caller is not valid");
		return false;
	}
	auto player = UGameplayStatics::GetPlayerController(caller, 0);
	if (!IsValid(player)) {
		LOGERROR("unrealHelpers::spawnDamageNumberNearMe - failed to get player controller");
		return false;
	}
	UAssetRefs* refs = nullptr;
	if (!MyGameplayStatics::getAssetRefs(refs)) {
		LOGERROR("unrealHelpers::spawnDamageNumberNearMe - failed to get asset refs");
		return false;
	}
	TSubclassOf<UDamageNumber> damagePopupClass = refs->getDamagePopupWidgetClass();
	if (!IsValid(damagePopupClass)) {
		LOGERROR("unrealHelpers::spawnDamageNumberNearMe - damage popup class not valid");
		return false;
	}
	UDamageNumber* widget = Cast<UDamageNumber>(UWidgetBlueprintLibrary::Create(caller, damagePopupClass, player));
	if (!IsValid(widget)) {
		LOGERROR("unrealHelpers::spawnDamageNumberNearMe - failed to create widget");
		return false;
	}
	widget->initialise_UDamageNumber(caller, offset, damageAmount);
	return true;
}

void unrealHelpers::makeRandomLineSegment(AMyPlayer* caller, float length, FVector& outStart, FVector& outDirection) {
	const float orthoWidth = getOrthoWidth(caller);
	const float screenHeight = orthoWidth / DEFAULT_SCREEN_RATIO;
	const float screenWidth = orthoWidth;
	auto player = UGameplayStatics::GetPlayerPawn(caller, 0);
	FVector center = player->GetActorLocation();

	const float angle = FMath::FRandRange(0, 2*PI);
	float minX = center.X - screenWidth / 2;
	float maxX = center.X + screenWidth / 2;
	float minZ = center.Z - screenHeight / 2;
	float maxZ = center.Z + screenHeight / 2;
	if (0 <= angle && angle < PI / 2) {
		maxX = maxX - length * abs(FMath::Cos(angle));
		maxZ = maxZ - length * abs(FMath::Sin(angle));
	}
	else if (PI / 2 <= angle && angle < PI) {
		minX = minX + length * abs(FMath::Cos(angle));
		maxZ = maxZ - length * abs(FMath::Sin(angle));
	}
	else if (PI <= angle && angle < 3 * PI / 2) {
		minX = minX + length * abs(FMath::Cos(angle));
		minZ = minZ + length * abs(FMath::Sin(angle));
	}
	else {
		maxX = maxX - length * abs(FMath::Cos(angle));
		minZ = minZ + length * abs(FMath::Sin(angle));
	}
	outStart.X = FMath::FRandRange(minX, maxX);
	outStart.Z = FMath::FRandRange(minZ, maxZ);
	outStart.Y = center.Y;
	outDirection.X = FMath::Cos(angle);
	outDirection.Y = 0;
	outDirection.Z = FMath::Sin(angle);
}

bool unrealHelpers::hasTag(const FGameplayTagContainer& container, const FName& tag) {
	UGameplayTagsManager& manager = UGameplayTagsManager::Get();
	bool failure = false;
	FGameplayTag tempTag = manager.RequestGameplayTag(tag, failure);
	if (failure) {
		LOGERROR("unrealHelpers::hasTag - failed to request gameplay tag");
		return false;
	}
	return tempTag.MatchesAny(container);
}

FVector unrealHelpers::getRandomLocationOnScreen(AMyPlayer* caller) {
	if (!IsValid(caller))
		return FVector(0, 0, 0);
	const float orthoWidth = getOrthoWidth(caller) / 2.0f;
	float X = FMath::FRandRange(-orthoWidth, orthoWidth);
	float Z = FMath::FRandRange(-orthoWidth / DEFAULT_SCREEN_RATIO, orthoWidth / DEFAULT_SCREEN_RATIO);
	return caller->GetActorLocation() + FVector(X, 0, Z);
}

float unrealHelpers::getDistance(const FVector& left, const FVector& right) {
	return std::pow(std::pow(left.X - right.X, 2) + std::pow(left.Z - right.Z, 2), 0.5);
}