#pragma once
#include "Base.h"

constexpr int HEIGHT = 4;
constexpr int WIDTH = 4;
constexpr double BUILDING_MAT_MULTIPLIER = 6.19;
constexpr double BATTLE_COMP_MULTIPLIER = 2.59;

// These constants are here until I get more datapoints to verify my formula assumptions
constexpr double level34BuildingOutput = 1577.72;
constexpr double level35BuildingOutput = 1972.15;
constexpr double level35BuildingCost = 1.29e9;
constexpr double level36BuildingOutput = 2465.19;
constexpr double level36BuildingCost = 1.94e9;
constexpr double level7BoostStrength = 1.88578;
constexpr double level8BoostStrength = 1.97436;
constexpr double level7BoostCost = 1e9;
constexpr double level8BoostCost = 1e10;

// I only have 1 datapoint so I can't even make a speculative formula
constexpr double BUILDING_UPGRADE_COST = level35BuildingCost;
class Base1 final : public Base
{
	struct OptimizationResult {
		double _value = 0.0;
		std::vector<std::vector<elemType>> _optimizedGrid;
		OptimizationResult(const std::vector<std::vector<elemType>>& grid) : _optimizedGrid(grid) {}
	};
	const double _buildingStrength;
	const double _buildingCost;
	const double _boostStrength;
	const double _boostCost;
	const double _battleInput;
	const double _battleStrength;
	const double _battleCost;
	const double _nextBuildingStrength;
	const double _nextBoostStrength;
	const double _nextBattleInput;
	const double _nextBattleStrength;

	static void maxBuildingMaterials(int row, int col, std::vector<std::vector<elemType>>& workingGrid, OptimizationResult& currentBest, double buildingStrength, double boostStrength);
	static void maxBattleComponents(int row, int col, std::vector<std::vector<elemType>>& workingGrid, OptimizationResult& currentBest, double buildingStrength, double boostStrength, double battleInput, double battleStrength);
	static double calculateBaseYield(const std::vector<std::vector<elemType>>& grid, elemType type, double elemStrength, double boostStrength);

	static double buildingStrengthValue(int level) { return 0.8 * pow(1.25, level); }
	static double boostStrengthValue(int level) { return 1.27951459663 * pow(1.0569726055, level);}
	static double boostCostValue(int nextLevel) { return 100 * pow(10.0, nextLevel); }
	static double battleInputValue(int level) { return 1.2 * pow(1.25, level);}
	static double battleOutputValue(int level) { return 0.5 * pow(1.3, level - 1) * BATTLE_COMP_MULTIPLIER; }
	static double battleCostValue(int nextLevel) { return 11125 * pow(1.5, nextLevel); }
public:
	Base1() = delete;
	Base1(int buildingLevel, int boostLevel, int battleLevel);

	std::vector<std::vector<elemType>> optimizeBuildingMats() const override;
	std::vector<std::vector<elemType>> optimizeBattleComponents() const override;
	elemType optimalUpgradeBuildingMats(double currentCash) const override;
	elemType optimalUpgradeBattleComponents(double currentCash) const override;
};

