#include "pch.h"
#include "ScenarioVector.h"
#include "ScenarioA.h"
#include "ScenarioB.h"
#include "ScenarioC.h"
#include "ScenarioC2.h"
#include "ScenarioD.h"
#include "ScenarioE.h"
#include "ScenarioF.h"
#include "ScenarioG.h"
#include "ScenarioH.h"
#include "ScenarioI.h"

using namespace helpers;

std::vector<std::unique_ptr<Scenario>> ScenarioVector::giveVector(CComPtr<ID3D11Device> device, peerColor color) {
	std::vector<std::unique_ptr<Scenario>> temp;
	temp.push_back(std::make_unique<ScenarioA>(device, color));
	temp.push_back(std::make_unique<ScenarioB>(device, color));
	temp.push_back(std::make_unique<ScenarioC>(device, color));
	temp.push_back(std::make_unique<ScenarioC2>(device, color));
	temp.push_back(std::make_unique<ScenarioD>(device, color));
	temp.push_back(std::make_unique<ScenarioE>(device, color));
	temp.push_back(std::make_unique<ScenarioF>(device, color));
	temp.push_back(std::make_unique<ScenarioG>(device, color));
	temp.push_back(std::make_unique<ScenarioH>(device, color));
	temp.push_back(std::make_unique<ScenarioI>(device, color));
	return temp;
}

std::vector<const char*> ScenarioVector::getScenarioNames() {
	std::vector<const char*> temp;
	temp.push_back(ScenarioA::myName());
	temp.push_back(ScenarioB::myName());
	temp.push_back(ScenarioC::myName());
	temp.push_back(ScenarioC2::myName());
	temp.push_back(ScenarioD::myName());
	temp.push_back(ScenarioE::myName());
	temp.push_back(ScenarioF::myName());
	temp.push_back(ScenarioG::myName());
	temp.push_back(ScenarioH::myName());
	temp.push_back(ScenarioI::myName());
	return temp;
}