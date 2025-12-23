#include <game/visual/room/gamestate_room.h>
#include <engine/resource_manager.h>
#include <game/game.h>
#include <engine/log.h>

GameStateRoom::GameStateRoom(
        std::shared_ptr<AtlasRenderer> _aRenderer
) : mapRenderer(std::move(_aRenderer)) {
}

GameStateRoom::~GameStateRoom() {
}

void GameStateRoom::setGame(IGame* _game) {
    game = dynamic_cast<Game*>(_game);
}

void GameStateRoom::load() {
    screenWidth = game->windowWidth / 2;
    screenHeight = game->windowHeight;

    // Set up WASD controls
    moveLeftKey = SDL_SCANCODE_A;
    moveRightKey = SDL_SCANCODE_D;
    moveUpKey = SDL_SCANCODE_W;
    moveDownKey = SDL_SCANCODE_S;

    // Load textures
    for (auto& s : {
        "cobble", "dirt", "empty", "gobrin", "gnome", "tree", "camilla"
    }) {
        ResourceManager::loadTexture(std::string("./assets/textures/")+s+std::string(".png"), s);
        ResourceManager::getTexture(s).setFiltering(GL_NEAREST, GL_NEAREST);
    }

    characterRenderer = std::make_shared<AtlasRenderer>(ResourceManager::getShader("atlas"));
    mapRenderer = std::make_shared<AtlasRenderer>(ResourceManager::getShader("atlas"));
    mapRenderer->setAtlasGlyphDims(16, 16);
    mapRenderer->setRenderGlyphDims(64, 64);
    characterRenderer->setAtlasGlyphDims(16, 19);
    characterRenderer->setRenderGlyphDims(64, 76);
    characterRenderer->setRowsCols(4, 3);

    // Create map
    tileMap = std::make_unique<TileMap>(screenWidth / GRID_SIZE, screenHeight / GRID_SIZE);
    tileMap->loadTileMap("assets/maps/intro.txt");

    // Load npcs
    npcs.push_back(Npc(InteractionType::Chess, "It's PUZZLE TIME!\nSolve the puzzle with a\nmove, like 'e2e4'.", 3, 3));
    npcs.push_back(Npc(InteractionType::Talk, "Hmm? Me? I'm a gnome.", 9, 9));
    for (auto& c : npcs) {
        auto [x, y] = c.getPos();
        tileMap->setWalkability(x, y, false);
    }

    // Initialize player at center
    player->setPos(6, 6);
    player->setRenderPos(Vector2(6 * GRID_SIZE, 6 * GRID_SIZE));

    // Initialize camera
    cameraPos = calculateCameraPosition();
}

void GameStateRoom::clean() {
}

void GameStateRoom::pause() {
}

void GameStateRoom::resume() {
}

void GameStateRoom::handleEvent(const InputState& inputState) {
    if (inputState.keyboardState.isJustPressed(SDL_SCANCODE_RETURN)) {
        // check if we're trying to interact with an npc
        auto [px, py] = player->getPos();
        auto pdir = player->getDir();
        for (auto& c : npcs) {
            // If the player is next to the npc and facing them when enter is pressed,
            // an interaction occurs.
            if (c.isAdjacentAndFacing(px, py, pdir)) {
                interaction = c.interact(pdir);
                return;
            }
        }
    }

    // handle movement
    if (player->getIsMoving())
        return;

    // Start movement
    if (inputState.keyboardState.isDown(SDL_Scancode(moveLeftKey))) {
        tryStartMovePlayer(Facing::W);
    }
    else if (inputState.keyboardState.isDown(SDL_Scancode(moveRightKey))) {
        tryStartMovePlayer(Facing::E);
    }
    else if (inputState.keyboardState.isDown(SDL_Scancode(moveUpKey))) {
        tryStartMovePlayer(Facing::N);
    }
    else if (inputState.keyboardState.isDown(SDL_Scancode(moveDownKey))) {
        tryStartMovePlayer(Facing::S);
    }
}

// Gets the player direction according to key
Facing GameStateRoom::getFacingFromKey(const InputState& inputState) {
    Facing f = Facing::N;
    if (inputState.keyboardState.isDown(SDL_Scancode(moveLeftKey)))
        f = Facing::W;
    else if (inputState.keyboardState.isDown(SDL_Scancode(moveRightKey)))
        f = Facing::E;
    else if (inputState.keyboardState.isDown(SDL_Scancode(moveDownKey)))
        f = Facing::S;
    return f;
}

void GameStateRoom::tryStartMovePlayer(Facing dir) {
    if (player->getIsMoving()) return;

    auto [gx, gy] = player->getPos();
    auto [dx, dy] = movementMap[dir];

    if (!isValidPosition(gx + dx, gy + dy)) {
        player->setDir(dir);
        return;
    }

    player->setDir(dir);
    player->setIsMoving(true);

    player->setMoveFrom(Vector2(gx * GRID_SIZE, gy * GRID_SIZE));
    player->setMoveTo(  Vector2((gx + dx) * GRID_SIZE,
                               (gy + dy) * GRID_SIZE));

    player->setMoveProgress(0.0f);
    player->setHalfStepTriggered(false);
}

void GameStateRoom::update(unsigned int dt) {
    if (!player->getIsMoving())
        return;

    // Advance movement
    auto mp = player->getMoveProgress();
    mp += dt / MOVEMENT_DELAY;
    float t = std::min(mp, 1.0f);
    player->setMoveProgress(mp);

    // Interpolate render position
    player->setRenderPos(
        player->getMoveFrom() * (1.0f - t) +
        player->getMoveTo()   * t);

    // Half-tile animation trigger
    if (!player->getHalfStepTriggered() && t >= 0.5f) {
        toggleAnimationFrame();
        player->setHalfStepTriggered(true);
    }

    // Finish movement
    if (t >= 1.0f) {
        auto [gx, gy] = player->getPos();
        auto [dx, dy] = movementMap[player->getDir()];

        player->setPos(gx + dx, gy + dy);
        player->setRenderPos(player->getMoveTo());

        player->setIsMoving(false);
        player->setAnimFrame(AnimationFrame::noSwing);
    }
}

void GameStateRoom::draw() {
    // Map first
    drawMap();
    // Then players
    auto [px, py] = player->getRenderPos();
    drawCharacter(*player, px, py, ResourceManager::getTexture("gobrin"));
    drawNpcs();
}

bool GameStateRoom::isValidPosition(int x, int y) {
    // Check map bounds
    if (!tileMap->isInBounds(x, y)) {
        return false;
    }

    // Check if tile is walkable
    return tileMap->isWalkable(x, y);
}

Vector2 GameStateRoom::calculateCameraPosition() {
    auto [px, py] = player->getPos();
    // Center player on screen
    float camX = px * GRID_SIZE - screenWidth / 2.0f;
    float camY = py * GRID_SIZE - screenHeight / 2.0f;

    // Clamp to map boundaries
    float maxCamX = tileMap->getWidth() * GRID_SIZE - screenWidth;
    float maxCamY = tileMap->getHeight() * GRID_SIZE - screenHeight;

    camX = std::max(0.0f, std::min(camX, maxCamX));
    camY = std::max(0.0f, std::min(camY, maxCamY));

    return Vector2(camX, camY);
}

void GameStateRoom::drawCharacter(Character &p, int px, int py, const Texture2D &texture) {
    // Player position relative to camera
    float screenX = px - cameraPos.x;
    float screenY = py - cameraPos.y;
    
    int row = static_cast<int>(p.getDir());
    int col = static_cast<int>(p.getAnimFrame());
    characterRenderer->setUV(row, col, texture);
    auto [renderWidth, renderHeight] = characterRenderer->getRenderGlyphDims();
    characterRenderer->drawAtlasSprite(
        texture,
        Vector2(screenX, screenY),
        Vector2(renderWidth, renderHeight)
    );
}

void GameStateRoom::drawMap() {
    // Update camera position
    cameraPos = calculateCameraPosition();

    // Calculate visible tile range
    int startX = static_cast<int>(cameraPos.x / GRID_SIZE);
    int startY = static_cast<int>(cameraPos.y / GRID_SIZE);
    int endX = startX + (screenWidth / GRID_SIZE);
    int endY = startY + (screenHeight / GRID_SIZE);

    // Clamp to map bounds
    startX = std::max(0, startX);
    startY = std::max(0, startY);
    endX = std::min(tileMap->getWidth(), endX);
    endY = std::min(tileMap->getHeight(), endY);

    // Get textures
    const Texture2D& emptyTex = ResourceManager::getTexture("empty");
    const Texture2D& dirtTex = ResourceManager::getTexture("dirt");
    const Texture2D& cobbleTex = ResourceManager::getTexture("cobble");
    const Texture2D& treeTex = ResourceManager::getTexture("tree");

    // Render visible tiles
    for (int y = startY; y < endY; y++) {
        for (int x = startX; x < endX; x++) {
            TileType tile = tileMap->getTile(x, y);

            // Calculate screen position (world position - camera offset)
            float screenX = x * GRID_SIZE - cameraPos.x;
            float screenY = y * GRID_SIZE - cameraPos.y;

            // Select texture based on tile type
            const Texture2D* tex = nullptr;
            switch (tile) {
                case TileType::EMPTY:  tex = &emptyTex; break;
                case TileType::DIRT:   tex = &dirtTex; break;
                case TileType::COBBLE: tex = &cobbleTex; break;
                case TileType::TREE: tex = &treeTex; break;
            }

            if (tex) {
                mapRenderer->setUV(0, 0, *tex);
                // TEMPORARY
                if (tile == TileType::DIRT) {
                    mapRenderer->setUV(y % 4, x % 4, *tex);
                } else if (tile == TileType::TREE) {
                    mapRenderer->setUV((y % 2), x % 2, *tex);
                }
                mapRenderer->drawAtlasSprite(
                    *tex,
                    Vector2(screenX, screenY),
                    Vector2(GRID_SIZE, GRID_SIZE)
                );
            }
        }
    }
}

void GameStateRoom::drawNpcs() {
    // Calculate visible tile range
    int startX = static_cast<int>(cameraPos.x / GRID_SIZE);
    int startY = static_cast<int>(cameraPos.y / GRID_SIZE);
    int endX = startX + (screenWidth / GRID_SIZE);
    int endY = startY + (screenHeight / GRID_SIZE);

    // Clamp to map bounds
    startX = std::max(0, startX);
    startY = std::max(0, startY);
    endX = std::min(tileMap->getWidth(), endX);
    endY = std::min(tileMap->getHeight(), endY);

    for (auto& c : npcs) {
        auto [x, y] = c.getPos();
        if ((x < startX || endX <= x) ||
            (y < startY || endY <= y)) continue;
        drawCharacter(c, x * GRID_SIZE, y * GRID_SIZE, ResourceManager::getTexture("gnome"));
    }
}

void GameStateRoom::onResize(int newWidth, int newHeight) {
    screenWidth = newWidth;
    screenHeight = newHeight;

    // Camera will automatically recenter on next draw() call
}

void GameStateRoom::toggleAnimationFrame() {
    AnimationFrame current = player->getAnimFrame();

    if (current == AnimationFrame::noSwing) {
        if (lastSwungLeft) {
            player->setAnimFrame(AnimationFrame::swingLeft);
            lastSwungLeft = false;
        } else {
            player->setAnimFrame(AnimationFrame::swingRight);
            lastSwungLeft = true;
        }
    }
    else if (current == AnimationFrame::swingLeft) {
        player->setAnimFrame(AnimationFrame::noSwing);
    }
    else {
        player->setAnimFrame(AnimationFrame::noSwing);
    }
}
