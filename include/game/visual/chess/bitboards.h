#ifndef BITBOARDS_H
#define BITBOARDS_H

// The basic chess logic. See README.md for attribution (to myself).

#include <string>
#include <unordered_map>
#include <vector>
#include <engine/log.h>

#define U64 unsigned long long int
typedef enum { pawn, knight, bishop, rook, queen, king } piece;
#define WHITE 0
#define BLACK 1
#define PIECE_MAP "PpNnBbRrQqKk"
typedef enum square_e {
    h1, g1, f1, e1, d1, c1, b1, a1, 
	h2, g2, f2, e2, d2, c2, b2, a2, 
	h3, g3, f3, e3, d3, c3, b3, a3, 
	h4, g4, f4, e4, d4, c4, b4, a4, 
	h5, g5, f5, e5, d5, c5, b5, a5, 
	h6, g6, f6, e6, d6, c6, b6, a6, 
	h7, g7, f7, e7, d7, c7, b7, a7, 
	h8, g8, f8, e8, d8, c8, b8, a8
} square;
const std::vector<std::string> boardStringMap = {
    "h1", "g1", "f1", "e1", "d1", "c1", "b1", "a1", "h2", "g2", "f2",
    "e2", "d2", "c2", "b2", "a2", "h3", "g3", "f3", "e3", "d3", "c3",
    "b3", "a3", "h4", "g4", "f4", "e4", "d4", "c4", "b4", "a4", "h5",
    "g5", "f5", "e5", "d5", "c5", "b5", "a5", "h6", "g6", "f6", "e6",
    "d6", "c6", "b6", "a6", "h7", "g7", "f7", "e7", "d7", "c7", "b7",
    "a7", "h8", "g8", "f8", "e8", "d8", "c8", "b8", "a8",
};
const std::unordered_map<std::string, square> stringBoardMap = {
	{"h1", h1}, {"g1", g1}, {"f1", f1}, {"e1", e1}, {"d1", d1}, {"c1", c1}, {"b1", b1}, {"a1", a1}, 
	{"h2", h2}, {"g2", g2}, {"f2", f2}, {"e2", e2}, {"d2", d2}, {"c2", c2}, {"b2", b2}, {"a2", a2}, 
	{"h3", h3}, {"g3", g3}, {"f3", f3}, {"e3", e3}, {"d3", d3}, {"c3", c3}, {"b3", b3}, {"a3", a3}, 
	{"h4", h4}, {"g4", g4}, {"f4", f4}, {"e4", e4}, {"d4", d4}, {"c4", c4}, {"b4", b4}, {"a4", a4}, 
	{"h5", h5}, {"g5", g5}, {"f5", f5}, {"e5", e5}, {"d5", d5}, {"c5", c5}, {"b5", b5}, {"a5", a5}, 
	{"h6", h6}, {"g6", g6}, {"f6", f6}, {"e6", e6}, {"d6", d6}, {"c6", c6}, {"b6", b6}, {"a6", a6}, 
	{"h7", h7}, {"g7", g7}, {"f7", f7}, {"e7", e7}, {"d7", d7}, {"c7", c7}, {"b7", b7}, {"a7", a7}, 
	{"h8", h8}, {"g8", g8}, {"f8", f8}, {"e8", e8}, {"d8", d8}, {"c8", c8}, {"b8", b8}, {"a8", a8}
};
const std::vector<std::string> pieceStringMap = {"p", "n", "b", "r", "q", "k"};

// Converts bb w/ 1 piece to its square (counts trailing zeros w/ GCC builtin)
square bbToSq(U64 bb);
struct Move {
	U64 source_bb;
	U64 dest_bb;
	piece moved_piece;
	piece promote_to; // pawn if not promoting
};
piece promoteToPiece(char input);
Move stringToMove(std::string s);
std::string moveToString(Move m);

class BoardState {
public:
	BoardState() {resetGame();};
    ~BoardState() = default;

	const U64* getPieceBoards() const {
        return piece_bb;
    }

	const U64* getColorBoards() const {
        return color_bb;
    }

	const U64& getAllBoard() const {
        return all_bb;
    }

	// Loads the bitboards from a FEN
	void loadFromFen(std::string fen);
	// clears all bitboards, resets whose_turn and castling
	void resetGame();

	void makeMove(Move m);

private:
	// The fifteen bitboards - one per piece, plus two for each color, plus one for all pieces
	U64 piece_bb[12];     // The pairs of boards for each piece
    U64 color_bb[2];      // The pair of color boards
    U64 all_bb;           // The board for all pieces
    int whose_turn;       // 0 = white, 1 = black
    int castling;         // (0b0000 or nonzero, in order WKQWbkbq)
    U64 en_passant;       // If last move was double pawn push, keep track of
                          // en-passant
	// which pieces the player controls
	bool controlsWhite = true;
};

#endif