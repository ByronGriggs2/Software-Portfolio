#include "Game.h"
#include "GameBoard.h"
#include "helpers.h"
#include <sstream>
#include <cassert>

using std::cout, std::cin, std::endl, std::string;
using helpers::clearConsole;

bool Game::startNewGame() {
	_board->resetGame();
	clearConsole();
	char playerChar = charPrompt();
	setPlayerChar(playerChar);
	
	bool AIWin = false;
	bool tie = false;
	bool currentlyAITurn = playerChar == 'O';
	while (!AIWin && !tie) {
		clearConsole();
		if (currentlyAITurn) {
			_ai.AIMove();
		}
		else {
			playerMovePrompt();
		}
		AIWin = _board->checkForWin(_AIChar);
		tie = _board->checkIfFull() && !AIWin;
		// AI should never lose
		if (_board->checkForWin(_playerChar))
			assert(false && "The AI lost a game!");
		currentlyAITurn = !currentlyAITurn; 
	}
	clearConsole();
#ifndef TEST
	if (AIWin)
		cout << "AI wins!" << endl;
	else
		cout << "It's a tie!" << endl;
	_board->displayBoard();
#endif
	if (AIWin)
		return true;
	return false;
}

char Game::charPrompt() const{
#ifdef TEST
	return rand() % 2 == 0 ? 'X' : 'O';
#endif

	char parsedChar;
	while (true) {
		cout << "X or O?" << endl;
		string playerInput;
		getline(cin, playerInput);
		if (playerInput.length() >= 1) {
			if (playerInput[0] == 'X' || playerInput[0] == 'x') {
				parsedChar = 'X';
				break;
			}
			else if (playerInput[0] == 'O' || playerInput[0] == 'o') {
				parsedChar = 'O';
				break;
			}
		}
		clearConsole();
		cout << "Please try again. ";
	}
	return parsedChar;
}

void Game::setPlayerChar(char player) {
	_playerChar = player;
	_AIChar = player == 'X' ? 'O' : 'X';
	_ai.setPlayerChar(player);
}

void Game::playerMovePrompt() {
#ifdef TEST
	while (true) {
		int row = rand() % HEIGHT;
		int col = rand() % WIDTH;
		if (_board->getFeature(row, col) == ' ') {
			_board->makeMove(_playerChar, row, col);
			return;
		}
	}
#endif
	int row, col;
	string playerInput;
	while (true) {
		_board->displayBoard();
		cout << "Enter a position as \"<row> <column>\"" << endl;
		getline(cin, playerInput);
		std::istringstream iss(playerInput);
		if (
			// less than 2 words
			!(iss >> row >> col) ||
			// more than 2 words
			(iss >> std::ws && !iss.eof()) ||
			// out of bounds
			row < 1 || row > HEIGHT || col < 1 || col > WIDTH ||
			// not empty
			_board->getFeature(row-1, col-1) != ' '
			) {
			clearConsole();
			cout << "Please try again." << endl;
			continue;
		}
		break;
	}
	_board->makeMove(_playerChar, row - 1, col - 1);
}

