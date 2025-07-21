#include "Base1.h"

using std::vector;

vector<vector<Base1::elemType>> Base1::optimizeBuildingMats() const {
    OptimizationResult result(_grid);
    vector<vector<elemType>> workingVector = _grid;
    maxBuildingMaterials(0, 0, workingVector, result);
    return result._optimizedGrid;
}

vector<vector<Base1::elemType>> Base1::optimizeBattleComponents() const {
    OptimizationResult result(_grid);
    vector<vector<elemType>> workingVector = _grid;
    maxBattleComponents(0, 0, workingVector, result);
    return result._optimizedGrid;
}

// Recursive function, calculates the building materials yield for every possible variation of the grid. Non-terminal function calls only serve to populate the grid.
// terminal function calls evaluate the completed grid, and place the value in currentBest if necessary.
// When the original function returns, the optimized grid is held in currentBest.
void Base1::maxBuildingMaterials(int row, int col, vector<vector<elemType>>& workingGrid, OptimizationResult& currentBest, double buildingStrength, double boostStrength) {
    const bool isLastSquare = row == HEIGHT - 1 && col == WIDTH - 1;
    if (isLastSquare) {
        auto compareAndSave = [row, col, &workingGrid, &currentBest, buildingStrength, boostStrength]() {
            double newValue = calculateBaseYield(workingGrid, building, buildingStrength, boostStrength);
            if (newValue > currentBest._value) {
                currentBest._value = newValue;
                currentBest._optimizedGrid = workingGrid;
            }
            };
        if (workingGrid[row][col] == blank) {
            compareAndSave();
            return;
        }
        elemType restore = workingGrid[row][col];
        workingGrid[row][col] = building;
        compareAndSave();
        workingGrid[row][col] = boost;
        compareAndSave();
        workingGrid[row][col] = restore;
        return;
    }
    const int nextRow = col == WIDTH - 1 ? row + 1 : row;
    const int nextCol = col == WIDTH - 1 ? 0 : col + 1;
    if (workingGrid[row][col] == blank) {
        maxBuildingMaterials(nextRow, nextCol, workingGrid, currentBest, buildingStrength, boostStrength);
        return;
    }
    elemType restore = workingGrid[row][col];
    workingGrid[row][col] = building;
    maxBuildingMaterials(nextRow, nextCol, workingGrid, currentBest, buildingStrength, boostStrength);
    workingGrid[row][col] = boost;
    maxBuildingMaterials(nextRow, nextCol, workingGrid, currentBest, buildingStrength, boostStrength);
}

// As above, but calculates the maximum battle component yield. Assumes you have 0 building materials stocked (or you're running it for infinite time)
void Base1::maxBattleComponents(int row, int col, vector<vector<elemType>>& workingGrid, OptimizationResult& currentBest, double buildingStrength, double boostStrength, double battleInput, double battleStrength) {
    const bool isLastSquare = row == HEIGHT - 1 && col == WIDTH - 1;
    auto compareAndSave = [row, col, &workingGrid, &currentBest, buildingStrength, boostStrength, battleInput, battleStrength]() {
        const double buildingMatsCreated = calculateBaseYield(workingGrid, building, buildingStrength, boostStrength);
        const double buildingMatsConsumed = calculateBaseYield(workingGrid, battle, battleInput, boostStrength);
        const double battleCreated = calculateBaseYield(workingGrid, battle, battleStrength, boostStrength);
        const double uptime = buildingMatsCreated > buildingMatsConsumed ? 1.0 : buildingMatsCreated / buildingMatsConsumed;
        const double newValue = battleCreated * uptime;
        if (newValue > currentBest._value) {
            currentBest._value = newValue;
            currentBest._optimizedGrid = workingGrid;
        }
        };
    if (isLastSquare) {
        if (workingGrid[row][col] == blank) {
            compareAndSave();
            return;
        }
        elemType restore = workingGrid[row][col];
        workingGrid[row][col] = building;
        compareAndSave();
        workingGrid[row][col] = boost;
        compareAndSave();
        workingGrid[row][col] = battle;
        compareAndSave();
        workingGrid[row][col] = restore;
        return;
    }

    const int nextRow = col == WIDTH - 1 ? row + 1 : row;
    const int nextCol = col == WIDTH - 1 ? 0 : col + 1;
    if (workingGrid[row][col] == blank) {
        maxBattleComponents(nextRow, nextCol, workingGrid, currentBest, buildingStrength, boostStrength, battleInput, battleStrength);
        return;
    }
    elemType restore = workingGrid[row][col];
    workingGrid[row][col] = building;
    maxBattleComponents(nextRow, nextCol, workingGrid, currentBest, buildingStrength, boostStrength, battleInput, battleStrength);
    workingGrid[row][col] = boost;
    maxBattleComponents(nextRow, nextCol, workingGrid, currentBest, buildingStrength, boostStrength, battleInput, battleStrength);
    workingGrid[row][col] = battle;
    maxBattleComponents(nextRow, nextCol, workingGrid, currentBest, buildingStrength, boostStrength, battleInput, battleStrength);
    workingGrid[row][col] = restore;
}

// Calculates the building materials produced or consumed, or battle components produced for the current grid, depending on paramaters
double Base1::calculateBaseYield(const vector<vector<Base1::elemType>>& base, Base1::elemType type, double typeStrength, double boostStrength) {
    double sum = 0.0;
    for (auto row = 0; row < HEIGHT; row++) {
        for (auto col = 0; col < WIDTH; col++) {
            if (base[row][col] == type) {
                float multiplier = 1.0f;
                if (row > 0 && base[row - 1][col] == boost)
                    multiplier *= boostStrength;
                if (row < HEIGHT - 1 && base[row + 1][col] == boost)
                    multiplier *= boostStrength;
                if (col > 0 && base[row][col - 1] == boost)
                    multiplier *= boostStrength;
                if (col < WIDTH - 1 && base[row][col + 1] == boost)
                    multiplier *= boostStrength;
                sum += multiplier * typeStrength;
            }
        }
    }
    return sum;
}

// Chooses the optimal next purchase for building materials using the above recursive functions. Optimal is the choice with the lowest payback period
// (time to afford + time to pay for itself).
Base1::elemType Base1::optimalUpgradeBuildingMats(double currentCash) const {
    vector<vector<elemType>> workingVector = _grid;
    auto getOptimalIncome = [this, &workingVector](double buildingStrength = BUILDING_STRENGTH, double boostStrength = BOOST_STRENGTH) {
        OptimizationResult result(_grid);
        maxBuildingMaterials(0, 0, workingVector, result, buildingStrength, boostStrength);
        return result._value;
        };

    const double currentIncome = getOptimalIncome();

    const double buildingUpgradeIncome = getOptimalIncome(NEXT_LEVEL_BUILDING_STRENGTH, BOOST_STRENGTH);
    const double paybackPeriodBuilding = (BUILDING_UPGRADE_COST - currentCash) / currentIncome + BUILDING_UPGRADE_COST / buildingUpgradeIncome;

    const double boostUpgradeIncome = getOptimalIncome(BUILDING_STRENGTH, NEXT_LEVEL_BOOST_STRENGTH);
    const double paybackPeriodBoost = (BOOST_UPGRADE_COST - currentCash) / currentIncome + BOOST_UPGRADE_COST / boostUpgradeIncome;

    return paybackPeriodBuilding < paybackPeriodBoost ? building : boost;
}

// As above, but for battle components. You don't upgrade elements with battle components, so the "cost to afford" here is the amount of battle
// components you could have produced in the time it takes to afford the upgrade using an optimal building materials setup.
// Does not look more than 1 upgrade ahead.
Base1::elemType Base1::optimalUpgradeBattleComponents(double currentCash) const {
    vector<vector<elemType>> workingVector = _grid;
    auto getOptimalIncome = [this, &workingVector](elemType type, double buildingStrength = BUILDING_STRENGTH, double boostStrength = BOOST_STRENGTH, double battleInput = BATTLE_INPUT, double battleStrength = BATTLE_STRENGTH) {
        OptimizationResult result(_grid);
        if (type == building)
            maxBuildingMaterials(0, 0, workingVector, result, buildingStrength, boostStrength);
        else if (type == battle)
            maxBattleComponents(0, 0, workingVector, result, buildingStrength, boostStrength, battleInput, battleStrength);
        return result._value;
        };

    const double currentIncome = getOptimalIncome(building);
    const double currentBattleIncome = getOptimalIncome(battle);

    const double buildingUpgradeBattleIncome = getOptimalIncome(battle, NEXT_LEVEL_BUILDING_STRENGTH, BOOST_STRENGTH, BATTLE_INPUT, BATTLE_STRENGTH);
    const double boostUpgradeBattleIncome = getOptimalIncome(battle, BUILDING_STRENGTH, NEXT_LEVEL_BOOST_STRENGTH, BATTLE_INPUT, BATTLE_STRENGTH);
    const double battleUpgradeBattleIncome = getOptimalIncome(battle, BUILDING_STRENGTH, BOOST_STRENGTH, NEXT_LEVEL_BATTLE_INPUT, NEXT_LEVEL_BATTLE_STRENGTH);

    const double affordTimeBuilding = (BUILDING_UPGRADE_COST - currentCash) / currentIncome;
    const double affordTimeBoost = (BOOST_UPGRADE_COST - currentCash) / currentIncome;
    const double affordTimeBattle = (BATTLE_UPGRADE_COST - currentCash) / currentIncome;

    const double opportunityCostBuilding = affordTimeBuilding * currentBattleIncome;
    const double opportunityCostBoost = affordTimeBoost * currentBattleIncome;
    const double opportunityCostBattle = affordTimeBattle * currentBattleIncome;

    const double paybackPeriodBuilding = affordTimeBuilding + (opportunityCostBuilding / buildingUpgradeBattleIncome);
    const double paybackPeriodBoost = affordTimeBoost + (opportunityCostBoost / boostUpgradeBattleIncome);
    const double paybackPeriodBattle = affordTimeBattle + (opportunityCostBattle / battleUpgradeBattleIncome);

    if (paybackPeriodBuilding < paybackPeriodBoost && paybackPeriodBuilding < paybackPeriodBattle)
        return building;
    if (paybackPeriodBoost < paybackPeriodBuilding && paybackPeriodBoost < paybackPeriodBattle)
        return boost;
    return battle;
}