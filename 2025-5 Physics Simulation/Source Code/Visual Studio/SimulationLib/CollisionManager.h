#pragma once

#include <DirectXMath.h>

class PhysicsObject;

/////////////////////
// Static class

class CollisionManager final
{
	static DirectX::XMVECTOR getCollisionDirection(PhysicsObject&, PhysicsObject&);
	static DirectX::XMVECTOR colDirCapsuleSphere(PhysicsObject*, const PhysicsObject*);
	static DirectX::XMVECTOR colDirSphereSphere(const PhysicsObject* cap, const PhysicsObject* sph);
	static DirectX::XMVECTOR colDirSpherePlane(const PhysicsObject* sphere, PhysicsObject* plane);
	static DirectX::XMVECTOR colDirSphereHollowCube(const PhysicsObject* sphere, PhysicsObject* plane);

public:
	CollisionManager() = delete;
	CollisionManager(const CollisionManager&) = delete;
	CollisionManager& operator=(const CollisionManager&) = delete;
	CollisionManager& operator=(const CollisionManager&&) = delete;
	CollisionManager(const CollisionManager&&) = delete;

	//ignores mass, works with all collidable shapes
	static void collisionSimplified(PhysicsObject&, PhysicsObject&, float extraFriction);
	// Uses mass, only works with spheres
	static void sphereMassCollision(PhysicsObject&, PhysicsObject&, float extraFriction);
};

