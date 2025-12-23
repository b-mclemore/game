#ifndef BOARD_RENDERER_H
#define BOARD_RENDERER_H

#include <string>
#include <engine/renderer_atlas.h>
#include <game/visual/chess/bitboards.h>
#include <engine/resource_manager.h>

// A renderer class able to draw the chess board
class BoardRenderer : public AtlasRenderer
{
public:
    explicit BoardRenderer(const Shader& shader) : AtlasRenderer(shader) {
	};
    
	void drawPiece(U64 pos, int pc_idx, int bottomX, int bottomY);

    // Draw board with optional success square (green square under piece)
    void drawBoard(const BoardState& bs, int centerX, int centerY, bool showSuccess, U64 successSquare);

    // Get screen position for a square given by bitboard
    Vector2 getSquareScreenPos(U64 square_bb, int boardCenterX, int boardCenterY);

    // Draw a specific piece type at arbitrary screen position
    void drawPieceAt(int pc_idx, Vector2 screenPos);

    // Draw board with animated piece (handles both correct and incorrect moves)
    void drawBoardWithAnimation(
        const BoardState& bs,
        int centerX, int centerY,
        bool isAnimating,
        int animPieceIdx,
        U64 sourceSquare,
        Vector2 animFrom,
        Vector2 animTo,
        float animProgress,
        bool isMoveCorrect
    );

    // setter for renderGlyphDims
    void setBoardSize(int size);

	void loadTextures();

private:
	Texture2D squareTex;
	Texture2D pieceTex;
};

#endif