#ifndef CHESSPUZZLE_H
#define CHESSPUZZLE_H

#include <string>
#include <game/visual/chess/bitboards.h>

class ChessPuzzle {
public:
	ChessPuzzle() = default;
    ~ChessPuzzle() = default;

	// Loads the fen and movelist from a text file
	// File should be fen on first line, movelist on second
	void loadFromFile(std::string path);

	const BoardState& getBoardState();
	
private:
	// the initial fen used to generate this chess puzzle
	std::string FEN;
	// the moves required to solve the puzzle
	// TODO
	// the bitboards holding the current state of the board
	BoardState bbs;
};

#endif