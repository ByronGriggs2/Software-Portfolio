#include "Lab4_Q1.h"

Lab4_Q1::Lab4_Q1() : Scenario() {
	_myPath = std::filesystem::current_path() / "Assets" / "Lab4";
	//my favorite color
	_backgroundColor.get()[0] = 0;
	_backgroundColor.get()[1] = (4 * 16 + 1) / 255.0f;
	_backgroundColor.get()[2] = (8 * 16 + 2) / 255.0f;

	_cameraPos = XMVectorSet(-2.5, 0, -10, 1.0f);
	_lightPos = XMVectorSet(0, 5, 0, 1.0f);

	ObjectPresets leftSphere(shapeHelpers::sphere, "obj0.obj", XMMatrixTranslation(-3, 0, 0), XMVectorSet(3, 0, 0, 0));
	leftSphere._initData._sphere._radius = 1.0f;
	_objectPresets.push_back(leftSphere);

	ObjectPresets plane(shapeHelpers::plane, "obj1.obj", shapeHelpers::convertToOrientation(XMVectorSet(0,0,0,0), XMVectorSet(-1,1,0,0)), XMVectorSet(0, 0, 0, 0), true);
	_objectPresets.push_back(plane);
}

void Lab4_Q1::generateObjects() const {
	auto& objectGenerator = GenerateObj::getInstance();
	objectGenerator.resetGenerator();
	objectGenerator.addSphere(1, 100);
	objectGenerator.addPlane(1);
	objectGenerator.generate(_myPath);
}