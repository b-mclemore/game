#ifndef CHESSPUZZLE_H
#define CHESSPUZZLE_H

#include <vector>
#include <string>
#include <game/visual/chess/bitboards.h>

class ChessPuzzle {
public:
	ChessPuzzle() = default;
    ~ChessPuzzle() = default;

	// Loads the fen and movelist from a text file
	// File should be fen on first line, movelist on second
	void loadFromFile(std::string path);
	// Takes space separated moves e.g. "e2e4 d2d4 ..."
	void loadMoves(std::string moves);

	const BoardState& getBoardState();

	void makeMove(Move m);

	// for debugging
	std::string getNextMove();
	
private:
	// the initial fen used to generate this chess puzzle
	std::string FEN;
	// the moves required to solve the puzzle
	// moves are a pair source, dest
	std::vector<Move> moveList = {};
	size_t currMoveIdx = 0;
	// the bitboards holding the current state of the board
	BoardState bbs;
};

#endif