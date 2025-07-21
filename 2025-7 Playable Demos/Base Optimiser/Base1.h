#pragma once
#include "Base.h"

constexpr int HEIGHT = 4;
constexpr int WIDTH = 4;

constexpr double level7BoostStrength = 1.88578;
constexpr double level7BoostCost = 1e9;
constexpr double level8BoostStrength = 1.97436;
constexpr double level8BoostCost = 1e10;
constexpr double buildingMatMultiplier = 6.19;

constexpr double BUILDING_STRENGTH = 1577.72 * buildingMatMultiplier;
constexpr double BUILDING_UPGRADE_COST = 1.29e9;
constexpr double BATTLE_INPUT = 969.35;
constexpr double BATTLE_STRENGTH = 1007.69;
constexpr double BATTLE_UPGRADE_COST = 3.2e8;
constexpr double BOOST_STRENGTH = level7BoostStrength;
constexpr double BOOST_UPGRADE_COST = level8BoostCost;

constexpr double NEXT_LEVEL_BUILDING_STRENGTH = 1972.15;
constexpr double NEXT_LEVEL_BATTLE_INPUT = 1211.69;
constexpr double NEXT_LEVEL_BATTLE_STRENGTH = 1310.0;
constexpr double NEXT_LEVEL_BOOST_STRENGTH = level8BoostStrength;

class Base1 final : public Base
{
	struct OptimizationResult {
		double _value = 0.0;
		std::vector<std::vector<elemType>> _optimizedGrid;
		OptimizationResult(const std::vector<std::vector<elemType>>& grid) : _optimizedGrid(grid) {}
	};
	static void maxBuildingMaterials(int row, int col, std::vector<std::vector<elemType>>& workingGrid, OptimizationResult& currentBest, double buildingStrength = BUILDING_STRENGTH, double boostStrength = BOOST_STRENGTH);
	static void maxBattleComponents(int row, int col, std::vector<std::vector<elemType>>& workingGrid, OptimizationResult& currentBest, double buildingStrength = BUILDING_STRENGTH, double boostStrength = BOOST_STRENGTH, double battleInput = BATTLE_INPUT, double battleStrength = BATTLE_STRENGTH);
	static double calculateBaseYield(const std::vector<std::vector<elemType>>& grid, elemType type, double elemStrength, double boostStrength = BOOST_STRENGTH);
public:
	Base1() {
		_grid = std::vector<std::vector<elemType>>(HEIGHT, std::vector<elemType>(WIDTH, building));
		_grid[0][2] = blank;
		_grid[2][3] = blank;
		_grid[3][1] = blank;
		_grid[3][2] = blank;
		_grid[3][3] = blank;
	}
	std::vector<std::vector<elemType>> optimizeBuildingMats() const override;
	std::vector<std::vector<elemType>> optimizeBattleComponents() const override;
	elemType optimalUpgradeBuildingMats(double currentCash) const override;
	elemType optimalUpgradeBattleComponents(double currentCash) const override;
};

