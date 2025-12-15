#include "renderer_text.h"
#include "../engine/log.h"

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
        int index = static_cast<unsigned char>(c);
        int col = index % cols;
        int row = index / cols;

        float u0 = (col * glyphWidth) / (float)texture.width;
        float v0 = (row * glyphHeight) / (float)texture.height;
        float u1 = u0 + glyphWidth / (float)texture.width;
        float v1 = v0 + glyphHeight / (float)texture.height;

        shader.setVector4f("uvRect", Vector4(u0, v0, u1, v1));

        drawSpriteNoBinding(
            texture,
            Vector2(x, position.y),
            Vector2(glyphWidth, glyphHeight),
            0.0f,
            color
        );

        x += glyphWidth;
    }
}