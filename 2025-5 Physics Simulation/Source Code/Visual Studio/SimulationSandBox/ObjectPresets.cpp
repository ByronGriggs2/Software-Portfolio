#include "pch.h"
#include "ObjectPresets.h"

helpers::ObjectPresets::ObjectPresets(shapeHelpers::ObjType type) : _type(type) {
	//I could just make these the default values but I want to make sure it gets initialized correctly
	switch (type) {
	case shapeHelpers::capsule:
		_initData._capsule._height = 1.0f;
		_initData._capsule._radius = 1.0f;
		break;
	case shapeHelpers::hollowCube:
		_initData._hollowCube._sideLength = 3.0f;
		break;
	case shapeHelpers::plane:
		break;
	case shapeHelpers::sphere:
		_initData._sphere._radius = 1.0f;
		break;
#ifdef _DEBUG
	case shapeHelpers::physObj:
		throw AbstractClassException();
	default:
		throw NotImplementedException();
#endif
	}
}