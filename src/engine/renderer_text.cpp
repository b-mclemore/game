#include "renderer_text.h"
#include "../engine/log.h"
#include <map>

std::map<char, char> charMap = {
    { 'a', 10 * 16 + 1 },
    { 'i', 10 * 16 + 9 },
    { 'q', 9 * 16 + 1 },
    { 'r', 9 * 16 + 2 },
    { 's', 9 * 16 + 3 },
    { 't', 9 * 16 + 4 },
    { 'u', 9 * 16 + 5 }
};

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

    for (char c : text) {
        auto idx = charMap.find(static_cast<unsigned char>(c));
        int index;
        if (idx == charMap.end()) index = 15 * 16 + 1; // exclam
        else index = charMap[c];
        int col = index % cols;
        int row = index / cols;

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