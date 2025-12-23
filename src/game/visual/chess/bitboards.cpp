#include <game/visual/chess/bitboards.h>
#include <engine/log.h>

square bbToSq(U64 bb) { return (square)__builtin_ctzll(bb); }

piece promoteToPiece(char input) {
    switch (input) {
    case 'N':
        return knight;
    case 'B':
        return bishop;
    case 'R':
        return rook;
    default:
        return queen;
    }
}

// assumes properly formatted
Move stringToMove(std::string s) {
    Move ret;
    int sourceFile = ('h' - s[0]);
    int sourceRank = (s[1] - '1');
    ret.source_bb = (U64)1 << (sourceFile + 8 * sourceRank);
    int destFile  = ('h' - s[2]);
    int destRank  = (s[3] - '1');
    ret.dest_bb = (U64)1 << (destFile + 8 * destRank);
    ret.moved_piece  = rook; // TODO
    ret.promote_to = (s.length() > 4 ? promoteToPiece(s[4]) : pawn);
    return ret;
}

std::string moveToString(Move m) {
    square sqrc = bbToSq(m.source_bb);
    square dest = bbToSq(m.dest_bb);
    std::string promote = (m.promote_to ? pieceStringMap[m.promote_to] : "");
    return boardStringMap[sqrc] + boardStringMap[dest] + promote;
}

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

void BoardState::makeMove(Move m) {
    // remove from source
    auto rev = ~m.source_bb;
    piece_bb[2 * m.moved_piece + whose_turn] &= rev;
    color_bb[whose_turn] &= rev;
    all_bb &= rev;
    // add to dest
    if (m.promote_to) {
        piece_bb[2 * m.promote_to + whose_turn] |= m.dest_bb;
    } else {
        piece_bb[2 * m.moved_piece + whose_turn] |= m.dest_bb;
    }
    color_bb[whose_turn] |= m.dest_bb;
    all_bb |= m.dest_bb;

    whose_turn = 1 - whose_turn;
}