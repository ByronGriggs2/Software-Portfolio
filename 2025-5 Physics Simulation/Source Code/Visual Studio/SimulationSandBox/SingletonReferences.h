#pragma once

#include <memory>

class D3DFramework;
class ImGuiHandler;
class ScenarioManager;
class SharedMemory;
class PhysicsSimulation;
class NetworkManager;
// This class holds a reference to every singleton so that they do not get destroyed early. Every singleton instance needs to be a shared ptr for this to work.
class SingletonReferences {
	std::shared_ptr<D3DFramework> _p2;
	std::shared_ptr<ImGuiHandler> _p3;
	std::shared_ptr<ScenarioManager> _p4;
	std::shared_ptr<SharedMemory> _p5;
	std::shared_ptr<PhysicsSimulation> _p6;
	std::shared_ptr<NetworkManager> _p7;
public:
	SingletonReferences();
};