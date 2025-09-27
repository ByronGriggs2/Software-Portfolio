#pragma once
#include <memory>
#include "Line.h"
#include "LineSegment.h"
#include "Plane.h"
#include "Point.h"
#include "Sphere.h"
#include "PhysicsObject.h"
#include "NotImplementedException.h"
#include "shapeHelpers.h"

using namespace shapeHelpers;

//class PhysicsObjectProxy
//{
//	ObjType _myType;
//	std::unique_ptr<Line> _linePtr;
//	std::unique_ptr<LineSegment> _lineSegmentPtr;
//	std::unique_ptr<Plane> _planePtr;
//	std::unique_ptr<Point> _pointPtr;
//	std::unique_ptr<Sphere> _spherePtr;
//	std::unique_ptr<PhysicsObject> _physPtr;
//public:
//	PhysicsObjectProxy(ObjType type) : _myType(type), _linePtr(nullptr), _lineSegmentPtr(nullptr), _planePtr(nullptr), _pointPtr(nullptr),
//		_spherePtr(nullptr), _physPtr(nullptr) {
//		switch (type) {
//		case line:
//			_linePtr = std::make_unique<Line>();
//			break;
//		case lineSegment:
//			_lineSegmentPtr = std::make_unique<LineSegment>();
//			break;
//		case plane:
//			_planePtr = std::make_unique<Plane>();
//			break;
//		case point:
//			_pointPtr = std::make_unique<Point>();
//			break;
//		case sphere:
//			_spherePtr = std::make_unique<Sphere>();
//			break;
//		case physObj:
//			AbstractClassException hello;
//			break;
//		}
//	}
//	// Default constructor
//	PhysicsObjectProxy() : PhysicsObjectProxy(physObj) {}
//	// Copy constructor
//	PhysicsObjectProxy(const PhysicsObjectProxy& other) : _myType(other._myType) {
//		if (other._linePtr) _linePtr = std::make_unique<Line>(*other._linePtr);
//		if (other._lineSegmentPtr) _lineSegmentPtr = std::make_unique<LineSegment>(*other._lineSegmentPtr);
//		if (other._planePtr) _planePtr = std::make_unique<Plane>(*other._planePtr);
//		if (other._pointPtr) _pointPtr = std::make_unique<Point>(*other._pointPtr);
//		if (other._spherePtr) _spherePtr = std::make_unique<Sphere>(*other._spherePtr);
//		if (other._physPtr) _physPtr = std::make_unique<PhysicsObject>(*other._physPtr);
//	}
//	// Copy assignment operator
//	PhysicsObjectProxy& operator=(const PhysicsObjectProxy& other) {
//		if (this != &other) {
//			_myType = other._myType;
//			if (other._linePtr) _linePtr = std::make_unique<Line>(*other._linePtr);
//			if (other._lineSegmentPtr) _lineSegmentPtr = std::make_unique<LineSegment>(*other._lineSegmentPtr);
//			if (other._planePtr) _planePtr = std::make_unique<Plane>(*other._planePtr);
//			if (other._pointPtr) _pointPtr = std::make_unique<Point>(*other._pointPtr);
//			if (other._spherePtr) _spherePtr = std::make_unique<Sphere>(*other._spherePtr);
//			if (other._physPtr) _physPtr = std::make_unique<PhysicsObject>(*other._physPtr);
//		}
//		return *this;
//	}
//	// Move constructor
//	PhysicsObjectProxy(PhysicsObjectProxy&& other) noexcept : _myType(other._myType), _linePtr(std::move(other._linePtr)), _lineSegmentPtr(std::move(other._lineSegmentPtr)),
//		_planePtr(std::move(other._planePtr)), _pointPtr(std::move(other._pointPtr)), _spherePtr(std::move(other._spherePtr)), _physPtr(std::move(other._physPtr)) {}
//	// Move assignment operator
//	PhysicsObjectProxy& operator=(PhysicsObjectProxy&& other) noexcept {
//		if (this != &other) {
//			_myType = other._myType;
//			_linePtr = std::move(other._linePtr);
//			_lineSegmentPtr = std::move(other._lineSegmentPtr);
//			_planePtr = std::move(other._planePtr);
//			_pointPtr = std::move(other._pointPtr);
//			_spherePtr = std::move(other._spherePtr);
//			_physPtr = std::move(other._physPtr);
//		}
//		return *this;
//	}
//
//	ObjType getType() const { return _myType; }
//	PhysicsObject* getPtr() const;
//
//
//	bool kinematics(float deltaT);
//
//	bool testIntersect(const PhysicsObjectProxy& other) const;
//	bool testSphereIntersect(const PhysicsObjectProxy& other) const;
//	bool testLineIntersect(const PhysicsObjectProxy& other) const;
//	bool testLineSegmentIntersect(const PhysicsObjectProxy& other) const;
//	bool testPointIntersect(const PhysicsObjectProxy& other) const;
//	bool testPlaneIntersect(const PhysicsObjectProxy& other) const;
//};
//
