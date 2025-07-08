#pragma once
constexpr int WIDTH = 3;
constexpr int HEIGHT = 3;
constexpr int CENTER = 1;
class GameBoard final
{
	char _board[WIDTH][HEIGHT];
public:
	GameBoard() = default;
	GameBoard(const GameBoard& other);
	GameBoard(GameBoard&& other) noexcept;
	GameBoard& operator=(const GameBoard& other);
	GameBoard& operator=(GameBoard&& other) noexcept;

	void resetGame();
	void makeMove(char, int, int);
	char getFeature(int row, int col) const { return _board[row][col]; }
	void displayBoard() const;
	bool checkForWin(char) const;
	bool checkIfFull() const;
};

