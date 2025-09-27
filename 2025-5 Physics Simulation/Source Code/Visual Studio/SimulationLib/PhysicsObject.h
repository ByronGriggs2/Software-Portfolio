#pragma once

#include <memory>
#include <DirectXMath.h>
// Not best practice but I'm lazy
#include "shapeHelpers.h"

//forward declaration
class Capsule;
class HollowCube;
class Plane;
class Sphere;
class PhysicsObjectTestHelper;
class PhysicsObjectCollisionHelper;

class PhysicsObject
{
	///////////////////////////////////////
	// Member Variables
private:
	DirectX::XMVECTOR _position = DirectX::XMVectorZero();
	DirectX::XMVECTOR _rotation = DirectX::XMQuaternionIdentity();
	// _prevPosition and _prevRotation are needed to check intersection with the hollow cube.
	DirectX::XMVECTOR _prevPosition = DirectX::XMVectorZero();
	DirectX::XMVECTOR _prevRotation = DirectX::XMVectorZero();
	DirectX::XMVECTOR _linearVelocity = DirectX::XMVectorZero();
	// vector in direction of rotation axis
	DirectX::XMVECTOR _angularVelocity = DirectX::XMVectorZero();
	DirectX::XMVECTOR _frameForce = DirectX::XMVectorZero();
	// If the torque were pointing directly at a viewer, the object would rotate counterclockwise from that perspective.
	DirectX::XMVECTOR _frameTorque = DirectX::XMVectorZero();

	float _mass = 1.0f;
	float _inverseMass = 1.0f;
	float _elasticity = 1.0f;
	//bool _collidedThisFrame = false;

protected:
	bool _fixed = false;
	DirectX::XMMATRIX _inertia = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX _inverseInertia = DirectX::XMMatrixIdentity();
	bool _simpleInertia = false;
	shapeHelpers::ObjType _myType = shapeHelpers::physObj;

	/////////////////////////////////////
	// Construction
public:
	// These are member variables that should only be directly modified during construction. You don't want the mass of an object changing between frames.
	struct constructParams {
		DirectX::XMVECTOR _position = DirectX::XMVectorZero();
		DirectX::XMVECTOR _linearVelocity = DirectX::XMVectorZero();
		DirectX::XMVECTOR _angularVelocity = DirectX::XMVectorZero();
		DirectX::XMVECTOR _rotation = DirectX::XMQuaternionIdentity();
		float _mass = 1.0f;
		float _elasticity = 1.0f;
		bool _fixed = false;
	};
	PhysicsObject(const constructParams& params) : 
		_position(params._position), 
		_prevPosition(params._position), 
		_linearVelocity(params._linearVelocity), 
		_angularVelocity(params._angularVelocity),
		_rotation(params._rotation), 
		_prevRotation(params._rotation),
		_mass(params._mass), 
		_inverseMass(1.0f / params._mass), 
		_elasticity(params._elasticity), 
		_fixed(params._fixed) {
	}
	PhysicsObject() = default;
	virtual ~PhysicsObject() = default;

	////////////////////////////////////
	// Private Methods
private:
	// For internal use and testing
	friend class PhysicsObjectTestHelper;
	void kinematics(float timeStep);
	void addRotation(const DirectX::XMVECTOR& displacementVector);
	void addRotationQuat(const DirectX::XMVECTOR& quat);
	void resolveForceAndTorque(float deltaT);
	void setDirection(const DirectX::XMVECTOR& direction) { _prevRotation = _rotation; _rotation = shapeHelpers::directionToQuaternion(direction); }
	void addTorque(const DirectX::XMVECTOR& torque);
	void setAngularVelocity(const DirectX::XMVECTOR& velocity) {
		if (!_fixed)
			_angularVelocity = velocity;
	}
	void setLinearVelocity(const DirectX::XMVECTOR& velocity) {
		if (!_fixed)
			_linearVelocity = velocity;
	}
	// For internal use and collision
	friend class PhysicsObjectCollisionHelper;

public:
	void addLinearForce(const DirectX::XMVECTOR& force);
	void addForce(const DirectX::XMVECTOR& force, const DirectX::XMVECTOR& point);
	void addGravity();
	void updateEuler(float deltaT) {
		kinematics(deltaT);
		resolveForceAndTorque(deltaT);
	}
	void updateSemiEuler(float deltaT) {
		resolveForceAndTorque(deltaT);
		kinematics(deltaT);
	}
	void updateRungeKutta(float deltaT);
	void pushTowardsOrigin(float distance);

	const DirectX::XMVECTOR& getPosition() const { return _position; }
	const DirectX::XMVECTOR& getPrevPosition() const { return _prevPosition; }
	DirectX::XMVECTOR getDirection() const { return XMVector3TransformNormal(DirectX::XMVectorSet(0, 1, 0, 0), DirectX::XMMatrixRotationQuaternion(_rotation)); }
	DirectX::XMVECTOR getPrevDirection() const { return XMVector3TransformNormal(DirectX::XMVectorSet(0, 1, 0, 0), DirectX::XMMatrixRotationQuaternion(_prevRotation)); }
	const DirectX::XMVECTOR& getRotation() const { return _rotation; }
	DirectX::XMMATRIX getOrientation() const;
	const DirectX::XMVECTOR& getLinearVelocity() const { return _linearVelocity; }
	const DirectX::XMVECTOR& getAngularVelocity() const { return _angularVelocity; }
	float getMomentum() const { return _mass * DirectX::XMVectorGetX(DirectX::XMVector3Length(_linearVelocity)); }
	float getEnergy(float potentialEnergyReference) {
		const float PE = _mass * abs(GRAVITY) * abs((DirectX::XMVectorGetY(_position) - potentialEnergyReference));
		const float KE = (1.0f / 2.0f) * _mass * pow(DirectX::XMVectorGetX(DirectX::XMVector3Length(_linearVelocity)), 2.0f);
		return PE + KE;
	}
	bool isFixed() const { return _fixed; }
	float getElasticity() const { return _elasticity; }
	shapeHelpers::ObjType getType() const { return _myType; }
	float getMass() const { return _mass; }

	virtual bool testIntersect(const PhysicsObject* other) const;
	virtual bool testIntersect(const Capsule* other) const;
	virtual bool testIntersect(const HollowCube* other) const;
	virtual bool testIntersect(const Plane* other) const;
	virtual bool testIntersect(const Sphere* other) const;
	virtual std::unique_ptr<PhysicsObject> clone() const;
};