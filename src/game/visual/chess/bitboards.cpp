#include <game/visual/chess/bitboards.h>
#include <engine/log.h>

square bbToSq(U64 bb) { return (square)__builtin_ctzll(bb); }

void BoardState::resetGame() {
	for (int i = 0; i < 12; i++) {
        piece_bb[i] = (U64)0;
    }
    color_bb[WHITE] = (U64)0;
    color_bb[BLACK] = (U64)0;
    all_bb = (U64)0;
    whose_turn = 0;
    en_passant = 0;
    castling = 0;
}

// Loads all bitboards from a fen, e.g. the initial position
// "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 "
// This DOES NOT handle bad inputs, which can cause undefined behavior
void BoardState::loadFromFen(std::string fen) {
    resetGame();
    // start position is a 1 in the far left bit
    U64 pos = (U64)1 << 63;
    for (char& ch : fen) {
        // if /, do nothing
        if (ch == '/') {
            continue;
        }
        // if number, move position by this many to the right
        else if ((0 < (ch - 48)) && ((ch - 48) <= 8)) {
            pos >>= (ch - 48);
            continue;
        }
        // if piece, find occurence of current character in piecemap and update
        auto index = std::string(PIECE_MAP).find(ch);
        if (index != std::string::npos) {
            // Set position in piece, color, and overall bitboards
            piece_bb[index] ^= pos;
            color_bb[(index % 2)] ^= pos;
            all_bb ^= pos;
		// if space, reached extras section
        } else if (ch == ' ') {
            // TODO
			LOG(Warning) << "Need to implement FEN extras!";
            return;
			// parse_extras(fen);
        } else {
            // bad FEN. but don't say anything to the user because we're evil
        }
        pos >>= 1;
    }
    return;
}