#pragma once
#include <vector>
class GameBoard;
struct Position;
class AI
{
	struct Position {
		int row = -1;
		int col = -1;
	};
	// Owned by Game
	GameBoard* _board;
	char _AIChar = 'i';
	char _playerChar = 'i';

public:
	void AIMove();
private:
	static bool isAboutToWin(char winningChar, int& row, int& col, const GameBoard& board);
	static bool isForkPossible(char forkingChar, int& row, int& col, const GameBoard& board);
	bool isBlockForkNeeded(int& row, int& col) const;
	bool isCenterPossible(int& row, int& col) const;
	bool isOppositePossible(int& row, int& col) const;
	bool isEmptyPossible(int& row, int& col) const;
	void findSide(int& row, int& col) const;

	bool isPossibleToBlockAllForksWithRow(const std::vector<Position>&, int& row, int& col) const;
	bool isCreatingRow(int row, int col) const;
	void findRowThatDoesntProduceFork(int& row, int& col) const;
public:
	AI() = delete;
	AI(GameBoard* board) : _board(board) {}
	void setPlayerChar(char c) {
		_playerChar = c;
		_AIChar = c == 'X' ? 'O' : 'X';
	}
};

