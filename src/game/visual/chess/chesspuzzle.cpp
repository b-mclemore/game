#include <game/visual/chess/chesspuzzle.h>
#include <engine/log.h>
#include <fstream>
#include <iostream>
#include <string>

const BoardState& ChessPuzzle::getBoardState() {
    return bbs;
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

    bbs.loadFromFen(fen);
	// TODO
	// movelist.loadFromString(moves);
}