#pragma once

#include <DirectXMath.h>

class PhysicsObject;
class CollisionManager;

/////////////////////
// Static class

class PhysicsObjectCollisionHelper final {
	friend class CollisionManager;
	static void setLinearVelocity(PhysicsObject& phys, const DirectX::XMVECTOR& newVel);
public:
	PhysicsObjectCollisionHelper() = delete;
	PhysicsObjectCollisionHelper(const PhysicsObjectCollisionHelper&) = delete;
	PhysicsObjectCollisionHelper& operator=(const PhysicsObjectCollisionHelper&) = delete;
	PhysicsObjectCollisionHelper& operator=(const PhysicsObjectCollisionHelper&&) = delete;
	PhysicsObjectCollisionHelper(const PhysicsObjectCollisionHelper&&) = delete;
};