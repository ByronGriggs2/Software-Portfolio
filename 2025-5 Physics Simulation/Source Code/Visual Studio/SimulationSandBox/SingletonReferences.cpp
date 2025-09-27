#include "pch.h"
#include "SingletonReferences.h"
#include "NetworkManager.h"
#include "SharedMemory.h"
#include "AdvancedSetup.h"
#include "ImGuiHandler.h"
#include "ScenarioManager.h"
#include "PhysicsSimulation.h"
#include "D3DFramework.h"

SingletonReferences::SingletonReferences() {
	_p2 = D3DFramework::getInstance();
	_p3 = ImGuiHandler::getInstance();
	_p4 = ScenarioManager::getInstance();
	_p5 = SharedMemory::getInstance();
	_p6 = PhysicsSimulation::getInstance();
	_p7 = NetworkManager::getInstance();
}