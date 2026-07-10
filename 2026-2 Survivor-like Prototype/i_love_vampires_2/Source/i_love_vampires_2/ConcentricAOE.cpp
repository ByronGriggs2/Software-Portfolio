#include "ConcentricAOE.h"
// Used to create ExplosiveAOEFactory
#include "ExplosiveAOE.h"

TArray<AAOE*> ConcentricAOEFactory::launchAttack_Impl(const FVector& forward) {
	const float radius = SPRITE_RADIUS;
	const float trueRadius = radius * getMember(&AttackAttributes::_radius);
	TArray<AAOE*> ret;

	auto mainAOE = AOEFactory::launchAttack_Impl(forward);
	if (mainAOE.Num() != 1) {
		LOGERROR("ConcentricAOEFactory::launchAttack - failed to launch main AOE");
		return {};
	}
	ret.Add(mainAOE[0]);
	for (auto& childFactory : _childFactories) {
		TArray<AAOE*> childAOE = childFactory->launchAttack_Impl(forward);
		if (childAOE.Num() > 0 && IsValid(childAOE[0])) {
			childAOE[0]->SetActorLocation(mainAOE[0]->GetActorLocation(), false, nullptr, ETeleportType::TeleportPhysics);
			ret.Add(childAOE[0]);
		}
	}
	return ret;
}