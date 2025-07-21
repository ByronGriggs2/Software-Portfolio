#include <iostream>
#include <sstream>
#include "Base1.h"

float MULTIPLIER_STRENGTH;

using std::cout, std::cin, std::endl, std::vector, std::string;
using elemType = Base1::elemType;

void clearConsole();

int main()
{
	Base1 base1;
	while (true){
		clearConsole();
		cout << "Optimal base1 configuration for building" << endl;
		vector<vector<elemType>> grid = base1.optimizeBuildingMats();
		Base::printGrid(grid);
		cout << endl;

		cout << "Optimal base1 configuration for battle" << endl;
		grid = base1.optimizeBattleComponents();
		Base::printGrid(grid);
		cout << endl;

		cout << "To find optimal next upgrade for building mats, enter 'B' followed by current building mats" << endl;
		cout << "To find optimal next upgrade for battle components, enter 'C' followed by current building mats" << endl;
		cout << "To exit, enter 'Q'" << endl;
		cout << "Example: B 3.1415e7" << endl;
		string line;
		char option;
		double currentCash;
		cin >> option;
		cin >> currentCash;

		if (option == 'Q') {
			return 0;
		}
		if (option == 'B') {
			elemType temp = base1.optimalUpgradeBuildingMats(currentCash);
			cout << "You should upgrade ";
			Base::printEnum(temp);
			cout << " next. Enter any character to reset." << endl;
			cin.clear();
			cin.ignore(100, '\n');
			cin >> option;
			cin.clear();
			cin.ignore(100, '\n');
			continue;
		}
		else if (option == 'C') {
			elemType temp = base1.optimalUpgradeBattleComponents(currentCash);
			cout << "You should upgrade ";
			Base::printEnum(temp);
			cout << " next. Enter any character to reset." << endl;
			cin.clear();
			cin.ignore(100, '\n');
			cin >> option;
			cin.clear();
			cin.ignore(100, '\n');
			continue;
		}
	}
}

void clearConsole() {
#ifdef TEST
	return;
#endif
#ifdef _WIN32
	system("CLS");
#else
	system("clear");
#endif
}

