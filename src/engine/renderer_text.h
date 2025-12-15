#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <memory>

#include "shader.h"
#include "texture.h"
#include "vertex_array.h"
#include "maths.h"
#include "color.h"
#include "renderer_sprite.h"

// A renderer class able to write text, accomplished by drawing
// sprites
class TextRenderer : public SpriteRenderer
{
public:
    int glyphWidth = 32;
    int glyphHeight = 32;
    int rows = 16;
    int cols = 16;
    explicit TextRenderer(const Shader& shader) : SpriteRenderer(shader) {};
    
    void drawText(const Texture2D& texture,
        const std::string& text,
        Vector2 position, 
        Color color = Color::white);

    void drawSpriteNoBinding(const Texture2D& texture, Vector2 position,
        Vector2 size = Vector2(10, 10), GLfloat rotate = 0.0f,
        Color color = Color::white);

private:
};

#endif