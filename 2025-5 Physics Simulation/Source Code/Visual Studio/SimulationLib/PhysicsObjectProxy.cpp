#include "pch.h"
#include "PhysicsObjectProxy.h"

//using namespace shapeHelpers;
//
//bool PhysicsObjectProxy::kinematics(float deltaT) {
//	switch (_myType) {
//	case line:
//		return _linePtr->kinematics(deltaT);
//		break;
//	case lineSegment:
//		return _lineSegmentPtr->kinematics(deltaT);
//		break;
//	case plane:
//		return _planePtr->kinematics(deltaT);
//		break;
//	case point:
//		return _pointPtr->kinematics(deltaT);
//		break;
//	case sphere:
//		return _spherePtr->kinematics(deltaT);
//		break;
//	case physObj:
//	{ AbstractClassException hello; }
//		return false;
//		break;
//	default:
//	{ NotImplementedException hi; }
//		return false;
//		break;
//	}
//}
//
//bool PhysicsObjectProxy::testIntersect(const PhysicsObjectProxy& other) const {
//	switch (_myType) {
//	case line:
//		return testLineIntersect;
//		break;
//	case lineSegment:
//		return testLineSegmentIntersect;
//		break;
//	case plane:
//		return testPlaneIntersect;
//		break;
//	case point:
//		return testPointIntersect;
//		break;
//	case sphere:
//		return testSphereIntersect;
//		break;
//	case physObj:
//		{ AbstractClassException hello; }
//		return false;
//		break;
//	default:
//		{ NotImplementedException hi; }
//		return false;
//		break;
//	}
//}
//
//
//bool PhysicsObjectProxy::testLineIntersect(const PhysicsObjectProxy& other) const {
//	switch (other._myType) {
//	case line:
//		return _linePtr->testIntersect(*other._linePtr);
//		break;
//	case lineSegment:
//		return _linePtr->testIntersect(*other._lineSegmentPtr);
//		break;
//	case plane:
//		return _linePtr->testIntersect(*other._planePtr);
//		break;
//	case point:
//		return _linePtr->testIntersect(*other._pointPtr);
//		break;
//	case sphere:
//		return _linePtr->testIntersect(*other._spherePtr);
//		break;
//	case physObj:
//		{ AbstractClassException hello; }
//		return false;
//		break;
//	default:
//		{ NotImplementedException hi; }
//		return false;
//		break;
//	}
//}
//bool PhysicsObjectProxy::testLineSegmentIntersect(const PhysicsObjectProxy& other) const {
//	switch (other._myType) {
//	case line:
//		return _lineSegmentPtr->testIntersect(*other._linePtr);
//		break;
//	case lineSegment:
//		return _lineSegmentPtr->testIntersect(*other._lineSegmentPtr);
//		break;
//	case plane:
//		return _lineSegmentPtr->testIntersect(*other._planePtr);
//		break;
//	case point:
//		return _lineSegmentPtr->testIntersect(*other._pointPtr);
//		break;
//	case sphere:
//		return _lineSegmentPtr->testIntersect(*other._spherePtr);
//		break;
//	case physObj:
//		{ AbstractClassException hello; }
//		return false;
//		break;
//	default:
//		{ NotImplementedException hi; }
//		return false;
//		break;
//	}
//}
//bool PhysicsObjectProxy::testPlaneIntersect(const PhysicsObjectProxy& other) const {
//	switch (other._myType) {
//	case line:
//		return _planePtr->testIntersect(*other._linePtr);
//		break;
//	case lineSegment:
//		return _planePtr->testIntersect(*other._lineSegmentPtr);
//		break;
//	case plane:
//		return _planePtr->testIntersect(*other._planePtr);
//		break;
//	case point:
//		return _planePtr->testIntersect(*other._pointPtr);
//		break;
//	case sphere:
//		return _planePtr->testIntersect(*other._spherePtr);
//		break;
//	case physObj:
//		{ AbstractClassException hello; }
//		return false;
//		break;
//	default:
//		{ NotImplementedException hi; }
//		return false;
//		break;
//	}
//
//}
//bool PhysicsObjectProxy::testPointIntersect(const PhysicsObjectProxy& other) const {
//	switch (other._myType) {
//	case line:
//		return _pointPtr->testIntersect(*other._linePtr);
//		break;
//	case lineSegment:
//		return _pointPtr->testIntersect(*other._lineSegmentPtr);
//		break;
//	case plane:
//		return _pointPtr->testIntersect(*other._planePtr);
//		break;
//	case point:
//		return _pointPtr->testIntersect(*other._pointPtr);
//		break;
//	case sphere:
//		return _pointPtr->testIntersect(*other._spherePtr);
//		break;
//	case physObj:
//		{ AbstractClassException hello; }
//		return false;
//		break;
//	default:
//		{ NotImplementedException hi; }
//		return false;
//		break;
//	}
//}
//
//bool PhysicsObjectProxy::testSphereIntersect(const PhysicsObjectProxy& other) const {
//	switch (other._myType) {
//	case line:
//		return _spherePtr->testIntersect(*other._linePtr);
//		break;
//	case lineSegment:
//		return _spherePtr->testIntersect(*other._lineSegmentPtr);
//		break;
//	case plane:
//		return _spherePtr->testIntersect(*other._planePtr);
//		break;
//	case point:
//		return _spherePtr->testIntersect(*other._pointPtr);
//		break;
//	case sphere:
//		return _spherePtr->testIntersect(*other._spherePtr);
//		break;
//	case physObj:
//		{ AbstractClassException hello; }
//		return false;
//		break;
//	default:
//		{ NotImplementedException hi; }
//		return false;
//		break;
//	}
//}