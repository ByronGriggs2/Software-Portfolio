#include "Projectile.h"
#include "Definitions.h"

#include "Math/Quat.h"
#include "CollisionShape.h"
#include "Engine/HitResult.h"
#include "CombatantManager.h"
#include "Combatant.h"
#include "Engine/World.h"
#include "MyGameplayStatics.h"
#include "helpers.h"
#include "unrealHelpers.h"
#include "PaperFlipbookComponent.h"
#include <cmath>
#include "EnemyBase.h"
#include "SniperBullet.h"
#include "ProjectileSword.h"
#include "CustomChannelEnums.h"

///////////////////////////////////////////////////////////////////////////////
namespace {
	float dist(float x, float y) {
		return std::sqrt(std::pow(x, 2.0) + std::pow(y, 2.0));
	}
	TArray<FVector> getFanDirections(const FVector& tempForward, int projectileCount, float spread) {
		TArray<FVector> ret;
		for (int i = 0; i < projectileCount; i++) {
			if (spread <= EPSILON) {
				ret.Add(tempForward);
				continue;
			}
			float angle = 0;
			if (helpers::nearEq(1, projectileCount)) {
				ret.Add(tempForward);
				return ret;
			}
			else {
				float proportion = i / static_cast<float>(projectileCount - 1);
				angle = proportion * spread - spread / 2.0;
			}
			FRotator rot = FRotator(angle, 0, 0);
			ret.Add(rot.RotateVector(tempForward));
		}
		return ret;
	}
	TArray<FVector> getRandomShapeDirections(const FVector& tempForward, int projectileCount, float spread) {
		TArray<FVector> ret;
		for (int i = 0; i < projectileCount; i++) {
			float angle = FMath::FRandRange(-spread / 2.0, spread / 2.0);
			FRotator rot = FRotator(angle, 0, 0);
			ret.Add(rot.RotateVector(tempForward));
		}
		return ret;
	}

	// The ineligible target will most likely be an enemy that was bounced off of this frame
	AEnemyBase* getRandomEnemy(UObject* caller, AEnemyBase* ineligibleTarget) {
		UCombatantManager* manager;
		if (!MyGameplayStatics::getCombatantManager(caller, manager)) {
			LOGERROR("ProjectileFactory::getRandomEnemy - failed to get combatant manager");
			return nullptr;
		}
		AEnemyBase* target = manager->getRandomEnemyPtr(ineligibleTarget);
		return target;
	}
	//Gives the direction that a projectile should be fired in to lead a target
	FVector getLeadTargetTrajectory(const AActor* caller, const AEnemyBase* target, float projectileSpeed) {
		if (!IsValid(target) || !IsValid(caller)) {
			LOGERROR("getLeadTargetTrajectory - invalid parameters");
			return FVector(1, 0, 0);
		}
		float moveSpeed;
		if (!target->getMoveSpeed(moveSpeed)) {
			LOGERROR("getLeadTargetTrajectory - failed to get target move speed");
			return FVector(1, 0, 0);
		}
		const FVector targetVelocity = target->getMoveDirection() * moveSpeed;
		const FVector targetLocation = target->GetActorLocation();
		const FVector myLocation = caller->GetActorLocation();

		const float distanceToTarget = FVector::Dist(myLocation, targetLocation);
		const float timeToTarget = distanceToTarget / projectileSpeed;
		const FVector futureTargetLocation = targetLocation + targetVelocity * timeToTarget;

		FVector ret = (futureTargetLocation - myLocation).GetSafeNormal();
		if (helpers::nearEq(ret.X, 0) && helpers::nearEq(ret.Z, 0)) {
			LOGWARNING("getLeadTargetTrajectory - bullet is centered on the same point as target. This is extremely unlikely in normal play.");
			return FVector(1, 0, 0);
		}
		return ret;
	}
}
///////////////////////////////////////////////////////////////////////////////
// AProjectile

void AProjectile::initialise_AProjectile(const ProjectileInitStruct& temp)
{
	initialise_AAttackActor(temp._attack);
	_projectileConfig = temp._projectileConfig;
	_projectileAttributes = std::make_unique<ProjectileAttributes>(temp._projectileAttributes);
	_directionX = temp._directionX; 
	_directionZ = temp._directionZ; 
}

void AProjectile::BeginPlay() {
	if (!IsValid(_projectileConfig.Get()) || _projectileAttributes.get() == nullptr) {
		LOGERROR("AProjectile::BeginPlay - parameter is not valid");
		return;
	}
	Super::BeginPlay();
	_distanceTravelled = 0;
	_pierce = _projectileAttributes->_pierce.getFinal();
	_bounce = _projectileAttributes->_bounce.getFinal();
}

void AProjectile::Tick(float delta) {
	Super::Tick(delta);
	FRotator currentRotation = GetActorRotation();
	if (_distanceTravelled >= _projectileAttributes->_range.getFinal()) {
		if (_projectileConfig->_isBoomerang && _pawnRef.IsValid() && _projectileAttributes.get() != nullptr) {
			// Destroys boomerang when it reaches caster
			const float dist = unrealHelpers::getDistance(_pawnRef->GetActorLocation(), GetActorLocation());
			const float speed = _projectileAttributes->_speed.getFinal();
			if (dist < speed * delta) {
				bulletDeath();
				return;
			}
			_projectileAttributes->_range._offset += 999;
			_boomerangActive = true;
		}
		else {
			bulletDeath();
			return;
		}
	}

	FVector start = GetActorLocation();
	FVector end = start + FVector(_directionX, 0, _directionZ) * _projectileAttributes->_speed.getFinal() * delta;
	TArray<struct FHitResult> OutHits;
	if (!performSweep(start, end, OutHits))
		return;
	// returns true iff bulletDeath was called
	if (handleSweepResults(OutHits))
		return;

	FHitResult* throwaway = nullptr;
	AddActorWorldOffset((end - start), false, throwaway, ETeleportType::TeleportPhysics);
	_distanceTravelled += _projectileAttributes->_speed.getFinal() * delta;

	FRotator newRotation = FVector(_directionX, 0, _directionZ).GetSafeNormal().Rotation();
	if (!helpers::nearEq(currentRotation.Pitch, newRotation.Pitch) || !helpers::nearEq(currentRotation.Roll, newRotation.Roll) || !helpers::nearEq(currentRotation.Yaw, newRotation.Yaw))
		SetActorRotation(newRotation, ETeleportType::TeleportPhysics);

	setNewDirection();
}

bool AProjectile::performSweep(const FVector& startPos, const FVector& endPos, TArray<struct FHitResult>& OutHits) {
	FCollisionShape collisionShape;
	if (_projectileConfig->_shape == _CIRCLE) {
		collisionShape = FCollisionShape::MakeSphere(_attackAttributes->_radius.getFinal()*SPRITE_RADIUS);
	}
	else {
		LOGERROR("AProjectile::performSweep - shape not implemented");
	}
	TArray<AActor*> tempArr;
	for (auto& tempPawn : _effectedPawns) {
		if (!tempPawn.IsValid())
			continue;
		tempArr.Add(tempPawn.Get());
	}
	auto type = isFriendly() ? collisionType::friendlyAttack : collisionType::enemyAttack;
	if (!unrealHelpers::performSweep(
		this,
		type,
		startPos,
		endPos,
		collisionShape,
		OutHits,
		tempArr)) {
		LOGERROR("AProjectile::performSweep - performSweepAtPawn failed");
		return false;
	}
	return true;
}

bool AProjectile::handleSweepResults(const TArray<struct FHitResult>& hits) {
	for (const FHitResult& hit : hits) {
		AActor* hitActor = hit.GetActor();
		//hitActor is in the middle of construction or destruction
		if (!IsValid(hitActor))
			continue;
		// Prevents actor from hitting itself with bullet
		if (_pawnRef.IsValid() && hitActor == _pawnRef.Get() && !canHitInstigator())
			continue;
		ACombatant* combatantActor = Cast<ACombatant>(hitActor);
		if (!IsValid(combatantActor)) {
			LOGERROR("AProjectile::handleSweepResults - hitActor is not a combatant");
			continue;
		}
		applyEffect(combatantActor);
		return handleBouncePierce(combatantActor);
	}
	return false;
}

// returns true iff bulletDeath was called
bool AProjectile::handleBouncePierce(ACombatant* hitActor) {
	if (helpers::nearEq(_pierce, 0)) {
		if (helpers::nearEq(_bounce, 0)) {
			bulletDeath();
			return true;
		}
		else {
			// If a bullet hitting a non-enemy has bounce, it will be ignored.
			// you'll need to change this later if you want it to bounce off of trees or something.
			AEnemyBase* casted = Cast<AEnemyBase>(hitActor);
			if (!IsValid(casted)) {
				bulletDeath();
				return true;
			}
			executeBounce(casted);
			_bounce -= 1;
		}
	}
	else {
		_pierce -= 1;
	}
	return false;
}

void AProjectile::executeBounce(AEnemyBase* ineligibleTarget) {
	auto cleanup = [this, ineligibleTarget]() {
		// After bouncing, a bullet can now contact any enemy except the one it bounced from. This means that if it is overlapping with 2 enemies it will hit the next one next frame,
		// and a bullet can get "stuck" between two enemies, bouncing between them until bounce expires or one dies. There are 2 main ways to solve this "problem". One is to use small
		// enemy and bullet hitboxes, and the other is to call it a feature instead of a bug. 
		_effectedPawns.Empty();
		APawn* casted = Cast<APawn>(ineligibleTarget);
		if (IsValid(casted))
			_effectedPawns.Add(casted);
		};
	AEnemyBase* enemy = getRandomEnemy(this, ineligibleTarget);
	if (!IsValid(enemy)) {
		cleanup();
		return;
	}
	const FVector newDirection = getLeadTargetTrajectory(this, enemy, _projectileAttributes->_speed.getFinal());
	_directionX = newDirection.X;
	_directionZ = newDirection.Z;
	cleanup();
}

void AProjectile::setNewDirection() {
	FVector targetLocation(0, -1, 0);
	if (_projectileConfig->_isHoming) {
		UCombatantManager* manager;
		if (!MyGameplayStatics::getCombatantManager(this, manager)) {
			LOGERROR("AProjectile::setNewDirection - failed to get combatant manager");
			return;
		}
		_targetEnemy = manager->getNearestEnemyPtr(this);
	}
	else if (_boomerangActive) {
		if (!_pawnRef.IsValid()) {
			bulletDeath();
			return;
		}
		targetLocation = _pawnRef->GetActorLocation();
	}
	if (_targetEnemy.IsValid()) {
		targetLocation = _targetEnemy->GetActorLocation();
	}
	if (!helpers::nearEq(targetLocation.Y, -1)) {
		const FVector myLocation = GetActorLocation();
		const FVector difference = targetLocation - myLocation;
		const FVector norm = difference.GetSafeNormal();
		_directionX = norm.X;
		_directionZ = norm.Z;
	}

}
///////////////////////////////////////////////////////////////////////////////
// ProjectileFactory
ProjectileFactory::ProjectileFactory(
	ACombatant* pawn, const UProjectileTemplate* projectileTemplate) :
	AttackFactory(pawn, projectileTemplate)
{
	if (
		!IsValid(projectileTemplate) || 
		!IsValid(projectileTemplate->_projectileConfig) || 
		projectileTemplate->_levels.Num() == 0 || 
		getLevel() < 0 ||
		getLevel() > projectileTemplate->_levels.Num() - 1 || 
		!IsValid(_levels[getLevel()])
		)
		{
			LOGERROR("AProjectileFactory::initialise_AProjectileFactory - invalid parameter");
			return;
	}
	_projectileConfig = projectileTemplate->_projectileConfig;
	const auto casted = Cast<UProjectileLevel>(projectileTemplate->_levels[getLevel()]);
	if (!IsValid(casted)) {
		LOGERROR("AProjectileFactory::initialise_AProjectileFactory - attempted to initialize with incompatible level");
		return;
	}
	auto temp = std::make_shared<ProjectileAttributes>(casted->_projectileOffsets, pawn->getAttributes());
	_projectileAttributes = std::make_unique<BaseAttributeWrapper<ProjectileAttributes>>(pawn, temp);
}

ProjectileFactory::ProjectileFactory(ProjectileFactory&& other) :
	AttackFactory(std::move(other)),
	_directionX(other._directionX),
	_directionZ(other._directionZ),
	_projectileConfig(other._projectileConfig),
	_projectileAttributes(std::move(other._projectileAttributes))
{
	other._projectileAttributes = nullptr;
}

void ProjectileFactory::tick(float delta) {
	if (!_projectileAttributes) {
		LOGERROR("AProjectileFactory::tick - _projectileAttributes is not initialized");
		return;
	}
	_projectileAttributes->tick(delta, getStatusEffects());
	AttackFactory::tick(delta);
}

void ProjectileFactory::launchAttack(const FVector& forward) {
	launchAttack_internal_projectile(forward);
}

TArray<AProjectile*> ProjectileFactory::launchAttack_internal_projectile(const FVector& forward) {
	if (getStatusCount(_BLIND) > 0)
		return {};

	TArray<AProjectile*> ret;
	const FVector tempForward = getTempForward(forward);
	const TArray<FVector> projectileDirections = getProjectileDirections(tempForward);
	for (const auto& direction : projectileDirections) {
		ret.Emplace(launchSingleProjectile(direction));
		//homing or other postinit here
	}
	return ret;
}

FVector ProjectileFactory::getTempForward(const FVector& forwardVector) const {
	EProjectileTargeting target = _projectileConfig->_targeting;
	if (target == _SKILLSHOT || target == _SNIPER)
		return forwardVector;
	else if (target == _RANDOM_DIRECTION)
		return getDirection_random();
	else if (target == _RANDOM_ENEMY) {
		AEnemyBase* enemy = getRandomEnemy(_owner.Get(), nullptr);
		// Most likely there are no enemies on screen
		if (!IsValid(enemy))
			return getDirection_random();
		return getLeadTargetTrajectory(
			_owner.Get(),
			enemy,
			_projectileAttributes->getMember(&ProjectileAttributes::_speed)
		);
	}
	else {
		LOGERROR("ProjectileFactory::launchAttack - targeting type not implemented ");
		return FVector(1, 0, 0);
	}
}

FVector ProjectileFactory::getDirection_random() {
	const float angle = FMath::FRandRange(0.0f, 360.0f);
	const FRotator rot = FRotator(angle, 0, 0);
	return rot.RotateVector(FVector(1, 0, 0));
}

TArray<FVector> ProjectileFactory::getProjectileDirections(const FVector& tempForward) {
	if (_projectileAttributes.get() == nullptr) {
		LOGERROR("AProjectileFactory::getProjectileDirections - _projectileAttributes is not initialized");
		return {};
	}
	const int projectileCount = static_cast<int>(_projectileAttributes->getMemberDiscretized(&ProjectileAttributes::_projectileCount));
	const float spread = _projectileAttributes->getMember(&ProjectileAttributes::_spread);
	EAttackShape type = _projectileConfig->_attackShape;

	if (type == _FAN) {
		return getFanDirections(tempForward, projectileCount, spread);
	}
	else if (type == _RANDOM_SHAPE) {
		return getRandomShapeDirections(tempForward, projectileCount, spread);
	}
	else {
		LOGERROR("AProjectileFactory::launchAttack - attack shape not implemented");
		return {};
	}
}

AProjectile* ProjectileFactory::launchSingleProjectile(const FVector& direction) {
	_directionX = direction.X;
	_directionZ = direction.Z;
	AProjectile* projectile = nullptr;

	bool spawnSuccess = false;
	{
		if (_projectileConfig->_targeting == _SNIPER) {
			ASniperBullet* temp = nullptr;
			spawnSuccess = unrealHelpers::spawnActorOnTopOfMeDeferred<ASniperBullet>(_owner.Get(), temp);
			projectile = Cast<AProjectile>(temp);
		}
		else
			spawnSuccess = unrealHelpers::spawnActorOnTopOfMeDeferred<AProjectile>(_owner.Get(), projectile);
	}
	spawnSuccess = spawnSuccess && IsValid(projectile);
	if (!spawnSuccess) {
		LOGERROR("AProjectileFactory::projectileLaunchInternal - failed to spawn projectile");
		return nullptr;
	}

	projectile->initialise_AProjectile(getProjectileInit());
	unrealHelpers::finishDeferredSpawn<AProjectile>(_owner.Get(), projectile);
	return projectile;
}

void ProjectileFactory::finishUpgrade(const UAttackLevel* upgrade) {
	AttackFactory::finishUpgrade(upgrade);
	const auto casted = Cast<UProjectileLevel>(upgrade);
	if (!IsValid(casted)) {
		LOGERROR("AProjectileFactory::finishUpgrade - attempted to upgrade with incompatible level");
		return;
	}
	_projectileAttributes->changeBaseValues<UProjectileAttributeData>(casted->_projectileOffsets.Get());
}

///////////////////////////////////////////////////////////////////////////////
// UProjectileAttributeData
//void UProjectileAttributeData::replaceOverrides() {
//	if (helpers::isInvalidData(_spread))
//		_spread = _defaults._spread;
//	if (helpers::isInvalidData(_speed))
//		_speed = _defaults._speed;
//	if (helpers::isInvalidData(_range))
//		_range = _defaults._range;
//	if (helpers::isInvalidData(_pierce))
//		_pierce = _defaults._pierce;
//	if (helpers::isInvalidData(_bounce))
//		_bounce = _defaults._bounce;
//	if (helpers::isInvalidData(_projectileCount))
//		_projectileCount = _defaults._projectileCount;
//}
void UProjectileAttributeData::replaceOverrides() {
	for (const auto& [memberPtr, defaultVal] : DefaultProxy<UProjectileAttributeData>::get())
		BASEATTRIBUTES_OVERRIDE(memberPtr, defaultVal);
}
///////////////////////////////////////////////////////////////////////////////
// ProjectileAttributes
ProjectileAttributes::ProjectileAttributes(const ProjectileAttributes& other) :
	BaseAttributes(other),
	//_spread(other._spread),
	//_speed(other._speed),
	//_range(other._range),
	//_pierce(other._pierce),
	//_bounce(other._bounce),
	//_projectileCount(other._projectileCount),
	_attrRef(other._attrRef)
{
	baseInit(other);
}
ProjectileAttributes::ProjectileAttributes(ProjectileAttributes&& other) :
	BaseAttributes(std::move(other)),
	//_spread(std::move(other._spread)),
	//_speed(std::move(other._speed)),
	//_range(std::move(other._range)),
	//_pierce(std::move(other._pierce)),
	//_bounce(std::move(other._bounce)),
	//_projectileCount(std::move(other._projectileCount)),
	_attrRef(other._attrRef)
{
	baseInit(std::move(other));
	other._attrRef.reset();
}

ProjectileAttributes::ProjectileAttributes(const UProjectileAttributeData* attr, std::shared_ptr<const CombatantAttributes> attrRef) :
	BaseAttributes(attr),
	_attrRef(attrRef)
{
	baseInit(attr);
}

void ProjectileAttributes::tick(UObject* context, float delta, const TArray<FEffectStruct>& statusEffects) {
	auto temp = _attrRef.lock();
	if (temp.get() == nullptr)
		return;
	softReset();
	modifyAttributes(temp);
	tick_internal(context, delta, statusEffects);
}

//void ProjectileAttributes::applyToAllStats(const std::function<void(Stat&)>& func) {
//	func(_spread);
//	func(_speed);
//	func(_range);
//	func(_pierce);
//	func(_bounce);
//	func(_projectileCount);
//}

void ProjectileAttributes::discretizeFull() {
	_pierce.discretize();
	_bounce.discretize();
	_projectileCount.discretize();
}

void ProjectileAttributes::modifyAttributes(const std::shared_ptr<const CombatantAttributes>& attr) {
	if (attr.get() == nullptr)
		return;
	_speed.modify(_speed.getBase() * attr->_projectileSpeed.getFinal() * _PROJECTILE_SPEED);
	_pierce.modify(_pierce.getBase() + attr->_bonusPierce.getFinal());
	_bounce.modify(_bounce.getBase() + attr->_bonusBounces.getFinal());
	_projectileCount.modify(_projectileCount.getBase() + attr->_bonusProjectiles.getFinal());
	_range.modify(_range.getBase() * _PROJECTILE_RANGE);
}
///////////////////////////////////////////////////////////////////////////////
// ProjectileConfig
void UProjectileConfig::replaceOverrides() {
	if (unrealHelpers::isInvalidData(_shape))
		_shape = _defaults._shape;
	if (unrealHelpers::isInvalidData(_attackShape))
		_attackShape = _defaults._attackShape;
	if (unrealHelpers::isInvalidData(_targeting))
		_targeting = _defaults._targeting;
}
///////////////////////////////////////////////////////////////////////////////
// UProjectileTemplate
std::unique_ptr<BaseAttackFactory> UProjectileTemplate::createFactory(ACombatant* owner) const {
	return std::make_unique<ProjectileFactory>(owner, this);
}