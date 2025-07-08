#include "AI.h"
#include "GameBoard.h"
#include <cassert>

void AI::AIMove() {
	int rowToPlay, colToPlay;
	bool moveFound = false;
	// Win if possible
	moveFound = isAboutToWin(_AIChar, rowToPlay, colToPlay, *_board);
	// Block player from win if necessary
	if (!moveFound)
		moveFound = isAboutToWin(_playerChar, rowToPlay, colToPlay, *_board);
	// Fork if possible
	if (!moveFound)
		moveFound = isForkPossible(_AIChar, rowToPlay, colToPlay, *_board);
	// Block player fork if needed
	if (!moveFound)
		moveFound = isBlockForkNeeded(rowToPlay, colToPlay);
	// Play center
	if (!moveFound)
		moveFound = isCenterPossible(rowToPlay, colToPlay);
	// Play opposite corner from opponent
	if (!moveFound)
		moveFound = isOppositePossible(rowToPlay, colToPlay);
	// Play any corner
	if (!moveFound)
		moveFound = isEmptyPossible(rowToPlay, colToPlay);
	// Play any side
	if (!moveFound)
		findSide(rowToPlay, colToPlay);

	// Execute planned action
	_board->makeMove(_AIChar, rowToPlay, colToPlay);
}

bool AI::isAboutToWin(char winningChar, int& row, int& col, const GameBoard& board) {
	for (int rowCandidate = 0; rowCandidate < HEIGHT; rowCandidate++) {
		for (int colCandidate = 0; colCandidate < WIDTH; colCandidate++) {
			if (board.getFeature(rowCandidate, colCandidate) != ' ')
				continue;
			GameBoard predictiveBoard = board;
			predictiveBoard.makeMove(winningChar, rowCandidate, colCandidate);
			if (predictiveBoard.checkForWin(winningChar)) {
				row = rowCandidate;
				col = colCandidate;
				return true;
			}
		}
	}
	return false;
}

bool AI::isForkPossible(char forkingChar, int& row, int& col, const GameBoard& board) {
	for (int rowCandidate = 0; rowCandidate < HEIGHT; rowCandidate++) {
		for (int colCandidate = 0; colCandidate < WIDTH; colCandidate++) {
			if (board.getFeature(rowCandidate, colCandidate) != ' ')
				continue;
			GameBoard predictiveBoard = board;
			predictiveBoard.makeMove(forkingChar, rowCandidate, colCandidate);
			int forcedRow;
			int forcedCol;
			if (!isAboutToWin(forkingChar, forcedRow, forcedCol, predictiveBoard))
				continue;
			char otherChar = forkingChar == 'X' ? 'O' : 'X';
			predictiveBoard.makeMove(otherChar, forcedRow, forcedCol);
			if (isAboutToWin(forkingChar, forcedRow, forcedCol, predictiveBoard)) {
				row = rowCandidate;
				col = colCandidate;
				return true;
			}
		}
	}
	return false;
}

bool AI::isBlockForkNeeded(int& row, int& col) const{
	std::vector<Position> possibleForks;
	{
		GameBoard predictiveBoard = *_board;
		bool done = false;
		while (!done) {
			done = true;
			Position tempPos;
			if (isForkPossible(_playerChar, tempPos.row, tempPos.col, predictiveBoard)) {
				done = false;
				possibleForks.push_back(tempPos);
				predictiveBoard.makeMove(_AIChar, tempPos.row, tempPos.col);
			}
		}
	}
	if (possibleForks.size() == 0)
		return false;
	if (possibleForks.size() == 1) {
		row = possibleForks[0].row;
		col = possibleForks[0].col;
		return true;
	}
	if (isPossibleToBlockAllForksWithRow(possibleForks, row, col)) {
		return true;
	}
	findRowThatDoesntProduceFork(row, col);
	return true;
}

bool AI::isPossibleToBlockAllForksWithRow(const std::vector<Position>& possibleForks, int& row, int& col) const {
	for (int rowCandidate = 0; rowCandidate < HEIGHT; rowCandidate++) {
		for (int colCandidate = 0; colCandidate < WIDTH; colCandidate++) {
			if (_board->getFeature(rowCandidate, colCandidate) != ' ' || !isCreatingRow(rowCandidate, colCandidate))
				continue;
			GameBoard predictiveBoard = *_board;
			predictiveBoard.makeMove(_AIChar, rowCandidate, colCandidate);
			int unusedInt1, unusedInt2;
			if (isForkPossible(_playerChar, unusedInt1, unusedInt2, predictiveBoard))
				continue;
			row = rowCandidate;
			col = colCandidate;
			return true;
		}
	}
	return false;
}

void AI::findRowThatDoesntProduceFork(int& row, int& col) const {
	for (int rowCandidate = 0; rowCandidate < HEIGHT; rowCandidate++) {
		for (int colCandidate = 0; colCandidate < WIDTH; colCandidate++) {
			if (_board->getFeature(rowCandidate, colCandidate) != ' ' || !isCreatingRow(rowCandidate, colCandidate))
				continue;
			// For a given AI move that gives the player 1 move that doesn't lose in 1 turn
			GameBoard predictiveBoard = *_board;
			predictiveBoard.makeMove(_AIChar, rowCandidate, colCandidate);
			int forcedRow, forcedCol;
			// When the player makes the nonlosing move
			isAboutToWin(_AIChar, forcedRow, forcedCol, predictiveBoard);
			predictiveBoard.makeMove(_playerChar, forcedRow, forcedCol);
			// Does this give the player a fork?
			{
				// Can they win if you do nothing?
				int winningRow, winningCol;
				const bool winPath1 = isAboutToWin(_playerChar, winningRow, winningCol, predictiveBoard);
				// Can they win even if you take their winning spot?
				predictiveBoard.makeMove(_AIChar, winningRow, winningCol);
				const bool winPath2 = isAboutToWin(_playerChar, winningRow, winningCol, predictiveBoard);
				if (winPath1 && winPath2)
					continue;
			}
			row = rowCandidate;
			col = colCandidate;
			return;
		}
	}
	// There should always be a row that doesn't produce a fork
	assert(false && "No play found that doesn't give the player a fork");
}

// A row in this context is any row, column or diagonal containing 1 empty space and 2 of the same letter
bool AI::isCreatingRow(int row, int col) const {
	if (_board->getFeature(row, col) != ' ')
		return false;
	GameBoard predictiveBoard = *_board;
	predictiveBoard.makeMove(_AIChar, row, col);
	int unusedInt1, unusedInt2;
	return isAboutToWin(_AIChar, unusedInt1, unusedInt2, predictiveBoard);
}

bool AI::isCenterPossible(int& row, int& col) const{
	if (_board->getFeature(CENTER,CENTER) == ' ') {
		row = CENTER;
		col = CENTER;
		return true;
	}
	return false;
}

bool AI::isOppositePossible(int& row, int& col) const {
	for (int rowCandidate = 0; rowCandidate < HEIGHT; rowCandidate++) {
		for (int colCandidate = 0; colCandidate < WIDTH; colCandidate++) {
			if (rowCandidate == 1 || colCandidate == 1)
				continue;
			if (_board->getFeature(rowCandidate, colCandidate) != ' ')
				continue;
			int oppositeRow = 2 - rowCandidate;
			int oppositeCol = 2 - colCandidate;
			if (_board->getFeature(oppositeRow, oppositeCol) != _playerChar)
				continue;
			row = rowCandidate;
			col = colCandidate;
			return true;
		}
	}
	return false;
}

bool AI::isEmptyPossible(int& row, int& col) const {
	for (int rowCandidate = 0; rowCandidate < HEIGHT; rowCandidate++) {
		for (int colCandidate = 0; colCandidate < WIDTH; colCandidate++) {
			if (rowCandidate == 1 || colCandidate == 1)
				continue;
			if (_board->getFeature(rowCandidate, colCandidate) != ' ')
				continue;
			row = rowCandidate;
			col = colCandidate;
			return true;
		}
	}
	return false;
}

void AI::findSide(int& row, int& col) const{
	if (_board->getFeature(0, 1) == ' ') {
		row = 0;
		col = 1;
	}
	else if (_board->getFeature(1, 0) == ' ') {
		row = 1;
		col = 0;
	}
	else if (_board->getFeature(1, 2) == ' ') {
		row = 1;
		col = 2;
	}
	else if (_board->getFeature(2, 1) == ' ') {
		row = 2;
		col = 1;
	}
	else
		assert(false && "No acceptable move found");
}