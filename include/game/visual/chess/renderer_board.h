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
    void drawBoard(const BoardState& bs, int centerX, int centerY);

    // setter for renderGlyphDims
    void setBoardSize(int size);

	void loadTextures();

private:
	Texture2D squareTex;
	Texture2D pieceTex;
};

#endif