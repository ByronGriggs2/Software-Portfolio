//An object after being loaded in from the obj file. Contains position, type, vertices/indices, vertex/index buffer, etc
//Notably the vertex and index buffers are nullptr after construction, and must be set later.

//Also, note the public data members that D3DFramework needs access to to create the buffers and draw the object
#pragma once
#include <vector>
#include <windows.h>
#include <iostream>
#include <atlbase.h>
#include <directxmath.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>

using namespace DirectX;

struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT3 Norm;
	XMFLOAT2 Tex;
	XMFLOAT3 Tangent;
	XMFLOAT3 Binormal;
};

enum class Rotation {
	null,
	cw,
	ccw
};

class CompleteObject final{
	bool _collected;
	bool _collectable;
	Rotation _rotation;
	XMFLOAT3 _position;
	XMFLOAT3 _velocity;
	//type is specified in config.txt and used to determine vertex shaders, rasterizers, etc
	std::string _type;
	std::vector<SimpleVertex> _vertices;
	std::vector<WORD> _indices;
	int _indexCount;
	CComPtr <ID3D11Buffer> _pVertexBuffer;
	CComPtr <ID3D11Buffer> _pIndexBuffer;
	XMFLOAT3 _spawnPoint;
public:
	CompleteObject() : _collected(false), _collectable(false), _rotation(Rotation::null), _position(XMFLOAT3(0, 0, 0)), _velocity(XMFLOAT3(0, 0, 0)), _type("null"),
		_vertices({}), _indices({}), _indexCount(0), _pVertexBuffer(nullptr), _pIndexBuffer(nullptr), _spawnPoint(XMFLOAT3(0,0,0)) {}
	
	CompleteObject(const XMFLOAT3& pos, const std::string& type, bool coll) : _collected(false), _collectable(coll), _rotation(Rotation::null), _position(pos), 
		_velocity(XMFLOAT3(0, 0, 0)), _type(type), _vertices({}), _indices({}), _indexCount(0), _pVertexBuffer(nullptr), _pIndexBuffer(nullptr), _spawnPoint(pos) {}

	//posInit offsets all vertices by the initial position specified in config.txt. Does not change vertex buffers, that must be done seperately
	//intended to only be called once for the initial position. Further translations are handled by CompleteObject::_position and D3DFramework::_World
	void posInit();
	void ground() { _position.y = _spawnPoint.y;}
	void respawn() {_position = _spawnPoint;}
	void updatePos();

	bool isCollectable() const { return _collectable; }
	bool isCollected() const { return _collected; }
	void collect() { _collected = true; }
	void unCollect() { _collected = false; }
	void setRotation(Rotation rot) { _rotation = rot; }
	Rotation getRotation() const { return _rotation; }
	XMFLOAT3 getPos() const {return _position;}
	void setVelocity(const XMFLOAT3& v) { _velocity = v; }
	std::string getType() const { return _type; }
	XMFLOAT3 getSpawn() const { return _spawnPoint; }

	std::vector<SimpleVertex> getVertices() const { return _vertices; }
	std::vector<WORD> getIndices() const { return _indices; }
	UINT getVertexByteWidth() const {return static_cast<UINT>(_vertices.size() * sizeof(SimpleVertex));}
	UINT getIndexByteWidth() const { return static_cast<UINT>(_indices.size() * sizeof(WORD)); }
	void setVectors(const std::vector<SimpleVertex>& vert, const std::vector<WORD>& ind) {
		_vertices = vert;
		_indices = ind;
	}
	void setTanBinorm(int i, const XMFLOAT3& tan, const XMFLOAT3& bin) {
		_vertices[i].Tangent = tan;
		_vertices[i].Binormal = bin;
	}
	int getIndexCount() const { return _indexCount; }
	void setIndexCount() { _indexCount = _indices.size(); }
	CComPtr<ID3D11Buffer> getVertexBuffer() const { return _pVertexBuffer; }
	void setVertexBuffer(const CComPtr<ID3D11Buffer>& v) { _pVertexBuffer = v; }
	CComPtr<ID3D11Buffer> getIndexBuffer() const { return _pIndexBuffer; }
	void setIndexBuffer(const CComPtr<ID3D11Buffer>& i) { _pIndexBuffer = i; }
};

