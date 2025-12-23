#include <game/visual/chess/chesspuzzle.h>
#include <engine/log.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

const BoardState& ChessPuzzle::getBoardState() {
    return bbs;
}

std::string ChessPuzzle::getNextMove() {
    return moveToString(moveList[currMoveIdx]);
}

void ChessPuzzle::loadMoves(std::string moves) {
    std::istringstream iss(moves);
    std::string curr_move;

    while (iss >> curr_move) {
        Move m = stringToMove(curr_move);
        moveList.push_back(m);
    }
}

void ChessPuzzle::loadFromFile(std::string path) {
	std::ifstream file(path);

    if (!file) {
        LOG(Error) << "Failed to open file at " << path;
        exit(1);
    }

    std::string fen, moves;
    std::getline(file, fen);
    std::getline(file, moves);

    // load pieces onto board
    bbs.loadFromFen(fen);
    // load correct moves into memory
	loadMoves(moves);
}

void ChessPuzzle::makeMove(Move m) {
    bbs.makeMove(m);
    currMoveIdx++;
}