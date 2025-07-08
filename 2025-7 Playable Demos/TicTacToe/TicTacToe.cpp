#include "Game.h"
#include "helpers.h"
#include <string>
using std::cout, std::cin, std::string, std::endl;
using helpers::clearConsole;
int main()
{
    Game game;
#ifdef TEST
    int wins = 0;
    int ties = 0;
    srand(1);
    for (int i = 0; i < 300000; i++) {
        if (i == 13)
            int r = 0;
        if (i % 100 == 0)
            cout << "Starting test " << i << endl;
        if (game.startNewGame())
            wins++;
        else
            ties++;
    }
    cout << "Wins: " << wins << endl;
    cout << "Ties: " << ties << endl;
    return 0;
#else
    game.startNewGame();
    while (true) {
        cout << "Would you like to start a new game? Y/N" << endl;
        string playerInput;
        getline(cin, playerInput);
        if (playerInput.length() != 1) {
            clearConsole();
            cout << "Please try again. ";
            continue;
        }
        if (playerInput[0] == 'Y' || playerInput[0] == 'y') {
            game.startNewGame();
            continue;
        }
        else if (playerInput[0] == 'N' || playerInput[0] == 'n')
            return 0;
        clearConsole();
        cout << "Please try again. ";
        continue;
    }
#endif
}
