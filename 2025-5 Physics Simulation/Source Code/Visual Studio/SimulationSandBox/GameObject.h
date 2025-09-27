#pragma once

#include <memory>
#include "DirectXMath.h"
#include "ObjectGeometry.h"
#include "PhysicsObject.h"
#include "d3d11.h"
#include <atlbase.h>

//#define DEBUGVERTEXBUFFER

namespace helpers {
	struct ObjectPresets;
	enum peerColor;
	enum staticPtrPreset;
}

class GameObject {
	ObjectGeometry _geometry;
	std::unique_ptr<PhysicsObject> _physics;
	helpers::staticPtrPreset _ptrPreset;
	helpers::peerColor _myColor;
	CComPtr<ID3D11Buffer> _vertexBuffer;
	CComPtr<ID3D11Buffer> _indexBuffer;

public:
	// Constructor with specified primitive
	GameObject(const helpers::ObjectPresets& preset);
	// Gang of 5
	GameObject() = delete;
	GameObject(const GameObject& other);
	GameObject(GameObject&& other) noexcept;
	GameObject& operator=(const GameObject& other);
	GameObject& operator=(GameObject&& other) noexcept;

	bool testIntersect(const GameObject& other) const;
	void updateEuler(float deltaT);
	void updateSemiEuler(float deltaT);
	void updateRungeKutta(float deltaT);
	void setBuffers(const CComPtr<ID3D11Device>&);
	void addLinearForce(const DirectX::XMVECTOR& force);
	void pushTowardsOrigin(float distanceToPush);
	static void addCollision(GameObject&, GameObject&, float extraFriction);

	void setVertices(const std::vector<helpers::SimpleVertex>& v);
	void setIndices(const std::vector<WORD>& i);
	void setPtrs(helpers::staticPtrPreset preset) { _ptrPreset = preset; }
	void setColor(helpers::peerColor color) { _myColor = color; }

	int getIndexCount() const;
	helpers::staticPtrPreset getPtrPreset() const { return _ptrPreset; }
	shapeHelpers::ObjType getType() const;
	DirectX::XMMATRIX getOrientation() const;
	const DirectX::XMVECTOR& getPosition() const;
	bool isFixed() const;
	float getSphereRadius();
	float getMomentum() const;
	float getEnergy(float potentialEnergyReference) const;
	helpers::peerColor getColor() const { return _myColor; }

	CComPtr<ID3D11Buffer> getVertexBuffer() const { return _vertexBuffer; }
	std::vector<helpers::SimpleVertex> getVertexList() const { return _geometry.getVertices(); }
	CComPtr<ID3D11Buffer> getIndexBuffer() const { return _indexBuffer; }
	std::vector<WORD> getIndexList() const { return _geometry.getIndices(); }
};