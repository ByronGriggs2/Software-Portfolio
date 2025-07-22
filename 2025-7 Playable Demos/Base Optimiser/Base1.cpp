#include "Base1.h"
#ifdef _DEBUG
#include <iostream>
#endif

using std::vector;

Base1::Base1(int buildingLevel, int boostLevel, int battleLevel) :
    _buildingStrength(0.8 * pow(1.25, buildingLevel) * BUILDING_MAT_MULTIPLIER),
    _buildingCost(809.41085281 * pow(1.50387596899,buildingLevel)),
    _boostStrength(1.27951459663 * pow(1.0569726055, boostLevel)),
    _boostCost(100 * pow(10.0, boostLevel + 1)),
    _battleInput(1.2 * pow(1.25, battleLevel)),
    _battleStrength(0.5 * pow(1.3, battleLevel - 1) * BATTLE_COMP_MULTIPLIER),
    _battleCost(1668.75* pow(1.5, battleLevel)),
    _nextBuildingStrength(0.8 * pow(1.25, buildingLevel + 1) * BUILDING_MAT_MULTIPLIER),
    _nextBoostStrength(1.27951459663 * pow(1.0569726055, boostLevel + 1)),
    _nextBattleInput(1.2 * pow(1.25, battleLevel + 1)),
    _nextBattleStrength(0.5 * pow(1.3, battleLevel) * BATTLE_COMP_MULTIPLIER)
{
    // These are either holes in the grid or tiles the player hasn't bought yet. Haven't yet implemented a way for the user to specify what tiles they have bought, these are just for me and my current game
    _grid = vector<vector<elemType>>(HEIGHT, std::vector<elemType>(WIDTH, building));
    _grid[0][2] = blank;
    _grid[2][3] = blank;
    _grid[3][1] = blank;
    _grid[3][2] = blank;
    _grid[3][3] = blank;
}

vector<vector<Base1::elemType>> Base1::optimizeBuildingMats() const {
    OptimizationResult result(_grid);
    vector<vector<elemType>> workingVector = _grid;
    maxBuildingMaterials(0, 0, workingVector, result, _buildingStrength, _boostStrength);
#ifdef _DEBUG
    std::cout << "Optimal output: " << result._value << std::endl;
#endif
    return result._optimizedGrid;
}

vector<vector<Base1::elemType>> Base1::optimizeBattleComponents() const {
    OptimizationResult result(_grid);
    vector<vector<elemType>> workingVector = _grid;
    maxBattleComponents(0, 0, workingVector, result, _buildingStrength, _boostStrength, _battleInput, _battleStrength);
#ifdef _DEBUG
    std::cout << "Optimal output: " << result._value << std::endl;
#endif
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
    auto getOptimalIncome = [this, &workingVector](double buildingStrength, double boostStrength) {
        OptimizationResult result(_grid);
        maxBuildingMaterials(0, 0, workingVector, result, buildingStrength, boostStrength);
        return result._value;
        };

    const double currentIncome = getOptimalIncome(_buildingStrength, _boostStrength);

    const double buildingUpgradeIncome = getOptimalIncome(_nextBuildingStrength, _boostStrength);
    const double paybackPeriodBuilding = (_buildingCost - currentCash) / currentIncome + _buildingCost / buildingUpgradeIncome;

    const double boostUpgradeIncome = getOptimalIncome(_buildingStrength, _nextBoostStrength);
    const double paybackPeriodBoost = (_boostCost - currentCash) / currentIncome + _boostCost / boostUpgradeIncome;
#ifdef _DEBUG
    std::cout << "New income will be" << (buildingUpgradeIncome > boostUpgradeIncome ? buildingUpgradeIncome : boostUpgradeIncome) << std::endl;
#endif

    return paybackPeriodBuilding < paybackPeriodBoost ? building : boost;
}

// As above, but for battle components. You don't upgrade elements with battle components, so the "cost to afford" here is the amount of battle
// components you could have produced in the time it takes to afford the upgrade using an optimal building materials setup.
// Does not look more than 1 upgrade ahead.
Base1::elemType Base1::optimalUpgradeBattleComponents(double currentCash) const {
    vector<vector<elemType>> workingVector = _grid;
    auto getOptimalIncome = [this, &workingVector](elemType type, double buildingStrength, double boostStrength, double battleInput, double battleStrength) {
        OptimizationResult result(_grid);
        if (type == building)
            maxBuildingMaterials(0, 0, workingVector, result, buildingStrength, boostStrength);
        else if (type == battle)
            maxBattleComponents(0, 0, workingVector, result, buildingStrength, boostStrength, battleInput, battleStrength);
        return result._value;
        };

    const double currentIncome = getOptimalIncome(building, _buildingStrength, _boostStrength, _battleInput, _battleStrength);
    const double currentBattleIncome = getOptimalIncome(battle, _buildingStrength, _boostStrength, _battleInput, _battleStrength);

    const double buildingUpgradeBattleIncome = getOptimalIncome(battle, _nextBuildingStrength, _boostStrength, _battleInput, _battleStrength);
    const double boostUpgradeBattleIncome = getOptimalIncome(battle, _buildingStrength, _nextBoostStrength, _battleInput, _battleStrength);
    const double battleUpgradeBattleIncome = getOptimalIncome(battle, _buildingStrength, _boostStrength, _nextBattleInput, _nextBattleStrength);

    const double affordTimeBuilding = (_buildingCost - currentCash) / currentIncome;
    const double affordTimeBoost = (_boostCost - currentCash) / currentIncome;
    const double affordTimeBattle = (_battleCost - currentCash) / currentIncome;

    const double opportunityCostBuilding = affordTimeBuilding * currentBattleIncome;
    const double opportunityCostBoost = affordTimeBoost * currentBattleIncome;
    const double opportunityCostBattle = affordTimeBattle * currentBattleIncome;

    const double paybackPeriodBuilding = affordTimeBuilding + (opportunityCostBuilding / buildingUpgradeBattleIncome);
    const double paybackPeriodBoost = affordTimeBoost + (opportunityCostBoost / boostUpgradeBattleIncome);
    const double paybackPeriodBattle = affordTimeBattle + (opportunityCostBattle / battleUpgradeBattleIncome);

    if (paybackPeriodBuilding < paybackPeriodBoost && paybackPeriodBuilding < paybackPeriodBattle) {
#ifdef _DEBUG
        std::cout << "New income will be " << buildingUpgradeBattleIncome << std::endl;
#endif
        return building;
    }
    if (paybackPeriodBoost < paybackPeriodBuilding && paybackPeriodBoost < paybackPeriodBattle) {
#ifdef _DEBUG
        std::cout << "New income will be " << boostUpgradeBattleIncome << std::endl;
#endif
        return boost;
    }
#ifdef _DEBUG
    std::cout << "New income will be " << battleUpgradeBattleIncome << std::endl;
#endif
    return battle;
}