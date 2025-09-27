#include "staticPch.h"
#include "CollisionManager.h"
#include "Capsule.h"
#include "Sphere.h"
#include "HollowCube.h"
#include "Plane.h"
#include "PhysicsObjectCollisionHelper.h"

using namespace DirectX;
using namespace shapeHelpers;

//Assumes mass of both objects is 1.
void CollisionManager::collisionSimplified(PhysicsObject& A, PhysicsObject& B, float extraFriction) {
	const XMVECTOR Apos = A.getPosition();
	const XMVECTOR Bpos = B.getPosition();
	const XMVECTOR Avel = A.getLinearVelocity();
	const XMVECTOR Bvel = B.getLinearVelocity();

	const XMVECTOR NA = getCollisionDirection(A, B);
	const XMVECTOR NB = -NA;
	//calculation for A
	//parallel = V.dot(N)*N
	const XMVECTOR parallelComponentA = XMVector3Dot(NA, Avel) * NA;
	//perpendicular component
	const XMVECTOR perpendicularComponentA = Avel - parallelComponentA;

	//calculation for B
	const XMVECTOR parallelComponentB = XMVector3Dot(NB, Bvel) * NB;
	const XMVECTOR perpendicularComponentB = Bvel - parallelComponentB;

	// Simplified elasticity model
	const float restitution = A.getElasticity() * B.getElasticity();
	
	if (A.isFixed()) {
		PhysicsObjectCollisionHelper::setLinearVelocity(B, restitution * -parallelComponentB + perpendicularComponentB);
	}
	else if (B.isFixed()) {
		PhysicsObjectCollisionHelper::setLinearVelocity(A, restitution * -parallelComponentA + perpendicularComponentA);
	}
	else {
		const XMVECTOR newAVel = (restitution * parallelComponentB + perpendicularComponentA) * (1 - extraFriction);
		const XMVECTOR newBVel = restitution * parallelComponentA + perpendicularComponentB * (1 - extraFriction);
		PhysicsObjectCollisionHelper::setLinearVelocity(A, newAVel);
		PhysicsObjectCollisionHelper::setLinearVelocity(B, newBVel);
	}
}

//could definitely be simplified but it's very clear what's happening and 
//that's good
void CollisionManager::sphereMassCollision(PhysicsObject& p1, PhysicsObject& p2, float extraFriction) {
	if (p1.getType() != sphere || p2.getType() != sphere) {
#ifdef _DEBUG
		throw ShouldNotRunException();
#endif
	}
	if (p1.isFixed() || p2.isFixed()) {
		collisionSimplified(p1, p2, extraFriction);
		return;
	}

	const float p1Mass = p1.getMass();
	const float p2Mass = p2.getMass();
	const XMVECTOR p1Vel = p1.getLinearVelocity();
	const XMVECTOR p2Vel = p2.getLinearVelocity();
	const XMVECTOR NA = colDirSphereSphere(&p1, &p2);
	const XMVECTOR NB = -NA;

	const XMVECTOR parallelComponentp1 = XMVector3Dot(NA, p1Vel) * NA;
	const XMVECTOR perpendicularComponentp1 = p1Vel - parallelComponentp1;

	const XMVECTOR parallelComponentp2 = XMVector3Dot(NB, p2Vel) * NB;
	const XMVECTOR perpendicularComponentp2 = p2Vel - parallelComponentp2;

	const float restitution = p1.getElasticity() * p2.getElasticity();

	const XMVECTOR termA1 = restitution * p2Mass * (parallelComponentp2 - parallelComponentp1);
	const XMVECTOR termB = p1Mass * parallelComponentp1;
	const XMVECTOR termC = p2Mass * parallelComponentp2;
	const float denominator = p1Mass + p2Mass;
	const XMVECTOR termA2 = restitution * p1Mass * (parallelComponentp1 - parallelComponentp2);

	const XMVECTOR newParallelp1 = (termA1 + termB + termC) / denominator;
	const XMVECTOR newParallelp2 = (termA2 + termB + termC) / denominator;
	
	PhysicsObjectCollisionHelper::setLinearVelocity(p1, (newParallelp1 + perpendicularComponentp1)* (1 - extraFriction));
	PhysicsObjectCollisionHelper::setLinearVelocity(p2, (newParallelp2 + perpendicularComponentp2)* (1 - extraFriction));
}

XMVECTOR CollisionManager::getCollisionDirection(PhysicsObject& A, PhysicsObject& B) {
	switch (A.getType()) {
	case capsule:
		switch (B.getType()) {
		case sphere:
			return colDirCapsuleSphere(&A, &B);
			break;
		default:
			return XMVectorZero();
		}
		break;
	case sphere:
		switch (B.getType()) {
		case capsule:
			return -1.0f * colDirCapsuleSphere(&B, &A);
		case sphere:
			return colDirSphereSphere(&A, &B);
			break;
		case plane:
			return colDirSpherePlane(&A, &B);
			break;
		case hollowCube:
			if (!B.isFixed())
				return XMVectorZero();
			return colDirSphereHollowCube(&A, &B);
			break;
		default:
			return XMVectorZero();
			break;
		}
		break;
	case plane:
		switch (B.getType()) {
		case sphere:
			return -1.0f * colDirSpherePlane(&B, &A);
			break;
		default:
			return XMVectorZero();
			break;
		}
		break;
	case hollowCube:
		if (!A.isFixed()) {
			return XMVectorZero();
		}
		switch (B.getType()) {
		case sphere:
			return -1.0f * colDirSphereHollowCube(&B, &A);
			break;
		default:
			return XMVectorZero();
			break;
		}
		break;
	default:
		return XMVectorZero();
		break;
	}
	return XMVectorZero();
}

XMVECTOR CollisionManager::colDirCapsuleSphere(PhysicsObject* cap, const PhysicsObject* sph) {
	const XMVECTOR spherePos = sph->getPosition();
	const Capsule* tempCap = dynamic_cast<Capsule*>(cap);
	const LineSegment capAxis = tempCap->getAxis();
	const XMVECTOR closestPoint = capAxis.closestPointOnLineSegment(spherePos);
	return XMVector3Normalize(closestPoint - spherePos);
}
XMVECTOR CollisionManager::colDirSphereSphere(const PhysicsObject* A, const PhysicsObject* B) {
	return XMVector3Normalize(B->getPosition() - A->getPosition());
}
XMVECTOR CollisionManager::colDirSpherePlane(const PhysicsObject* sphere, PhysicsObject* plane) {
	const XMVECTOR spherePos = sphere->getPosition();
	const Plane* tempPlane = dynamic_cast<Plane*>(plane);
	const XMVECTOR p1 = tempPlane->closestPointOnPlane(spherePos);
	return XMVector3Normalize(p1 - spherePos);
}
XMVECTOR CollisionManager::colDirSphereHollowCube(const PhysicsObject* sphere, PhysicsObject* cube) {
	// redoes calculations that have been thrown away :(
	const HollowCube* tempCube = dynamic_cast<HollowCube*>(cube);
	float throwaway;
	Plane collisionFace = tempCube->closestPlane(sphere->getPosition(), throwaway);
	return colDirSpherePlane(sphere, &collisionFace);
}