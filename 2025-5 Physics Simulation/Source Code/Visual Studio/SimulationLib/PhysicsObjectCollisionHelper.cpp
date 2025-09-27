#include "staticPch.h"
#include "PhysicsObjectCollisionHelper.h"
#include "PhysicsObject.h"

void PhysicsObjectCollisionHelper::setLinearVelocity(PhysicsObject& phys, const DirectX::XMVECTOR& newVel) {
	phys.setLinearVelocity(newVel);
}