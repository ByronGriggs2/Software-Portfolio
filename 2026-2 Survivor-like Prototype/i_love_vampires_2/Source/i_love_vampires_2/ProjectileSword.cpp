#include "ProjectileSword.h"

ProjectileSwordFactory::ProjectileSwordFactory(ACombatant* owner, const UAOETemplate* aoeTemplate, const UProjectileTemplate* projectileTemplate) : AOEFactory(owner, aoeTemplate), _projectileFactory(owner, projectileTemplate) {}

ProjectileSwordFactory::ProjectileSwordFactory(ProjectileSwordFactory&& other) :
	AOEFactory(std::move(other)),
	_projectileFactory(std::move(other._projectileFactory))
{}

TArray<AAOE*> ProjectileSwordFactory::launchAttack_Impl(const FVector& forward) {
	const float radius = SPRITE_RADIUS;
	const float trueRadius = radius * getMember(&AttackAttributes::_radius);

	auto mainAttack = AOEFactory::launchAttack_Impl(forward);
	TArray<AProjectile*> projectiles = _projectileFactory.launchAttack_internal_projectile(forward);

	for (AProjectile* projectile : projectiles) {
		if (!IsValid(projectile))
			continue;
		const FVector currentDir = projectile->getDirection();
		const FVector currentLocation = projectile->GetActorLocation();
		const FVector offset = currentDir * trueRadius;
		projectile->AddActorWorldOffset(offset, false, nullptr, ETeleportType::TeleportPhysics);
	}
	return mainAttack;
}