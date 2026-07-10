#include "StatusEnum.h"
#include "EffectStruct.h"

FEffectStruct StatusStatics::getRandomNegativePersistentEffect(float magnitude, float duration) {
	EStatus type;
	float actualMagnitude = 0;
	int rand = FMath::RandRange(0, 7);
	switch (rand) {
	case 0:
		type = EStatus::bleed;
		actualMagnitude = magnitude * 5;
		break;
	case 1:
		type = EStatus::burn;
		actualMagnitude = magnitude * 0.05;
		break;
	case 2:
		type = EStatus::poison;
		actualMagnitude = magnitude * 3;
		break;
	case 3:
		type = EStatus::blind;
		actualMagnitude = 1;
		break;
	case 4:
		type = EStatus::chill;
		actualMagnitude = magnitude * 10;
		if (actualMagnitude > 100)
			actualMagnitude = 100;
		break;
	case 5:
		type = EStatus::decay;
		actualMagnitude = magnitude * 0.08;
		break;
	case 6:
		type = EStatus::root;
		actualMagnitude = 1;
		break;
	case 7:
		type = EStatus::slow;
		actualMagnitude = magnitude * 15;
		if (actualMagnitude > 100)
			actualMagnitude = 100;
		break;
	default:
		type = EStatus::INVALID;
	}
	return FEffectStruct(type, actualMagnitude, duration, 1);
}