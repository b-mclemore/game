#include <engine/renderer_atlas.h>

void AtlasRenderer::setRowsCols(int r, int c) {
    rows = r;
    cols = c;
}

std::pair<int, int> AtlasRenderer::getRowsCols() {
    return {rows, cols};
}

void AtlasRenderer::setGlyphDim(int gd) {
    atlasGlyphDim = gd;
}

int AtlasRenderer::getGlyphDim() {
    return atlasGlyphDim;
}

static std::array<GLfloat, 18> vertexBuffer = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.5f, 0.5f, 0.0f,

    0.5f, 0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f
};

static std::array<GLfloat, 12> texBuffer = {
    0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 0.0f,
    1.0f, 0.0f, 0.0f
};

AtlasRenderer::AtlasRenderer(const Shader& shader) : shader(shader) {
    vertexArray = std::make_unique<VertexArray>(vertexBuffer, texBuffer);
}

// Draws a sprite at a set location. Requires that the texture holds a single sprite.
void AtlasRenderer::drawSprite(const Texture2D& texture, Vector2 position,
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

// Tiles a sprite that is in a single-sprite atlas
void AtlasRenderer::drawTiledRect(const Texture2D& texture, Vector2 position,
                                   Vector2 size, int tileSize, Color color) {
    // Calculate how many tiles fit in the rectangle
    int tilesX = static_cast<int>(size.x) / tileSize;
    int tilesY = static_cast<int>(size.y) / tileSize;

    // Draw each tile
    for (int y = 0; y < tilesY; y++) {
        for (int x = 0; x < tilesX; x++) {
            Vector2 tilePos(
                position.x + x * tileSize,
                position.y + y * tileSize
            );
            drawSprite(texture, tilePos, Vector2(tileSize, tileSize), 0.0f, color);
        }
    }
}

// Set the uv for sprite in an atlas
void AtlasRenderer::setUV(int row, int col, const Texture2D& texture) {
	shader.use();

	// Use atlas dimensions for UV calculation
	float u0 = (col * atlasGlyphDim) / (float)texture.width;
	float v0 = (row * atlasGlyphDim) / (float)texture.height;
	float u1 = u0 + atlasGlyphDim / (float)texture.width;
	float v1 = v0 + atlasGlyphDim / (float)texture.height;

	shader.setVector4f("uvRect", Vector4(u0, v0, u1, v1));
}

// draw sprite from atlas. requires that uv is set.
void AtlasRenderer::drawAtlasSprite(const Texture2D& texture, Vector2 position,
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