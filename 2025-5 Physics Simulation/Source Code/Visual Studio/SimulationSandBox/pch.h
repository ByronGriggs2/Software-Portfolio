// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
// Not gonna change, external
#include "framework.h"
#include "winsock2.h"
#include <Windows.h>

#include <atlbase.h>
#include <algorithm>
#include <cmath>
#include <d3d11.h>
#include "DirectXMath.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include "DDSTextureLoader.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "imgui_stdlib.h"

// Static lib
#include "NotImplementedException.h"
#include "shapeHelpers.h"
#include "LineSegment.h"
#include "Line.h"
#include "PhysicsObject.h"
#include "Capsule.h"
#include "HollowCube.h"
#include "Plane.h"
#include "Sphere.h"
#include "Intersect.h"
#include "CollisionManager.h"

// Helpers, used widely
#include "filepathHelpers.h"
#include "socketTest.h"
#include "structs.h"
#include "peerColor.h"
#include "ObjectPresets.h"

// Not gonna change, internal
#include "ObjectGeometry.h"
#include "GeometryGenerator.h"

#endif //PCH_H
