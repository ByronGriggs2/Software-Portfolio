#pragma once
#include "DamageNumber.h"
#include "Definitions.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "TestingDefinitions.h"

void UDamageNumber::initialise_UDamageNumber_Implementation(AActor* instigator, const FVector& offset, float damageAmount) {
	if (!IsValid(instigator)) {
		LOGERROR("UDamageNumber::initialise_UDamageNumber_Implementation - invalid instigator");
		return;
	}
	AddToViewport();
	_instigator = instigator;
	_offset = offset;
	if (SHOW_ALL_NUMBERS)
		_damageAmount = damageAmount;
	else
		_damageAmount = damageAmount >= 10 ? round(damageAmount) : round(damageAmount * 10) / 10.0f;
	_lastKnownLocation = instigator->GetActorLocation();
}

void UDamageNumber::NativeTick(const FGeometry& MyGeometry, float InDeltaTime) {
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (!_instigator.IsValid()) {
		moveToApparentLocation(_lastKnownLocation + _offset);
	}
	else {
		moveToApparentLocation(_instigator->GetActorLocation() + _offset);
		_lastKnownLocation = _instigator->GetActorLocation();
	}
}

void UDamageNumber::moveToApparentLocation(const FVector& location) {
	FVector2D screenPos;
	auto temp = UGameplayStatics::GetPlayerController(this, 0);
	// Player most likely died this or last frame. Just leave the widget where it was last frame.
	if (!IsValid(temp) || !IsValid(UGameplayStatics::GetPlayerPawn(this, 0))) {
		return;
	}
	if (!UGameplayStatics::ProjectWorldToScreen(temp, location, screenPos)) {
	LOGERROR("unrealHelpers::spawnDamageNumberNearMe - failed to project world to screen");
		return;
	}
	SetAlignmentInViewport(FVector2D(0.5, 0.5));
	SetPositionInViewport(screenPos);
}