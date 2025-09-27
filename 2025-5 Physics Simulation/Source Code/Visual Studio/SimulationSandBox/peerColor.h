#pragma once

#include "NotImplementedException.h"

namespace helpers {
	enum peerColor {
		undef,
		red,
		yellow,
		green,
		blue
	};

	inline const char* colorToString(peerColor color) {
		switch (color) {
		case red:
			return "red";
			break;
		case yellow:
			return "yellow";
			break;
		case green:
			return "green";
			break;
		case blue:
			return "blue";
			break;
		case undef:
			return "undef";
			break;
		default:
#ifdef _DEBUG
			throw ShouldNotRunException();
#endif
			break;
		}
	}
	peerColor stringToColor(const char* str);
}