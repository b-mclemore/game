#ifndef ATLAS_RENDERER_H
#define ATLAS_RENDERER_H

#include <memory>

#include "shader.h"
#include "texture.h"
#include "vertex_array.h"
#include "maths.h"
#include "color.h"
#include "renderer_sprite.h"

// A renderer class able to write a sprite from an atlas
class AtlasRenderer : public SpriteRenderer
{
public:
    // Atlas properties (size of glyphs IN the texture)
    int atlasGlyphWidth = 8;
    int atlasGlyphHeight = 8;
    int rows = 16;
    int cols = 16;

    // Render properties (size to display on screen)
    int renderGlyphWidth = 32;
    int renderGlyphHeight = 32;

    explicit AtlasRenderer(const Shader& shader) : SpriteRenderer(shader) {};

	void setUV(int row, int col, const Texture2D& texture);

    void drawSprite(const Texture2D& texture, Vector2 position,
        Vector2 size = Vector2(32, 32), GLfloat rotate = 0.0f,
        Color color = Color::white);

private:
};

#endif