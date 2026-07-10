#pragma once
#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "CollisionQueryParams.h"
#include "CustomChannelEnums.h"

class UPrimitiveComponent;

class CustomChannels {
private:
	static bool convertToChannel(collisionType type, ECollisionChannel& ret);
	static bool convertToType(ECollisionChannel channel, collisionType& ret);
	static bool addResponses(UPrimitiveComponent* comp, collisionType type);
	static bool getCollisionResponses(collisionType type, TArray<ECollisionChannel>& ret);

public:
	CustomChannels() = delete;
	static bool setupCollision(UPrimitiveComponent* comp, collisionType type, bool automaticEvents = false);
	static bool setupCollision(UPrimitiveComponent* comp, ECollisionChannel channel, bool automaticEvents = false);
	template<typename T>
	static bool getParams(T type, FCollisionObjectQueryParams& ret);
	template<typename T>
	static bool setupCollision(UPrimitiveComponent* comp, bool automaticEvents = false);
	template<typename T>
	static bool getParams(FCollisionObjectQueryParams& ret);
	template<typename T>
	static collisionType getType();
};