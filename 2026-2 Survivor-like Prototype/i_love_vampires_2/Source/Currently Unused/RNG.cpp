#include "RNG.h"
#include <cstdlib>
#include <time.h>
#include "Math/UnrealMathUtility.h"

URNG::URNG() {
	srand(time(NULL));
}

float URNG::randFloat() {
	return FMath::FRand(rand());
}

float URNG::randFloatRange(float min, float max) {
	return FMath::FRandRange(rand(), min, max);
}

int URNG::randInt() {
	return rand();
}

int URNG::randIntRange(int min, int max) {
	return min + rand() % (max - min + 1);
}