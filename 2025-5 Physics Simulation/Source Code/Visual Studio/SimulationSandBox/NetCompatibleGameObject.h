#pragma once

#include <vector>
#include "DirectXMath.h"
#ifndef NOT_DISTRIBUTED
#include "GameObject_generated.h"
#endif

namespace helpers {
	enum peerColor;
	enum staticPtrPreset;
	struct SimpleVertex;
}
namespace shapeHelpers {
	enum ObjType;
}
class GameObject;
// Is NOT deleted every network loop. Only deleted when a scenario is reset.
class NetCompatibleBuffers {
	CComPtr<ID3D11Buffer> _vertexBuffer = nullptr;
	CComPtr<ID3D11Buffer> _indexBuffer = nullptr;
public:
	NetCompatibleBuffers(const GameObject& other);

	CComPtr<ID3D11Buffer> getVertexBuffer() const { return _vertexBuffer; }
	CComPtr<ID3D11Buffer> getIndexBuffer() const { return _indexBuffer; }
#ifndef NOT_DISTRIBUTED
	std::vector<helpers::SimpleVertex> _vertices = {};
	std::vector<WORD> _indices = {};

	NetCompatibleBuffers(const net::BufferData* lists, CComPtr<ID3D11Device>& device);
	NetCompatibleBuffers();

	const std::vector<helpers::SimpleVertex>& getVertices() const { return _vertices; }
	const std::vector<WORD>& getIndices() const { return _indices; }
#endif
};

// Is deleted and resent every network/physics loop
class NetCompatibleGameObject final {
	int _UID = -1;
	shapeHelpers::ObjType _type;
	DirectX::XMMATRIX _orientation = DirectX::XMMATRIX();
	helpers::staticPtrPreset _ptrType;
	int _indexCount = -1;
	helpers::peerColor _color;

	// Reference to data that does not get deleted
	std::shared_ptr<NetCompatibleBuffers> _objBuffers = nullptr;

#ifndef NOT_DISTRIBUTED
	int convertUID(const net::GameObject* other) const;
	shapeHelpers::ObjType convertType(const net::GameObject* other) const;
	DirectX::XMMATRIX convertOrientation(const net::GameObject* object) const;
	helpers::staticPtrPreset convertPtrType(const net::GameObject* object) const;
	int convertIndexCount(const net::GameObject* object) const;
	helpers::peerColor convertColor(const net::GameObject* object) const;
#endif
public:
	NetCompatibleGameObject() = default;
	NetCompatibleGameObject(const GameObject& obj, int UID, std::shared_ptr<NetCompatibleBuffers> buffers);
	
	// Internal (to this computer) use
	DirectX::XMMATRIX getOrientation() const { return _orientation; }
	helpers::staticPtrPreset getPtrType() const { return _ptrType; }
	int getIndexCount() const { return _indexCount; }
	helpers::peerColor getColor() const { return _color; }
	CComPtr<ID3D11Buffer> getVertexBuffer() const;
	CComPtr<ID3D11Buffer> getIndexBuffer() const;
	int getID() const { return _UID; }
	shapeHelpers::ObjType getType() const { return _type; }

#ifndef NOT_DISTRIBUTED
	NetCompatibleGameObject(const net::GameObject* obj, std::shared_ptr<NetCompatibleBuffers> buffers);
	void update(const net::GameObject* obj);
	// Prepare for serialization
	net::Matrix4x4 getMatrix4x4() const;
	std::vector<helpers::SimpleVertex> getVertexList() const { return _objBuffers->getVertices(); }
	std::vector<WORD> getIndexList() const { return _objBuffers->getIndices(); }
#endif
};

struct GlobalObjectStruct {
	// Physics sets this to true when creating an object.
	// Network reads it, acts on it, then sets it to false.
	// Network cannot set it to true (not enforced)
	bool _newItem = false;
	NetCompatibleGameObject _object;
};