#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <string>
#include <engine/renderer_atlas.h>

// A renderer class able to write text, accomplished by drawing
// sprites
class TextRenderer : public AtlasRenderer
{
public:
    explicit TextRenderer(const Shader& shader) : AtlasRenderer(shader) {};
    
    void drawText(const Texture2D& texture,
        const std::string& text,
        Vector2 position, 
        Color color = Color::white);

    // setter for renderGlyphDim
    void setTextSize(int size);

private:
};

#endif