#include "pch.h"
#include "NetCompatibleGameObject.h"
#ifndef NOT_DISTRIBUTED
#include "flatbuffers/flatbuffers.h"
#endif
#include "GameObject.h"

using namespace DirectX;
using namespace helpers;

NetCompatibleBuffers::NetCompatibleBuffers(const GameObject& other) : 
	_vertexBuffer(other.getVertexBuffer()), 
	_indexBuffer(other.getIndexBuffer()) 
#ifndef NOT_DISTRIBUTED
	,_vertices(other.getVertexList()), 
	_indices(other.getIndexList()) 
#endif
{}

NetCompatibleGameObject::NetCompatibleGameObject(const GameObject& obj, int UID, std::shared_ptr<NetCompatibleBuffers> buffers) :
	_UID(UID), //unused?
	_type(obj.getType()),
	_orientation(obj.getOrientation()),
	_ptrType(obj.getPtrPreset()),
	_indexCount(obj.getIndexCount()),
	_color(obj.getColor()),
	_objBuffers(buffers)
{}

CComPtr<ID3D11Buffer> NetCompatibleGameObject::getVertexBuffer() const {
#ifdef _DEBUG
	if (_objBuffers == nullptr) {
		std::cout << "_sharedBuffers == nullptr" << std::endl;
		throw ShouldNotRunException();
	}
#endif
	return _objBuffers->getVertexBuffer();
}
CComPtr<ID3D11Buffer> NetCompatibleGameObject::getIndexBuffer() const {
#ifdef _DEBUG
	if (_objBuffers == nullptr) {
		std::cout << "_sharedBuffers == nullptr" << std::endl;
		throw ShouldNotRunException();
	}
#endif
	return _objBuffers->getIndexBuffer();
}

#ifndef NOT_DISTRIBUTED
NetCompatibleGameObject::NetCompatibleGameObject(const net::GameObject* obj, std::shared_ptr<NetCompatibleBuffers> buffers) :
	_UID(convertUID(obj)),
	_type(convertType(obj)),
	_objBuffers(buffers),
	_orientation(convertOrientation(obj)),
	_ptrType(convertPtrType(obj)),
	_indexCount(convertIndexCount(obj)),
	_color(convertColor(obj))
{
}

NetCompatibleBuffers::NetCompatibleBuffers(const net::BufferData* lists, CComPtr<ID3D11Device>& device) {
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	D3D11_SUBRESOURCE_DATA InitData = {};

	std::vector<SimpleVertex> myVertices;
	for (const auto& vertex : *lists->vertex_data()) {
		SimpleVertex temp;
		temp.Pos = XMVectorSet(vertex->pos()->Get(0), vertex->pos()->Get(1), vertex->pos()->Get(2), 0);
		temp.Norm = XMVectorSet(vertex->norm()->Get(0), vertex->norm()->Get(1), vertex->norm()->Get(2), 0);
		temp.Tex = XMVectorSet(vertex->tex()->Get(0), vertex->tex()->Get(1), 0, 0);
		myVertices.push_back(temp);
	}
	std::vector<WORD> myIndices;
	for (const auto& index : *lists->index_data()) {
		myIndices.push_back(static_cast<WORD>(index));
	}
	bd.ByteWidth = static_cast<UINT>(myVertices.size()) * sizeof(SimpleVertex);
	InitData.pSysMem = &myVertices[0];
	device->CreateBuffer(&bd, &InitData, &_vertexBuffer);

	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.ByteWidth = static_cast<UINT>(myIndices.size()) * sizeof(WORD);
	InitData.pSysMem = &myIndices[0];
	device->CreateBuffer(&bd, &InitData, &_indexBuffer);
}

NetCompatibleBuffers::NetCompatibleBuffers() {
#ifdef _DEBUG 
	throw ShouldNotRunException();
#endif 
}

void NetCompatibleGameObject::update(const net::GameObject* obj) {
	_orientation = convertOrientation(obj);
	_ptrType = convertPtrType(obj);
	_indexCount = convertIndexCount(obj);
	_color = convertColor(obj);
}

int NetCompatibleGameObject::convertUID(const net::GameObject* object) const {
	return object->type();
}

shapeHelpers::ObjType NetCompatibleGameObject::convertType(const net::GameObject* object) const {
	return static_cast<shapeHelpers::ObjType>(object->type());
}

XMMATRIX NetCompatibleGameObject::convertOrientation(const net::GameObject* object) const {
	return DirectX::XMMATRIX(
		object->orientation()->m()->Get(0), object->orientation()->m()->Get(1), object->orientation()->m()->Get(2), object->orientation()->m()->Get(3),
		object->orientation()->m()->Get(4), object->orientation()->m()->Get(5), object->orientation()->m()->Get(6), object->orientation()->m()->Get(7),
		object->orientation()->m()->Get(8), object->orientation()->m()->Get(9), object->orientation()->m()->Get(10), object->orientation()->m()->Get(11),
		object->orientation()->m()->Get(12), object->orientation()->m()->Get(13), object->orientation()->m()->Get(14), object->orientation()->m()->Get(15)
	);
}

staticPtrPreset NetCompatibleGameObject::convertPtrType(const net::GameObject* object) const {
	return static_cast<staticPtrPreset>(object->ptr_type());
}

int NetCompatibleGameObject::convertIndexCount(const net::GameObject* object) const {
	return object->index_count();
}

peerColor NetCompatibleGameObject::convertColor(const net::GameObject* object) const {
	return static_cast<peerColor>(object->color());
}

net::Matrix4x4 NetCompatibleGameObject::getMatrix4x4() const {
	float matrixData[16] = {
		_orientation.r[0].m128_f32[0], _orientation.r[0].m128_f32[1], _orientation.r[0].m128_f32[2], _orientation.r[0].m128_f32[3],
		_orientation.r[1].m128_f32[0], _orientation.r[1].m128_f32[1], _orientation.r[1].m128_f32[2], _orientation.r[1].m128_f32[3],
		_orientation.r[2].m128_f32[0], _orientation.r[2].m128_f32[1], _orientation.r[2].m128_f32[2], _orientation.r[2].m128_f32[3],
		_orientation.r[3].m128_f32[0], _orientation.r[3].m128_f32[1], _orientation.r[3].m128_f32[2], _orientation.r[3].m128_f32[3]
	};
	return net::Matrix4x4(matrixData);
}
#endif