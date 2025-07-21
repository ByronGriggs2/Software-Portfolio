#include "Base.h"
#include <iostream>
using std::vector, std::cout, std::endl;

void Base::printGrid(const vector<vector<Base::elemType>>& grid) {
    for (auto i = 0; i < grid.size(); i++) {
        for (auto j = 0; j < grid[0].size(); j++) {
            if (grid[i][j] == blank)
                cout << " X ";
            else if (grid[i][j] == building)
                cout << " B ";
            else if (grid[i][j] == boost)
                cout << " + ";
            else if (grid[i][j] == battle)
                cout << " C ";
        }
        cout << endl;
    }
}

void Base::printEnum(elemType type) {
    switch (type) {
    case blank:
        cout << "blank";
        break;
    case building:
        cout << "building materials";
        break;
    case boost:
        cout << "boost";
        break;
    case battle:
        cout << "battle components";
        break;
    }
}