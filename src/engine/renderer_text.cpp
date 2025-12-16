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

void TextRenderer::drawSpriteNoBinding(const Texture2D& texture, Vector2 position,
    Vector2 size, GLfloat rotate, Color color) {
    // shader.use();

    Matrix4 scaleMat = Matrix4::createScale(
    static_cast<float>(texture.width),
    static_cast<float>(texture.height),
    1.0f);

    Matrix4 mWorldTransform = Matrix4::createScale(Vector3(size.x, size.y, 1));
    mWorldTransform *= Matrix4::createRotationZ(rotate);
    mWorldTransform *= Matrix4::createTranslation(Vector3(position.x + size.x / 2, position.y + size.y / 2, 0.0f));

    Matrix4 model = mWorldTransform;

    shader.setMatrix4("model", model);
    shader.setVector3f("spriteColor", color.toVector3());

    vertexArray->setActive();
    texture.setActive();

    glDrawArrays(GL_TRIANGLES, 0, 6);
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
        drawSpriteNoBinding(
            texture,
            Vector2(x, position.y),
            Vector2(renderGlyphWidth, renderGlyphHeight),
            0.0f,
            color
        );

        x += renderGlyphWidth;
    }
}