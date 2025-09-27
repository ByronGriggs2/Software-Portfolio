#include "pch.h"
#include "TCPPeer.h"
#include "TCPStreamingSocket.h"
#include "TCPListeningSocket.h"
#include "NetCompatibleGameObject.h"
#include "NetCompatibleImGuiState.h"
#ifndef NOT_DISTRIBUTED
#include "GameObject_generated.h"
#include "ImGuiState_generated.h"
#endif

using namespace helpers;
using namespace DirectX;

TCPPeer::TCPPeer() : _myColor(undef) {
	_listen = std::make_unique<TCPListeningSocket>();
}

bool TCPPeer::acceptConnectionRequestIfNotConnected() {
#ifdef _DEBUG
	if (_streams.size() >= 3)
		throw ShouldNotRunException();
#endif
	std::unique_ptr<TCPStreamingSocket> temp = _listen->myAccept();
	if (temp == nullptr)
		return false;
	const int targetIP = temp->getPeerName();
	if (hasConnection(targetIP)) {
#ifdef VERBOSE
		std::cout << "Not accepting connection request as I already have a connection" << std::endl;
#endif
		return false;
	}
	_streams.push_back(PeerConnection(undef, targetIP, std::move(temp)));
	if (_streams.size() == 3) {
		_fullyConnected = true;
	}
	return true;
}

bool TCPPeer::sendConnectionRequestIfNotConnected(const sockaddr_in& addr) {
#ifdef _DEBUG
	if (_streams.size() >= 3)
		throw ShouldNotRunException();
#endif
	if (hasConnection(addr)) {
#ifdef VERBOSE
		std::cout << "Not sending connection request as I already have a connection" << std::endl;
#endif
		return false;
	}
	std::unique_ptr<TCPStreamingSocket> tempSocket = std::make_unique<TCPStreamingSocket>();
	if (!tempSocket->connectToPeer(addr)) {
		return false;
	}
	_streams.push_back(PeerConnection(undef, tempSocket->getPeerName(), std::move(tempSocket)));
	if (_streams.size() == 3) {
		_fullyConnected = true;
	}
	return true;
}

void TCPPeer::assignColors() {
	// Add self to back of vector
	_streams.push_back(PeerConnection(_myColor, _streams[0]._socket->getMyName(), nullptr));
	// Sort vector based on IP address. ALL COMPUTERS ON NETWORK SHOULD NOW HAVE THE SAME VECTOR AFTER THIS
	std::sort(_streams.begin(), _streams.end(), [](const PeerConnection& a, const PeerConnection& b) {
		return a._IP < b._IP;
		});
	// Instructions suggest random assignment. This is not really random but it's closer than it would be otherwise
	srand(_streams[0]._IP);
	// create vector of random numbers 0-3
	std::vector<int> pos;
	for (auto i = 0; i < 4; i++)
		pos.push_back(static_cast<int>(rand() % 4));
	// Ensure no duplicates
	{
		bool done = false;
		while (!done) {
			done = true;
			for (auto i = 0; i < 4; i++)
				for (auto j = i + 1; j < 4; j++)
					if (pos[i] == pos[j]) {
						pos[i] = rand() % 4;
						done = false;
					}
		}
	}
	// Swap the colors to that order
	{
		bool done = false;
		while (!done) {
			done = true;
			// Bubble sort both lists based on rng vector
			for (auto i = 0; i < 3; i++) {
				if (pos[i] > pos[i + 1]) {
					std::swap(pos[i], pos[i + 1]);
					std::swap(_streams[i], _streams[i + 1]);
				}
			}
		}
	}
	// Assign colors based on sort
	_streams[0]._color = red;
	_streams[1]._color = yellow;
	_streams[2]._color = green;
	_streams[3]._color = blue;
	// Figure out my own color
	for (auto& stream : _streams) {
		if (stream._socket == nullptr) {
			_myColor = stream._color;
			return;
		}
	}
}

#ifndef NOT_DISTRIBUTED
void TCPPeer::sendObjectData(const NetCompatibleGameObject& obj, bool newData, helpers::peerColor color) const {
	auto getVec = [](const XMVECTOR& vec) {
		return std::vector<float>({ XMVectorGetX(vec), XMVectorGetY(vec), XMVectorGetZ(vec), XMVectorGetW(vec) });
		};
	if (color == _myColor)
		return;

	// Flatbuffers are the bane of my existence so I have to do this all in one block
	flatbuffers::FlatBufferBuilder builder(1024);
	// BufferData
		// Vertices
	std::vector<flatbuffers::Offset<net::SimpleVertex>> netVertices;
	if (newData) {
		const auto& myVertices = obj.getVertexList();
		for (const auto& vertex : myVertices) {
			std::vector<float> myPos = getVec(vertex.Pos);
			std::vector<float> myNorm = getVec(vertex.Norm);
			std::vector<float> myTex = getVec(vertex.Tex);
			auto netVertex = net::CreateSimpleVertexDirect(builder, &myPos, &myNorm, &myTex);
			netVertices.push_back(netVertex);
		}
	}
	auto convertedNetVertices = builder.CreateVector(netVertices);
		// Indices
	std::vector<uint16_t> netIndices;
	if (newData) {
		const auto& myIndices = obj.getIndexList();
		for (const auto& index : myIndices) {
			netIndices.push_back(static_cast<uint16_t>(index));
		}
	}
	auto convertedNetIndices = builder.CreateVector(netIndices);
	auto netBufferData = net::CreateBufferData(builder, convertedNetVertices, convertedNetIndices);
	// GameObject
	auto netUID = obj.getID();
	auto netType = net::ObjType(obj.getType());
	auto netOrientation = obj.getMatrix4x4();
	auto netStaticPtrPreset = net::StaticPtrPreset(obj.getPtrType());
	auto netIndexCount = obj.getIndexCount();
	auto netPeerColor = net::PeerColor(obj.getColor());
	auto netGameObject = net::CreateGameObject(
		builder,
		netUID,
		netType,
		&netOrientation,
		netStaticPtrPreset,
		netIndexCount,
		netPeerColor,
		netBufferData
	);
	net::FinishGameObjectBuffer(builder, netGameObject);
	_streams[color - 1]._socket->sendGameObject(builder);
}

void TCPPeer::sendImGuiState(const NetCompatibleImGuiState& myState, peerColor color) const {
	auto getVec = [](const XMVECTOR& vec) {
		return std::vector<float>({ XMVectorGetX(vec), XMVectorGetY(vec), XMVectorGetZ(vec), XMVectorGetW(vec) });
		};

	if (color == _myColor || !connectionAlive(color))
		return;
	flatbuffers::FlatBufferBuilder builder(1024);
	auto netRequestingReset = myState._resetLocal;
	auto netNewData = myState._newData;
	auto netTargetRenderFrequency = myState._targetRenderFrequency;
	auto netTargetSimulationFrequency = myState._targetSimulationFrequency;
	auto netTargetNetworkFrequency = myState._targetNetworkFrequency;
	auto netCurrentScenarioIndex = myState._currentScenarioIndex;
	auto netPaused = myState._paused;
	auto netGravityOn = myState._gravityOn;
	auto netGravityInverted = myState._gravityInverted;
	auto netTimestep = myState._timestep;
	std::vector<float> tempBackgroundColor({ myState._backgroundColor[0],myState._backgroundColor[1],myState._backgroundColor[2],myState._backgroundColor[3] });
	auto netBackgroundColor = builder.CreateVector(tempBackgroundColor);
	auto netIntegrationType = static_cast<net::IntegrationType>(myState._integrationType);
	auto tempCameraPositionOffset = getVec(myState._cameraPositionOffset);
	auto netCameraPositionOffset = builder.CreateVector(tempCameraPositionOffset);
	auto netCameraRotationMatrix = myState.get4x4();
	auto netImGuiState = net::CreateImGuiState(
		builder,
		netRequestingReset,
		netNewData,
		netTargetRenderFrequency,
		netTargetSimulationFrequency,
		netTargetNetworkFrequency,
		netCurrentScenarioIndex,
		netPaused,
		netGravityOn,
		netGravityInverted,
		netTimestep,
		netBackgroundColor,
		netIntegrationType,
		netCameraPositionOffset,
		&netCameraRotationMatrix
	);
	net::FinishImGuiStateBuffer(builder, netImGuiState);
#ifdef _DEBUG
	flatbuffers::Verifier verifier(builder.GetBufferPointer(), builder.GetSize());
	if (!verifier.VerifyBuffer<net::ImGuiState>()) {
		std::cout << "Invalid FlatBuffer: ImGuiState verification failed!" << std::endl;
		return;
	}
#endif
	_streams[color-1]._socket->sendImGuiState(builder);
}

void TCPPeer::clearRecvBuffers() {
	for (auto& stream : _streams) {
		if (!connectionAlive(stream._color))
			continue;
		stream._socket->clearRecvBuffer();
	}
}

bool TCPPeer::connectionAlive(peerColor color) const {
	for (auto& stream : _streams) {
		if (stream._color != color)
			continue;
		if (stream._socket == nullptr)
			return false;
		if (stream._socket->testMe())
			return true;
	}
	return false;
}

int TCPPeer::recvType(peerColor color) const {
	if (!connectionAlive(color))
		return 0;
	if (!_streams[color - 1]._socket->waitForMagicNumber())
		return 0;
	int type = 0;
	if (!_streams[color - 1]._socket->recvInt(type))
		return 0;
	return type;
}

int TCPPeer::recvSize(peerColor color) const {
	int size = -1;
	if (!connectionAlive(color))
		return -1;
	if (!_streams[color - 1]._socket->recvInt(size))
		return -1;
	return size;
}

void TCPPeer::recvFB(std::vector<uint8_t>& buf, peerColor color) const {
	if (!connectionAlive(color))
		return;
	_streams[color - 1]._socket->waitRecv(buf);
}
#endif