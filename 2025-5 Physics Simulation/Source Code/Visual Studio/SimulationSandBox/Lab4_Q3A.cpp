#include "Lab4_Q3A.h"

Lab4_Q3A::Lab4_Q3A() : Scenario() {
	_myPath = std::filesystem::current_path() / "Assets" / "basicSphere";
	//my favorite color
	_backgroundColor.get()[0] = 0;
	_backgroundColor.get()[1] = (4 * 16 + 1) / 255.0f;
	_backgroundColor.get()[2] = (8 * 16 + 2) / 255.0f;

	_cameraPos = XMVectorSet(0, 8, -0.001, 1.0f);
	_lightPos = XMVectorSet(0, 5, 0, 1.0f);

	ObjectPresets leftSphere(shapeHelpers::sphere, "obj0.obj", XMMatrixTranslation(-3, 0, -3), XMVectorSet(3, 0, 3, 0), false, 10.0);
	leftSphere._initData._sphere._radius = 1.0f;
	_objectPresets.push_back(leftSphere);

	ObjectPresets rightSphere(shapeHelpers::sphere, "obj0.obj", XMMatrixTranslation(0, 0, 0), XMVectorSet(0, 0, 0, 0), false, 25.0);
	rightSphere._initData._sphere._radius = 1.0f;
	_objectPresets.push_back(rightSphere);
}

void Lab4_Q3A::generateObjects() const {
	auto& objectGenerator = GenerateObj::getInstance();
	objectGenerator.resetGenerator();
	objectGenerator.addSphere(1, 100);
	objectGenerator.generate(_myPath);
}