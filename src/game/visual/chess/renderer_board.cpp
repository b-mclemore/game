#include <game/visual/chess/renderer_board.h>
#include <engine/log.h>

void BoardRenderer::loadTextures() {
	pieceTex = ResourceManager::getTexture("pieces");
	squareTex = ResourceManager::getTexture("squares");
}

// void BoardRenderer::drawBoard(const BoardState& bs, int centerX, int centerY) {
//     shader.use();

// 	// first, draw squares
// 	float x = centerX - (renderGlyphWidth * 4);
// 	float y = centerY - (renderGlyphHeight * 4);
// 	auto texture = squareTex;
// 	for (int i = 0; i < 8; ++i) {
// 		for (int j = 0; j < 8; ++j) {
// 			setUV(0, 1 - ((i + j) % 2), texture);
// 			drawAtlasSprite(
// 				texture,
// 				Vector2(x, y),
// 				Vector2(renderGlyphWidth, renderGlyphHeight)
// 			);
// 			x += renderGlyphWidth;
// 		}
// 		y += renderGlyphHeight;
//         x = centerX - (renderGlyphWidth * 4);
// 	}

// 	// next, draw pieces
// 	x = centerX - (renderGlyphWidth * 4);
// 	y = centerY - (renderGlyphHeight * 4);
//     U64 pos = (U64)1 << 63;
//     for (int row = 0; row < 8; row++) {
//         for (int col = 0; col < 8; col++) {
//             if (pos & bs.getAllBoard()) {
//                 // Find which piece
//                 for (int pc_idx = 0; pc_idx < 12; pc_idx++) {
//                     if (pos & bs.getPieceBoards()[pc_idx]) {
//                         drawPiece(pos, pc_idx, x, y);
//                         break;
//                     }
//                 }
//             }
//             pos >>= 1;
//         }
//     }
// }

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

Vector2 BoardRenderer::getSquareScreenPos(U64 square_bb, int boardCenterX, int boardCenterY) {
    // Convert bitboard to square index
    square sq = bbToSq(square_bb);
    int file = 7 - (sq % 8);
    int rank = sq / 8;

    // Calculate board top-left corner
    float boardLeft = boardCenterX - (renderGlyphWidth * 4);
    float boardTop = boardCenterY - (renderGlyphHeight * 4);

    // Calculate square position
    float x = boardLeft + file * renderGlyphWidth;
    float y = boardTop + rank * renderGlyphHeight;

    return Vector2(x, y);
}

void BoardRenderer::drawPieceAt(int pc_idx, Vector2 screenPos) {
    // Calculate UV coordinates (same logic as drawPiece)
    int rowOffset = (pc_idx % 2 ? 2 : 0);  // Black pieces offset
    int col = ((pc_idx / 2) % 4);
    int row = (pc_idx < 8 ? 0 : 1) + rowOffset;

    setUV(row, col, pieceTex);
    drawAtlasSprite(
        pieceTex,
        screenPos,
        Vector2(renderGlyphWidth, renderGlyphHeight)
    );
}

void BoardRenderer::drawBoard(const BoardState& bs, int centerX, int centerY, bool showSuccess, U64 successSquare) {
    shader.use();

    // First, draw squares
    float x = centerX - (renderGlyphWidth * 4);
    float y = centerY + (renderGlyphHeight * 3);
    U64 pos = (U64)1 << 63;

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            // Check if this is the success square
            if (showSuccess && pos == successSquare) {
                // Draw green square
                setUV(0, 2, squareTex);
            } else {
                // Draw normal checkerboard pattern
                setUV(0, ((i + j) % 2), squareTex);
            }
            drawAtlasSprite(
                squareTex,
                Vector2(x, y),
                Vector2(renderGlyphWidth, renderGlyphHeight)
            );
            x += renderGlyphWidth;
            pos >>= 1;
        }
        y -= renderGlyphHeight;
        x = centerX - (renderGlyphWidth * 4);
    }

    // Next, draw pieces (using same pattern as drawBoard)
    x = centerX - (renderGlyphWidth * 4);
    y = centerY - (renderGlyphHeight * 4);
    pos = (U64)1 << 63;
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

void BoardRenderer::drawBoardWithAnimation(
    const BoardState& bs,
    int centerX, int centerY,
    bool isAnimating,
    int animPieceIdx,
    U64 sourceSquare,
    Vector2 animFrom,
    Vector2 animTo,
    float animProgress,
    bool isMoveCorrect
) {
    shader.use();

    // Draw squares (same as drawBoard)
    float x = centerX - (renderGlyphWidth * 4);
    float y = centerY - (renderGlyphHeight * 4);
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            setUV(0, 1 - ((i + j) % 2), squareTex);
            drawAtlasSprite(
                squareTex,
                Vector2(x, y),
                Vector2(renderGlyphWidth, renderGlyphHeight)
            );
            x += renderGlyphWidth;
        }
        y += renderGlyphHeight;
        x = centerX - (renderGlyphWidth * 4);
    }

    // Draw pieces, skipping the animating piece at its source
    x = centerX - (renderGlyphWidth * 4);
    y = centerY - (renderGlyphHeight * 4);
    U64 pos = (U64)1 << 63;

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (pos & bs.getAllBoard()) {
                // Skip source square during animation (piece is animating from there)
                bool isAnimSource = isAnimating && (pos == sourceSquare);

                if (!isAnimSource) {
                    // Find and draw piece normally
                    for (int pc_idx = 0; pc_idx < 12; pc_idx++) {
                        if (pos & bs.getPieceBoards()[pc_idx]) {
                            drawPiece(pos, pc_idx, x, y);
                            break;
                        }
                    }
                }
            }
            pos >>= 1;
        }
    }

    // Draw the animating piece at interpolated position
    if (isAnimating) {
        float t = std::min(animProgress, 1.0f);
        Vector2 currentPos = animFrom * (1.0f - t) + animTo * t;
        drawPieceAt(animPieceIdx, currentPos);
    }
}