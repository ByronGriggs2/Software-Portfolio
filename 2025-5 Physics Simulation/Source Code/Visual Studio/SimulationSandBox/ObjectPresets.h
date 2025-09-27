#pragma once

#include "PhysicsObject.h"
#include "structs.h"

namespace helpers {
	enum staticPtrPreset {
		def,
		texture
	};
	staticPtrPreset stringToPtrPreset(const char* str) {
		auto compStr = [](const char* str1, const char* str2, int str2Len) {
			for (auto i = 0; i < str2Len; i++) {
				if (str1[i] != str2[i])
					return false;
			}
			return true;
			};
		if (compStr(str, "def", 3))
			return def;
		if (compStr(str, "texture", 7))
			return texture;
#ifdef _DEBUG
		throw NotImplementedException();
#endif
	}
	struct ObjectPresets {
		bool _forcedRed = false;
		PhysicsObject::constructParams _params;
		shapeHelpers::ObjType _type;
		SpecialInit _initData;
		staticPtrPreset _ptrs = def;
		ObjectPresets(shapeHelpers::ObjType type);
	};
}