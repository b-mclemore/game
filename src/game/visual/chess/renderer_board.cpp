#include <game/visual/chess/renderer_board.h>
#include <engine/log.h>

void BoardRenderer::loadTextures() {
	pieceTex = ResourceManager::getTexture("pieces");
	squareTex = ResourceManager::getTexture("squares");
}

void BoardRenderer::drawBoard(const BoardState& bs, int centerX, int centerY) {
    shader.use();

	// first, draw squares
	float x = centerX - (renderGlyphWidth * 4);
	float y = centerY - (renderGlyphHeight * 4);
	auto texture = squareTex;
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			setUV(0, (i + j) % 2, texture);
			drawAtlasSprite(
				texture,
				Vector2(x, y),
				Vector2(renderGlyphWidth, renderGlyphHeight)
			);
			x += renderGlyphWidth;
		}
		y += renderGlyphHeight;
        x = centerX - (renderGlyphWidth * 4);
	}

	// next, draw pieces
	x = centerX - (renderGlyphWidth * 4);
	y = centerY - (renderGlyphHeight * 4);
    U64 pos = (U64)1 << 63;
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (pos & bs.getAllBoard()) {
                // Find which piece
                for (int pc_idx = 0; pc_idx < 12; pc_idx++) {
                    if (pos & bs.getPieceBoards()[pc_idx]) {
                        drawPiece(pos, pc_idx, x, y);
                        break;
                    }
                }
            }
            pos >>= 1;
        }
    }
}

void BoardRenderer::drawPiece(U64 pos, int pc_idx, int bottomX, int bottomY) {
    // black pieces are the top half of the texture
    int rowOffset = (pc_idx % 2 ? 2 : 0);
    // pieces are:
    // pawn, knight, bishop, rook
    // queen, king, empty, empty
    int col = ((pc_idx / 2) % 4);
    int row = (pc_idx < 8 ? 0 : 1) + rowOffset;
    setUV(row, col, pieceTex);
    // get coords from bb
    square sq = bbToSq(pos);
    auto x = 7 - (sq % 8);
    auto y = sq / 8;
    drawAtlasSprite(
        pieceTex,
        Vector2(bottomX + x * renderGlyphWidth, bottomY + y * renderGlyphHeight),
        Vector2(renderGlyphWidth, renderGlyphHeight)
    );
}

void BoardRenderer::setBoardSize(int size) {
    renderGlyphWidth = size;
    renderGlyphHeight = size;
}