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
    ResourceManager::loadTexture("./assets/textures/goblin.png", "player");
    ResourceManager::loadTexture("./assets/textures/goblin.png", "npc");
    ResourceManager::loadTexture("./assets/textures/cobble.png", "cobble");
    ResourceManager::loadTexture("./assets/textures/dirt.png", "dirt");
    ResourceManager::loadTexture("./assets/textures/empty.png", "empty");
    ResourceManager::loadTexture("./assets/textures/gobrin.png", "goblin_moving");

    playerRenderer = std::make_shared<AtlasRenderer>(ResourceManager::getShader("atlas"));
    mapRenderer = std::make_shared<AtlasRenderer>(ResourceManager::getShader("atlas"));
    mapRenderer->setGlyphDim(32);
    playerRenderer->setGlyphDim(16);
    playerRenderer->setRowsCols(4, 3);
    ResourceManager::getTexture("goblin_moving").setFiltering(GL_NEAREST, GL_NEAREST);

    // Create map
    tileMap = std::make_unique<TileMap>(screenWidth / GRID_SIZE, screenHeight / GRID_SIZE);
    tileMap->loadTileMap("assets/maps/tutorial.txt");

    // Load npcs
    npcs.push_back(Npc(InteractionType::Talk, "The goblin screeches\n'Welcome to CYBER-SPACE!!'", 12, 13));
    npcs.push_back(Npc(InteractionType::Chess, "It's GAME TIME!", 17, 10));

    // Initialize player at center
    player->setPos(15, 15);

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
    // movement is handled in update

    if (inputState.keyboardState.isJustPressed(SDL_SCANCODE_RETURN)) {
        // check if we're trying to interact with an npc
        auto [px, py] = player->getPos();
        for (auto& c : npcs) {
            if (c.isAdjacent(px, py)) {
                interaction = c.interact();
                return;
            }
        }
    }
}

bool GameStateRoom::movementJustPressed() {
    const InputState& inputState = game->getInputManager()->getState();
    return
    (inputState.keyboardState.isJustPressed(SDL_Scancode(moveRightKey)) ||
    inputState.keyboardState.isJustPressed(SDL_Scancode(moveLeftKey)) ||
    inputState.keyboardState.isJustPressed(SDL_Scancode(moveUpKey)) ||
    inputState.keyboardState.isJustPressed(SDL_Scancode(moveDownKey)));
}

// Simple boolean check that sees if any of the movekeys are isDown
bool GameStateRoom::movementKeyDown() {
    const InputState& inputState = game->getInputManager()->getState();
    return
    (inputState.keyboardState.isDown(SDL_Scancode(moveRightKey)) ||
    inputState.keyboardState.isDown(SDL_Scancode(moveLeftKey)) ||
    inputState.keyboardState.isDown(SDL_Scancode(moveUpKey)) ||
    inputState.keyboardState.isDown(SDL_Scancode(moveDownKey)));
}

// Moves the player according to key
void GameStateRoom::movePlayer(const InputState& inputState) {
    auto [px, py] = player->getPos();
    Facing f = Facing::N;
    if (inputState.keyboardState.isDown(SDL_Scancode(moveLeftKey)))
        f = Facing::W;
    else if (inputState.keyboardState.isDown(SDL_Scancode(moveRightKey)))
        f = Facing::E;
    else if (inputState.keyboardState.isDown(SDL_Scancode(moveDownKey)))
        f = Facing::S;
    auto [dx, dy] = movementMap[f];
    if (isValidPosition(px+dx, py+dy))
        player->setPos(px+dx, py+dy);
    player->setDir(f);
}

void GameStateRoom::update(unsigned int dt) {
    // Get current input state
    const InputState& inputState = game->getInputManager()->getState();
    bool anyKeyPressed = movementKeyDown();
    if (!anyKeyPressed) {
        movementAccumulator = 0.0f;
        return;
    }

    // get current position
    auto [px, py] = player->getPos();

    bool anyKeyJustPressed = movementJustPressed();
    if (anyKeyJustPressed) {
        // Handle justpressed key (meaning not held down, immediate movement)
        movementAccumulator = 0.0f;
        movePlayer(inputState);
    }
    else {
        // If no instant movement, handle continuous movement for held keys
        movementAccumulator += dt;
        // Check if enough time has passed
        if (movementAccumulator >= MOVEMENT_DELAY) {
            movementAccumulator -= MOVEMENT_DELAY;
            movePlayer(inputState);
        }
    }
}

void GameStateRoom::draw() {
    drawMap();
    drawPlayer();
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

void GameStateRoom::drawPlayer() {
    auto [px, py] = player->getPos();
    // Player position relative to camera
    float screenX = px * GRID_SIZE - cameraPos.x;
    float screenY = py * GRID_SIZE - cameraPos.y;
    
    int row = static_cast<int>(player->getDir());
    int col = static_cast<int>(player->getAnimFrame());
    playerRenderer->setUV(row, col, ResourceManager::getTexture("goblin_moving"));
    playerRenderer->drawAtlasSprite(
        ResourceManager::getTexture("goblin_moving"),
        Vector2(screenX, screenY),
        Vector2(GRID_SIZE, GRID_SIZE)
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
            }

            if (tex) {
                mapRenderer->setUV(0, 0, *tex);
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
        const Texture2D& npcTex = ResourceManager::getTexture("npc");
        mapRenderer->setUV(0, 0, npcTex);
        float screenX = x * GRID_SIZE - cameraPos.x;
        float screenY = y * GRID_SIZE - cameraPos.y;
        mapRenderer->drawAtlasSprite(
            npcTex,
            Vector2(screenX, screenY),
            Vector2(GRID_SIZE, GRID_SIZE)
        );
    }
}

void GameStateRoom::onResize(int newWidth, int newHeight) {
    screenWidth = newWidth;
    screenHeight = newHeight;

    // Camera will automatically recenter on next draw() call
}
