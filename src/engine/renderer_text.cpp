#include <engine/renderer_text.h>
#include <engine/log.h>

std::pair<int, int> charMap(char32_t c) {
    // ascii block 1 - 95
    if (' ' <= c && c <= '~') {
        int idx = c - ' ';
        int col = idx % 16;
        int row = 15 - (idx / 16);
        return std::pair<int, int>(row, col);
    }
    // boxes and borders
    else if (0x2500 <= c && c <= 0x259F) {
        int idx = c - 0x2500 - 1;
        int col = idx % 16;
        int row = 9 - (idx / 16);
        return std::pair<int, int>(row, col);
    } 
    // reserved
    else if (c == 0x007F) {
        LOG(Warning) << "Displaying a reserved character";
        return std::pair<int, int>(0, 15);
    }
    // not mapped
    else {
        LOG(Error) << "The requested character is not mapped";
        return std::pair<int, int>(15, 1);
    }
}

void TextRenderer::drawText(const Texture2D& texture, const std::string& text, Vector2 position, Color color) {
    shader.use();
    float x = position.x;

    for (char32_t c : text) {
        auto coords = charMap(c);
        int row = coords.first;
        int col = coords.second;

        // Use atlas dimensions for UV calculation
        float u0 = (col * atlasGlyphWidth) / (float)texture.width;
        float v0 = (row * atlasGlyphHeight) / (float)texture.height;
        float u1 = u0 + atlasGlyphWidth / (float)texture.width;
        float v1 = v0 + atlasGlyphHeight / (float)texture.height;

        shader.setVector4f("uvRect", Vector4(u0, v0, u1, v1));

        // Use render dimensions for display size
        drawSprite(
            texture,
            Vector2(x, position.y),
            Vector2(renderGlyphWidth, renderGlyphHeight),
            0.0f,
            color
        );

        x += renderGlyphWidth;
    }
}

void TextRenderer::setTextSize(int size) {
    renderGlyphWidth = size;
    renderGlyphHeight = size;
}