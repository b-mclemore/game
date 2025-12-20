#ifndef ATLAS_RENDERER_H
#define ATLAS_RENDERER_H

#include <memory>

#include "shader.h"
#include "texture.h"
#include "vertex_array.h"
#include "maths.h"
#include "color.h"

// A renderer class able to write a sprite from an atlas
class AtlasRenderer
{
public:
    explicit AtlasRenderer(const Shader& shader);

    void setRowsCols(int rows, int cols);
    std::pair<int, int> getRowsCols();

    void setGlyphDim(int sideLength);
    int getGlyphDim();

    void drawSprite(const Texture2D& texture, Vector2 position,
		Vector2 size = Vector2(10, 10), GLfloat rotate = 0.0f,
		Color color = Color::white);

	void drawTiledRect(const Texture2D& texture, Vector2 position,
		Vector2 size, int tileSize, Color color = Color::white);

	void setUV(int row, int col, const Texture2D& texture);

    void drawAtlasSprite(const Texture2D& texture, Vector2 position,
        Vector2 size = Vector2(32, 32), GLfloat rotate = 0.0f,
        Color color = Color::white);

protected:
    Shader shader;
    std::unique_ptr<VertexArray> vertexArray;

    // Atlas properties (size of glyphs IN the texture)
    int atlasGlyphDim = 8;
    
    int rows = 16;
    int cols = 16;

    // Render properties (size to display on screen)
    int renderGlyphDim = 32;
};

#endif