#pragma once

#include "PhysicsObject.h"

/////////////////////
// Static class

class Intersect final {
public:
	Intersect() = delete;
	Intersect(const Intersect&) = delete;
	Intersect& operator=(const Intersect&) = delete;
	Intersect& operator=(const Intersect&&) = delete;
	Intersect(const Intersect&&) = delete;

	static bool testIntersect(const Capsule&, const Capsule&);
	static bool testIntersect(const Capsule&, const HollowCube&);
	static bool testIntersect(const Capsule&, const Plane&);
	static bool testIntersect(const Capsule&, const Sphere&);
	static bool testIntersect(const HollowCube&, const HollowCube&);
	static bool testIntersect(const HollowCube&, const Plane&);
	static bool testIntersect(const HollowCube&, const Sphere&);
	static bool testIntersect(const Plane&, const Plane&);
	static bool testIntersect(const Plane&, const Sphere&);
	static bool testIntersect(const Sphere&, const Sphere&);
};