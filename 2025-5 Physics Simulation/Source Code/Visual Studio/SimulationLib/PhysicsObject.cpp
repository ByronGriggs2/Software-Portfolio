#include "staticPch.h"
#include "PhysicsObject.h"

using namespace shapeHelpers;
using namespace DirectX;

void PhysicsObject::addTorque(const DirectX::XMVECTOR& torque) { _frameTorque += torque; }
void PhysicsObject::addLinearForce(const DirectX::XMVECTOR& force) { _frameForce += force; }
void PhysicsObject::addForce(const DirectX::XMVECTOR& force, const DirectX::XMVECTOR& point) {
	addLinearForce(force);
	const DirectX::XMVECTOR directionToCenter = _position - point;
	const DirectX::XMVECTOR torqueForce = DirectX::XMVector3Cross(directionToCenter, force);
	addTorque(torqueForce);
}
void PhysicsObject::addGravity() {
	const DirectX::XMMATRIX convertToLocal = DirectX::XMMatrixRotationQuaternion(_rotation);
	DirectX::XMVECTOR forceVec = DirectX::XMVectorSet(0, GRAVITY, 0, 0) * _mass;
	forceVec = XMVector3Transform(forceVec, convertToLocal);
	addLinearForce(forceVec);
}
void PhysicsObject::pushTowardsOrigin(float distance) { _position -= XMVector3Normalize(_position) * distance; _prevPosition = _position; }

XMMATRIX PhysicsObject::getOrientation() const { return DirectX::XMMatrixRotationQuaternion(_rotation) * XMMatrixTranslationFromVector(_position); }

void PhysicsObject::kinematics(float timeStep) {
	// Catches inheritors that don't initialize correctly
#ifdef _DEBUG
	if (vec3Equal(_rotation, XMVectorZero()) && abs(_rotation.m128_f32[3]) <= EPSILON)
		throw NotImplementedException();
#endif
	if (_fixed)
		return;

	if (!vec3Equal(_angularVelocity, XMVectorZero())) {
		const XMVECTOR deltaAngle = _angularVelocity * timeStep;
		addRotation(deltaAngle);
	}

	_prevPosition = _position;
	_position = _position + _linearVelocity * static_cast<float>(timeStep);
}

// _angularVelocity is a vector in the direction of the axis of rotation, with magnitude equal to the angular velocity in radians per second.
// This converts the angular velocity to angular diplacement in the form of a quaternion
void PhysicsObject::addRotation(const XMVECTOR& displacementVector) {
	_prevRotation = _rotation;
	const XMVECTOR axis = XMVector3Normalize(displacementVector);
	// XMQuaternionRotationNormal expects global coordinates, so we have to convert the local rotation to global.
	// We do this by rotating the rotation axis by the same rotation matrix the object's direction vector has been multiplied by.
	const XMMATRIX transformToGlobal = XMMatrixRotationQuaternion(_rotation);
	const XMVECTOR globalAxis = XMVector3TransformNormal(axis, transformToGlobal);
	// Torque uses right hand rule, DirectX11 rotation uses left hand rule
	const float magnitude = -1.0f * XMVectorGetX(XMVector3Length(displacementVector));
	const XMVECTOR deltaQuat = XMQuaternionRotationNormal(globalAxis, magnitude);
	_rotation = XMQuaternionMultiply(_rotation, deltaQuat);
	_rotation = XMQuaternionNormalize(_rotation);
}

void PhysicsObject::addRotationQuat(const XMVECTOR& rotationQuaternion) {
	_prevRotation = _rotation;
	_rotation = XMQuaternionMultiply(_rotation, rotationQuaternion);
	_rotation = XMQuaternionNormalize(_rotation);
}

void PhysicsObject::resolveForceAndTorque(float deltaT) {
	if (_fixed)
		return;
	if (!vec3Equal(_frameTorque, XMVectorZero())) {
#ifdef _DEBUG
		// Catches inheritor with undefined inertia
		if (XMMatrixIsIdentity(_inertia) && !_simpleInertia)
			throw NotImplementedException();
#endif
		// Assumes torque is in local coordinates. Can change later easily by multiplying by the rotation matrix part of the orientation matrix
		if (_simpleInertia) {
			const XMVECTOR acceleration = _inverseInertia.r[0].m128_f32[0] * _frameTorque;
			_angularVelocity += acceleration * static_cast<float>(deltaT);
		}
		else {
			const XMVECTOR acceleration = XMVector4Transform(_frameTorque, _inverseInertia);
			_angularVelocity += acceleration * static_cast<float>(deltaT);
		}
		_frameTorque = XMVectorZero();
	}
	if (!vec3Equal(_frameForce, XMVectorZero())) {
		_linearVelocity += _frameForce * static_cast<float>(_inverseMass * deltaT);
		_frameForce = XMVectorZero();
	}
}

// Does not take into account rotation
void PhysicsObject::updateRungeKutta(float deltaT) {
	const XMVECTOR& y0 = _position;
	const float& h = deltaT;
	const XMVECTOR& v0 = _linearVelocity;
	const XMVECTOR& k1 = v0;
	const XMVECTOR k2 = v0 + (h / 2) * k1;
	const XMVECTOR k3 = v0 + (h / 2) * k2;
	const XMVECTOR k4 = v0 + h * k3;
	const XMVECTOR y1 = y0 + (h / 6) * (k1 + 2 * k2 + 2 * k3 + k4);

	_position = y1;
	resolveForceAndTorque(deltaT);
}

///////////////////////////////////////////////////
bool PhysicsObject::testIntersect(const PhysicsObject* other) const {
#ifdef _DEBUG
	throw NotImplementedException();
#endif
	return false;
}

bool PhysicsObject::testIntersect(const Capsule* other) const {
#ifdef _DEBUG
	throw NotImplementedException();
#endif
	return false;
}

bool PhysicsObject::testIntersect(const HollowCube* other) const {
#ifdef _DEBUG
	throw NotImplementedException();
#endif
	return false;
}

bool PhysicsObject::testIntersect(const Plane* other) const {
#ifdef _DEBUG
	throw NotImplementedException();
#endif
	return false;
}

bool PhysicsObject::testIntersect(const Sphere* other) const {
#ifdef _DEBUG
	throw NotImplementedException();
#endif
	return false;
}

std::unique_ptr<PhysicsObject> PhysicsObject::clone() const {
#ifdef _DEBUG
	throw NotImplementedException();
#endif
	return nullptr;
}