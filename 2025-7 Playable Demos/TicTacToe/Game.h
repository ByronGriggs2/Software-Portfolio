#pragma once
#include <memory>
#include "AI.h"
#include "GameBoard.h"

class Game final
{
	std::unique_ptr<GameBoard> _board;
	AI _ai;

	char _AIChar = 'i';
	char _playerChar = 'i';

	char charPrompt() const;
	void setPlayerChar(char);
	void playerMovePrompt();
public:
	Game() : _board(std::make_unique<GameBoard>()), _ai(AI(_board.get())) {}

	bool startNewGame();
};

