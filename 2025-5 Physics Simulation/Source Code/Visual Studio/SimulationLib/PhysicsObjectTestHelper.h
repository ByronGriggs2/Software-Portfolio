#pragma once

#include <DirectXMath.h>
class PhysicsObject;

class PhysicsObjectTestHelper final {
public:
	PhysicsObjectTestHelper() = delete;
	PhysicsObjectTestHelper(const PhysicsObjectTestHelper&) = delete;
	PhysicsObjectTestHelper& operator=(const PhysicsObjectTestHelper&) = delete;
	PhysicsObjectTestHelper& operator=(const PhysicsObjectTestHelper&&) = delete;
	PhysicsObjectTestHelper(const PhysicsObjectTestHelper&&) = delete;

	static void addRotation(PhysicsObject& phys, const DirectX::XMVECTOR& displacementVector);
	static void addRotationQuat(PhysicsObject& phys, const DirectX::XMVECTOR& rotationQuaternion);
	static void addTorque(PhysicsObject& phys, const DirectX::XMVECTOR& torqueVector);
	static void addLinearForce(PhysicsObject& phys, const DirectX::XMVECTOR& force);
	static void kinematics(PhysicsObject& phys, float deltaT);
	static void setAngularVelocity(PhysicsObject& phys, const DirectX::XMVECTOR& angularVelocity);
	static void setLinearVelocity(PhysicsObject& phys, const DirectX::XMVECTOR& linearVelocity);
	static void setDirection(PhysicsObject& phys, const DirectX::XMVECTOR& direction);
	static void resolveForceAndTorque(PhysicsObject& phys, float deltaT);
};