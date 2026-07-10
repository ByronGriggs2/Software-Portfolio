#include "AttackFactoryTemplate.h"
#include "Definitions.h"

#include "GameFramework/Pawn.h"

#include "AttackConfig.h"
#include "AttackAttributes.h"
#include "AttackFactory.h"

#include "ProjectileConfig.h"
#include "ProjectileAttributes.h"

#include ""

bool UAttackFactoryTemplate::isValid() const {
	if (!IsValid(_attackConfig) || !IsValid(_attackAttributes)) {
		LOGERROR("UAttackFactoryTemplate::createAttackFactory - invalid");
		return false;
	}
	return true;
}
UAttackFactory* UAttackFactoryTemplate::createAttackFactory(APawn* pawnRef) const {
	if (!isValid())
		return nullptr;

	UAttackFactory* factory = NewObject<UAttackFactory>();
	factory->initialise_UAttackFactory(pawnRef, _attackConfig, _attackAttributes);
	return factory;
}