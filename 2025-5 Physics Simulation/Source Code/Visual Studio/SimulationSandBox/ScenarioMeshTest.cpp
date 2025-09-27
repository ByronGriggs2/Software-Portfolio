#include "ScenarioMeshTest.h"

ScenarioMeshTest::ScenarioMeshTest(const helpers::shaderListStruct& list, const helpers::ObjectPtrs& ptrs) : Scenario(list, ptrs) {
	_myPath = std::filesystem::current_path() / "Assets" / "ScenarioMeshTest";
	//some boring color
	_backgroundColor.get()[0] = 0;
	_backgroundColor.get()[1] = 0.5;
	_backgroundColor.get()[2] = 0;

	_cameraPos = XMVectorSet(0, 0, -5, 1.0f);
	_lightPos = XMVectorSet(0, 1000, 0, 1.0f);

	//ObjectPresets obj(XMVectorSet(0, 0, 0, 0), XMQuaternionIdentity(), XMVectorSet(0, 0, 0, 0), shapeHelpers::sphere, "obj0.obj");
	//obj._initData._capsule._radius = 1.0f;
	//obj._initData._capsule._height = 1.0f;
	//_objectPresets.push_back(obj);

	ObjectPresets obj = newObjPreset(XMVectorSet(0, 0, 0, 0), XMQuaternionIdentity(), XMVectorSet(0, 0, 0, 0), shapeHelpers::sphere, "obj0.obj", 1);
	obj._initData._sphere._radius = 1.0f;
	_objectPresets.push_back(obj);
}

void ScenarioMeshTest::generateObjects() const {
	auto& objectGenerator = GenerateObj::getInstance();
	objectGenerator.resetGenerator();
	objectGenerator.addSphere(1, 100);
	//objectGenerator.addCapsule(5, 1, 100);
	objectGenerator.generate(_myPath);
}