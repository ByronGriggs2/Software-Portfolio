#include "CustomChannels.h"
#include "MyPlayer.h"
#include "Summon.h"
#include "EnemyBase.h"
#include "ExperienceShard.h"
#include "Definitions.h"
//#include "AttackActor.h"

using enum collisionType;

namespace {
	constexpr ECollisionChannel PLAYER = ECC_GameTraceChannel1;
	constexpr ECollisionChannel FRIENDLY = ECC_GameTraceChannel2;
	constexpr ECollisionChannel ENEMY = ECC_GameTraceChannel3;
	constexpr ECollisionChannel FRIENDLY_ATTACK = ECC_GameTraceChannel4;
	constexpr ECollisionChannel ENEMY_ATTACK = ECC_GameTraceChannel5;
	constexpr ECollisionChannel POWERUP = ECC_GameTraceChannel6;
}

bool CustomChannels::convertToChannel(collisionType type, ECollisionChannel& ret) {
	switch (type) {
		case collisionType::player:
			ret = PLAYER;
			return true;
		case collisionType::friendly:
			ret = FRIENDLY;
			return true;
		case collisionType::enemy:
			ret = ENEMY;
			return true;
		case collisionType::friendlyAttack:
			ret = FRIENDLY_ATTACK;
			return true;
		case collisionType::enemyAttack:
			ret = ENEMY_ATTACK;
			return true;
		case collisionType::powerup:
			ret = POWERUP;
			return true;
		default:
			return false;
	}
}
bool CustomChannels::convertToType(ECollisionChannel channel, collisionType& ret) {
	switch (channel) {
	case PLAYER:
		ret = collisionType::player;
		return true;
	case FRIENDLY:
		ret = collisionType::friendly;
		return true;
	case ENEMY:
		ret = collisionType::enemy;
		return true;
	case FRIENDLY_ATTACK:
		ret = collisionType::friendlyAttack;
		return true;
	case ENEMY_ATTACK:
		ret = collisionType::enemyAttack;
		return true;
	case POWERUP:
		ret = collisionType::powerup;
		return true;
	default:
		return false;
	}
}
bool CustomChannels::setupCollision(UPrimitiveComponent* comp, ECollisionChannel channel, bool automaticEvents) {
	collisionType type;
	if (!convertToType(channel, type))
		return false;
	return setupCollision(comp, type, automaticEvents);
}

bool CustomChannels::setupCollision(UPrimitiveComponent* comp, collisionType type, bool automaticEvents) {
	if (!IsValid(comp)) {
		LOGERROR("CustomChannels::setupCollision - parameter is not valid");
		return false;
	}
	ECollisionChannel channel;
	if (!convertToChannel(type, channel))
		return false;
	
	comp->SetCollisionObjectType(channel);
	comp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	comp->SetGenerateOverlapEvents(automaticEvents);
	return addResponses(comp, type);
}

bool CustomChannels::addResponses(UPrimitiveComponent* comp, collisionType type) {
	if (!IsValid(comp)) {
		LOGERROR("CustomChannels::addResponses - parameter is not valid");
		return false;
	}
	TArray<ECollisionChannel> responses;
	if (!getCollisionResponses(type, responses))
		return false;
	for (const auto& response : responses) {
		comp->SetCollisionResponseToChannel(response, ECollisionResponse::ECR_Overlap);
	}
	return true;
}

template<typename T>
bool CustomChannels::getParams(T type, FCollisionObjectQueryParams& ret) {
	static_assert(
		std::is_same_v<T, ECollisionChannel> || std::is_same_v<T, collisionType>,
		"T must be ECollisionChannel or collisionType"
		);
	collisionType ctype;
	if constexpr (std::is_same_v<T, ECollisionChannel>) {
		if (!convertToType(type, ctype))
			return false;
	}
	else {
		ctype = type;
	}
	TArray<ECollisionChannel> responses;
	if (!getCollisionResponses(ctype, responses))
		return false;
	ret = FCollisionObjectQueryParams();
	for (const auto& response : responses) {
		ret.AddObjectTypesToQuery(response);
	}
	return true;
}
template bool CustomChannels::getParams<collisionType>(collisionType type, FCollisionObjectQueryParams& ret);
template bool CustomChannels::getParams<ECollisionChannel>(ECollisionChannel type, FCollisionObjectQueryParams& ret);

bool CustomChannels::getCollisionResponses(collisionType type, TArray<ECollisionChannel>& ret) {
	ret.Empty();
	switch (type) {
	case player:
		ret.Add(ENEMY);
		ret.Add(ENEMY_ATTACK);
		ret.Add(POWERUP);
		break;
	case friendly:
		ret.Add(ENEMY);
		ret.Add(ENEMY_ATTACK);
		break;
	case enemy:
		ret.Add(PLAYER);
		ret.Add(FRIENDLY);
		ret.Add(FRIENDLY_ATTACK);
		break;
	case friendlyAttack:
		ret.Add(ENEMY);
		break;
	case enemyAttack:
		ret.Add(PLAYER);
		ret.Add(FRIENDLY);
		break;
	case powerup:
		ret.Add(PLAYER);
		break;
	default:
		return false;
	}
	return true;
}

template<typename T>
bool CustomChannels::setupCollision(UPrimitiveComponent* comp, bool automaticEvents) {
	return setupCollision(comp, getType<T>(), automaticEvents);
}
template bool CustomChannels::setupCollision<AMyPlayer>(UPrimitiveComponent* comp, bool automaticEvents);
template bool CustomChannels::setupCollision<AEnemyBase>(UPrimitiveComponent* comp, bool automaticEvents);
template bool CustomChannels::setupCollision<ASummon>(UPrimitiveComponent* comp, bool automaticEvents);
template bool CustomChannels::setupCollision<AExperienceShard>(UPrimitiveComponent* comp, bool automaticEvents);

template<typename T>
bool CustomChannels::getParams(FCollisionObjectQueryParams& ret) {
	return getParams(getType<T>(), ret);
}

template<typename T>
collisionType CustomChannels::getType() {
	static_assert(always_false_v<T>, "getType<T> not implemented for this type");
}
template<>
collisionType CustomChannels::getType<AMyPlayer>() {
	return collisionType::player;
}
template<>
collisionType CustomChannels::getType<AEnemyBase>() {
	return collisionType::enemy;
}
template<>
collisionType CustomChannels::getType<ASummon>() {
	return collisionType::friendly;
}
template<>
collisionType CustomChannels::getType<AExperienceShard>() {
	return collisionType::powerup;
}
template collisionType CustomChannels::getType<AMyPlayer>();
template collisionType CustomChannels::getType<AEnemyBase>();
template collisionType CustomChannels::getType<ASummon>();
template collisionType CustomChannels::getType<AExperienceShard>();