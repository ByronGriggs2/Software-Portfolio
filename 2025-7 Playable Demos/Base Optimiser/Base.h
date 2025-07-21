#pragma once
#include <vector>

// There are 4 bases. I've only unlocked the first, so I don't know how different they are. If they aren't very different, I can move most of Base1 to here and reuse it for Base2.
class Base
{
public:
	enum elemType {
		blank,
		building,
		battle,
		boost
	};
protected:
	std::vector<std::vector<elemType>> _grid;
public:
	virtual std::vector<std::vector<elemType>> optimizeBuildingMats() const = 0;
	virtual std::vector<std::vector<elemType>> optimizeBattleComponents() const = 0;
	virtual elemType optimalUpgradeBuildingMats(double currentCash) const = 0;
	virtual elemType optimalUpgradeBattleComponents(double currentCash) const = 0;
	static void printGrid(const std::vector <std::vector<elemType>>& grid);
	static void printEnum(elemType);
};

