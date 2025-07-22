#include <iostream>
#include <sstream>
#include "Base1.h"
#include <thread>
#include <chrono>

using std::cout, std::cin, std::endl, std::vector, std::string;
using elemType = Base1::elemType;

void clearConsole();
bool myGetLine(const string& prompt, string& result);

int main()
{
	while (true){
		if (cin.fail()) {
			cin.clear();
			cin.ignore(100, '\n');
		}
		clearConsole();

		cout << "Note: For some reason, the Windows command prompt can freeze if you unfocus the prompt while the game is running, even with no app running on it. For best results, memorise/write down the numbers you need before launching the program" << endl << endl;
		int build, boost, battle;
		{
			string line;
			cout << "Enter level of building materials, booster, and battle components." << endl;
			cout << "Example: 4 6 1" << endl;;
			getline(cin, line);
			std::istringstream iss(line);
			if (!(iss >> build) || !(iss >> boost) || !(iss >> battle)) {
				continue;
			}
		}
		Base1 base1(build, boost, battle);

		cout << endl << "Optimal base1 configuration for building:" << endl;
		vector<vector<elemType>> grid = base1.optimizeBuildingMats();
		Base::printGrid(grid);
		cout << endl;

		cout << "Optimal base1 configuration for battle:" << endl;
		grid = base1.optimizeBattleComponents();
		Base::printGrid(grid);
		cout << endl;

		char option;
		double currentCash;
		{
			cout << "To find optimal next upgrade for building mats, enter 'B' followed by current building mats." << endl;
			cout << "To find optimal next upgrade for battle components, enter 'C' followed by current building mats." << endl;
			cout << "To exit, enter 'Q'." << endl;
			cout << "Example: B 3.1415e7" << endl;
			string line;
			getline(cin, line);
			std::istringstream iss(line);
			if (!(iss >> option)) {
				continue;
			}
			if (option == 'Q')
				return 0;
			if (!(iss >> currentCash)) {
				continue;
			}
		}
		if (option == 'B') {
			elemType temp = base1.optimalUpgradeBuildingMats(currentCash);
			cout << "You should upgrade ";
			Base::printEnum(temp);
			cout << " next. Press enter to reset." << endl;
			{
				string line;
				std::getline(cin, line);
				continue;
			}
		}
		else if (option == 'C') {
			elemType temp = base1.optimalUpgradeBattleComponents(currentCash);
			cout << "You should upgrade ";
			Base::printEnum(temp);
			cout << " next. Press enter to reset." << endl;
			{
				string line;
				std::getline(cin, line);
				continue;
			}
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

