#pragma once
#include "CoreMinimal.h"
#include <string>

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

// Parameters must be passed as std::declval<dataType>()
#define HAS_METHOD(T, RET, METHOD) \
    requires { \
		{ std::declval<T>().METHOD } -> std::convertible_to<RET>; \
	}

#define EPSILON 0.0001f
#define DEFAULT_SCREEN_WIDTH 1920
#define DEFAULT_SCREEN_HEIGHT 1080
#define DEFAULT_SCREEN_RATIO ((float)DEFAULT_SCREEN_WIDTH / (float)DEFAULT_SCREEN_HEIGHT)
#define SPRITE_SCALE 1.0f
#define SPRITE_RADIUS 16.0f
#define SENTINEL_FLOAT -999.0f
#define INFINITE_DURATION 999.0f
#define ORTHO_WIDTH 480.0f
#define SOCKETS_PER_CONTAINER 3
#define PHI 1.61803398875f
//#define PI 3.14159265359f

void LOGERROR(const char* inputString);
void LOGWARNING(const char* inputString);
void LOGINFO(const std::string& inputString);

template<typename>
constexpr bool always_false_v = false;