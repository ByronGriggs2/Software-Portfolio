#pragma once
#include "CoreMinimal.h"
#include <unordered_map>
#include "Definitions.h"

// I'm not used to templates, but apparently I'm not allowed a .cpp file
template<typename stdEnum, typename UEnum>
class EnumConverter {
	const static std::unordered_map<stdEnum, UEnum> map;
public:
	static stdEnum toStdEnum(UEnum input) {
		for (const auto& pair : map) {
			if (pair.second == input) {
				return pair.first;
			}
		}
		LOGERROR("Failed to find stdEnum for UEnum in EnumConverter::toStdEnum");
		return static_cast<stdEnum>(0);
	}
	static UEnum toUEnum(stdEnum input) {
		auto it = map.find(input);
		if (it == map.end()) {
			LOGERROR("Failed to find UEnum for stdEnum in EnumConverter::toUEnum");
			return static_cast<UEnum>(0);
		}
		return it->second;
	}
	static TMap<UEnum, float> toTMap(const std::unordered_map<stdEnum, float>& input) {
		TMap<UEnum, float> ret;
		for (const auto& pair : input) {
			ret.Add(toUEnum(pair.first), pair.second);
		}
		return ret;
	}
	static std::unordered_map<stdEnum, float> toUnorderedMap(const TMap<UEnum, float>& input) {
		std::unordered_map<stdEnum, float> ret;
		for (const auto& pair : input) {
			ret[toStdEnum(pair.Key)] = pair.Value;
		}
		return ret;
	}
};