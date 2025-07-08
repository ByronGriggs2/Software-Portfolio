#include "GameBoard.h"
#include <cassert>
#include <iostream>
using std::cout, std::endl;

GameBoard::GameBoard(const GameBoard& other) {
	for (int row = 0; row < HEIGHT; row++) {
		for (int col = 0; col < WIDTH; col++) {
			_board[row][col] = other._board[row][col];
		}
	}
}
GameBoard::GameBoard(GameBoard&& other) noexcept {
	for (int row = 0; row < HEIGHT; row++) {
		for (int col = 0; col < WIDTH; col++) {
			_board[row][col] = std::move(other._board[row][col]);
		}
	}
}
GameBoard& GameBoard::operator=(const GameBoard& other) {
	if (this == &other)
		return *this;
	for (int row = 0; row < HEIGHT; row++) {
		for (int col = 0; col < WIDTH; col++) {
			_board[row][col] = other._board[row][col];
		}
	}
	return *this;
}
GameBoard& GameBoard::operator=(GameBoard&& other) noexcept {
	if (this == &other)
		return *this;
	for (int row = 0; row < HEIGHT; row++) {
		for (int col = 0; col < WIDTH; col++) {
			_board[row][col] = std::move(other._board[row][col]);
		}
	}
	return *this;
}

void GameBoard::resetGame() {
	for (auto& row : _board)
		for (auto& element : row)
			element = ' ';
}
void GameBoard::makeMove(char type, int row, int col) {
	if (getFeature(row, col) != ' ')
		assert(false && "Invalid move");
	_board[row][col] = type;
}
void GameBoard::displayBoard() const {
	cout << "    1   2   3" << endl;
	cout << "  +---+---+---+" << endl;
	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < HEIGHT; j++) {
			if (j == 0)
				cout << (i+1) << " ";
			cout << "| " << _board[i][j] << " ";
		}
		cout << "|" << endl;
		cout << "  +---+---+---+" << endl;
	}
}
bool GameBoard::checkForWin(char winChar) const {
	for (int row = 0; row < HEIGHT; row++) {
		for (int col = 0; col < WIDTH; col++) {
			if (_board[row][col] != winChar)
				break;
			if (col == WIDTH-1)
				return true;
		}
	}
	for (int col = 0; col < WIDTH; col++) {
		for (int row = 0; row < HEIGHT; row++) {
			if (_board[row][col] != winChar)
				break;
			if (row == HEIGHT-1)
				return true;
		}
	}
	for (int index = 0; index < WIDTH; index++) {
		if (_board[index][index] != winChar)
			break;
		if (index == WIDTH-1)
			return true;
	}
	for (int index = 0; index < WIDTH; index++) {
		if (_board[index][WIDTH - index - 1] != winChar)
			break;
		if (index == WIDTH-1)
			return true;
	}
	return false;
}
bool GameBoard::checkIfFull() const {
	for (int row = 0; row < HEIGHT; row++) {
		for (int col = 0; col < WIDTH; col++)
			if (_board[row][col] == ' ')
				return false;
	}
	return true;
}