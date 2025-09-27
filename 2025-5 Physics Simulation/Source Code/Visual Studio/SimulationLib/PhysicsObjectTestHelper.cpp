#include "staticPch.h"
#include "PhysicsObjectTestHelper.h"
#include "PhysicsObject.h"

void PhysicsObjectTestHelper::addRotation(PhysicsObject& phys, const DirectX::XMVECTOR& displacementVector) {
	phys.addRotation(displacementVector);
}
void PhysicsObjectTestHelper::addRotationQuat(PhysicsObject& phys, const DirectX::XMVECTOR& rotationQuaternion) {
	phys.addRotationQuat(rotationQuaternion);
}
void PhysicsObjectTestHelper::addTorque(PhysicsObject& phys, const DirectX::XMVECTOR& torqueVector) {
	phys.addTorque(torqueVector);
}
void PhysicsObjectTestHelper::addLinearForce(PhysicsObject& phys, const DirectX::XMVECTOR& force) {
	phys.addLinearForce(force);
}
void PhysicsObjectTestHelper::kinematics(PhysicsObject& phys, float deltaT) {
	phys.kinematics(deltaT);
}
void PhysicsObjectTestHelper::setAngularVelocity(PhysicsObject& phys, const DirectX::XMVECTOR& angularVelocity) {
	phys.setAngularVelocity(angularVelocity);
}
void PhysicsObjectTestHelper::setLinearVelocity(PhysicsObject& phys, const DirectX::XMVECTOR& linearVelocity) {
	phys.setLinearVelocity(linearVelocity);
}
void PhysicsObjectTestHelper::setDirection(PhysicsObject& phys, const DirectX::XMVECTOR& direction) {
	phys.setDirection(direction);
}
void PhysicsObjectTestHelper::resolveForceAndTorque(PhysicsObject& phys, float deltaT) {
	phys.resolveForceAndTorque(deltaT);
}