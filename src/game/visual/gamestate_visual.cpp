#include <game/visual/gamestate_visual.h>
#include <engine/resource_manager.h>
#include <game/game.h>
#include <engine/log.h>

GameStateVisual::GameStateVisual(
        std::shared_ptr<AtlasRenderer> _aRenderer
) : mapRenderer(std::move(_aRenderer)) {
}

GameStateVisual::~GameStateVisual() {
}

void GameStateVisual::setGame(IGame* _game) {
    game = dynamic_cast<Game*>(_game);
}

void GameStateVisual::load() {
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
    ResourceManager::loadTexture("./assets/textures/goblin_new.png", "goblin_moving");

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
    tileMap->setTile(7, 7, TileType::NPC);

    // Initialize player at center
    player->setPos(15, 15);

    // Initialize camera
    cameraPos = calculateCameraPosition();
}

void GameStateVisual::clean() {
}

void GameStateVisual::pause() {
}

void GameStateVisual::resume() {
}

void GameStateVisual::handleEvent(const InputState& inputState) {
    // movement is handled in update

    if (inputState.keyboardState.isJustPressed(SDL_SCANCODE_RETURN)) {
        // check if we're trying to interact with an npc (@TODO make a list of
        // npcs somewhere so we can just iterate or something)
        auto [px, py] = player->getPos();
        if ((px == 7 && (py == 8 || py == 6)) ||
            (py == 7 && (px == 8 || px == 6))) {
            // if so, print a tutorial statement
            should_print = true;
            msg = "The goblin screeches\n'Welcome to CYBER-SPACE!!'";
            return;
        }
    }
    should_print = false;
}

void GameStateVisual::update(unsigned int dt) {
    // get current position
    auto [px, py] = player->getPos();
    // Get current input state
    const InputState& inputState = game->getInputManager()->getState();

    // Check if any movement key is pressed
    bool anyKeyPressed = inputState.keyboardState.isDown(SDL_Scancode(moveRightKey)) ||
                         inputState.keyboardState.isDown(SDL_Scancode(moveLeftKey)) ||
                         inputState.keyboardState.isDown(SDL_Scancode(moveUpKey)) ||
                         inputState.keyboardState.isDown(SDL_Scancode(moveDownKey));

    // Check for instant movement on key press (JustPressed)
    bool movedInstantly = false;

    // Move right
    if (inputState.keyboardState.isJustPressed(SDL_Scancode(moveRightKey))) {
        int newX = px + 1;
        if (isValidPosition(newX, py)) {
            player->setPos(newX, py);
            movedInstantly = true;
            movementAccumulator = 0.0f;
        }
        player->setDir(PlayerFacing::E);
    }
    // Move left
    else if (inputState.keyboardState.isJustPressed(SDL_Scancode(moveLeftKey))) {
        int newX = px - 1;
        if (isValidPosition(newX, py)) {
            player->setPos(newX, py);
            movedInstantly = true;
            movementAccumulator = 0.0f;
        }
        player->setDir(PlayerFacing::W);
    }
    // Move up
    else if (inputState.keyboardState.isJustPressed(SDL_Scancode(moveUpKey))) {
        int newY = py + 1;
        if (isValidPosition(px, newY)) {
            player->setPos(px, newY);
            movedInstantly = true;
            movementAccumulator = 0.0f;
        }
        player->setDir(PlayerFacing::N);
    }
    // Move down
    else if (inputState.keyboardState.isJustPressed(SDL_Scancode(moveDownKey))) {
        int newY = py - 1;
        if (isValidPosition(px, newY)) {
            player->setPos(px, newY);
            movedInstantly = true;
            movementAccumulator = 0.0f;
        }
        player->setDir(PlayerFacing::S);
    }

    // If no instant movement, handle continuous movement for held keys
    if (!movedInstantly && anyKeyPressed) {
        movementAccumulator += dt;

        // Check if enough time has passed for continuous movement
        if (movementAccumulator >= MOVEMENT_DELAY) {
            movementAccumulator -= MOVEMENT_DELAY;

            // Move right
            if (inputState.keyboardState.isDown(SDL_Scancode(moveRightKey))) {
                int newX = px + 1;
                if (isValidPosition(newX, py)) {
                    player->setPos(newX, py);
                }
            }
            // Move left
            else if (inputState.keyboardState.isDown(SDL_Scancode(moveLeftKey))) {
                int newX = px - 1;
                if (isValidPosition(newX, py)) {
                    player->setPos(newX, py);
                }
            }
            // Move up
            else if (inputState.keyboardState.isDown(SDL_Scancode(moveUpKey))) {
                int newY = py + 1;
                if (isValidPosition(px, newY)) {
                    player->setPos(px, newY);
                }
            }
            // Move down
            else if (inputState.keyboardState.isDown(SDL_Scancode(moveDownKey))) {
                int newY = py - 1;
                if (isValidPosition(px, newY)) {
                    player->setPos(px, newY);
                }
            }
        }
    } else if (!anyKeyPressed) {
        // Reset accumulator when no keys are pressed
        movementAccumulator = 0.0f;
    }
}

void GameStateVisual::draw() {
    drawMap();
    drawPlayer();
}

bool GameStateVisual::isValidPosition(int x, int y) {
    // Check map bounds
    if (!tileMap->isInBounds(x, y)) {
        return false;
    }

    // Check if tile is walkable
    return tileMap->isWalkable(x, y);
}

Vector2 GameStateVisual::calculateCameraPosition() {
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

void GameStateVisual::drawPlayer() {
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

void GameStateVisual::drawMap() {
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
    const Texture2D& npcTex = ResourceManager::getTexture("npc");

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
                case TileType::NPC:    tex = &npcTex; break;
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

void GameStateVisual::onResize(int newWidth, int newHeight) {
    screenWidth = newWidth;
    screenHeight = newHeight;

    // Camera will automatically recenter on next draw() call
}
