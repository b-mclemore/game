#include <game/gamestate_visual.h>
#include <engine/resource_manager.h>
#include <game/game.h>
#include <engine/log.h>

GameStateVisual::GameStateVisual(
        std::shared_ptr<SpriteRenderer> _sRenderer,
        std::shared_ptr<GeometryRenderer> _gRenderer
) : sRenderer(std::move(_sRenderer)), gRenderer(std::move(_gRenderer)) {
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
    ResourceManager::loadTexture("./assets/textures/cobble.png", "cobble");
    ResourceManager::loadTexture("./assets/textures/dirt.png", "dirt");
    ResourceManager::loadTexture("./assets/textures/empty.png", "empty");

    // Create map (30×30 tiles)
    tileMap = std::make_unique<TileMap>(30, 30);

    // Calculate border positions
    int centerTile = 15;
    // int wallDistance = 13;
    int wallMin = 0;
    int wallMax = 29;

    // Initialize map layout
    for (int y = 0; y < 30; y++) {
        for (int x = 0; x < 30; x++) {
            // Outside wall empty
            if (x < wallMin || x > wallMax || y < wallMin || y > wallMax) {
                tileMap->setTile(x, y, TileType::EMPTY);
            }
            // Wall border cobble
            else if (x == wallMin || x == wallMax || y == wallMin || y == wallMax) {
                tileMap->setTile(x, y, TileType::COBBLE);
            }
            // Interior dirt
            else {
                tileMap->setTile(x, y, TileType::DIRT);
            }
        }
    }

    // Initialize player at center
    player.x = 15;
    player.y = 15;

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
    // Event handling is now empty - movement happens in update()
    // Could add non-movement actions here (open inventory, etc.)
}

void GameStateVisual::update(unsigned int dt) {
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
        int newX = player.x + 1;
        if (isValidPosition(newX, player.y)) {
            player.x = newX;
            movedInstantly = true;
            movementAccumulator = 0.0f;
        }
    }
    // Move left
    else if (inputState.keyboardState.isJustPressed(SDL_Scancode(moveLeftKey))) {
        int newX = player.x - 1;
        if (isValidPosition(newX, player.y)) {
            player.x = newX;
            movedInstantly = true;
            movementAccumulator = 0.0f;
        }
    }
    // Move up
    else if (inputState.keyboardState.isJustPressed(SDL_Scancode(moveUpKey))) {
        int newY = player.y + 1;
        if (isValidPosition(player.x, newY)) {
            player.y = newY;
            movedInstantly = true;
            movementAccumulator = 0.0f;
        }
    }
    // Move down
    else if (inputState.keyboardState.isJustPressed(SDL_Scancode(moveDownKey))) {
        int newY = player.y - 1;
        if (isValidPosition(player.x, newY)) {
            player.y = newY;
            movedInstantly = true;
            movementAccumulator = 0.0f;
        }
    }

    // If no instant movement, handle continuous movement for held keys
    if (!movedInstantly && anyKeyPressed) {
        movementAccumulator += dt;

        // Check if enough time has passed for continuous movement
        if (movementAccumulator >= MOVEMENT_DELAY) {
            movementAccumulator -= MOVEMENT_DELAY;

            // Move right
            if (inputState.keyboardState.isDown(SDL_Scancode(moveRightKey))) {
                int newX = player.x + 1;
                if (isValidPosition(newX, player.y)) {
                    player.x = newX;
                }
            }
            // Move left
            else if (inputState.keyboardState.isDown(SDL_Scancode(moveLeftKey))) {
                int newX = player.x - 1;
                if (isValidPosition(newX, player.y)) {
                    player.x = newX;
                }
            }
            // Move up
            else if (inputState.keyboardState.isDown(SDL_Scancode(moveUpKey))) {
                int newY = player.y + 1;
                if (isValidPosition(player.x, newY)) {
                    player.y = newY;
                }
            }
            // Move down
            else if (inputState.keyboardState.isDown(SDL_Scancode(moveDownKey))) {
                int newY = player.y - 1;
                if (isValidPosition(player.x, newY)) {
                    player.y = newY;
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
    // Center player on screen
    float camX = player.x * GRID_SIZE - screenWidth / 2.0f;
    float camY = player.y * GRID_SIZE - screenHeight / 2.0f;

    // Clamp to map boundaries
    float maxCamX = tileMap->getWidth() * GRID_SIZE - screenWidth;
    float maxCamY = tileMap->getHeight() * GRID_SIZE - screenHeight;

    camX = std::max(0.0f, std::min(camX, maxCamX));
    camY = std::max(0.0f, std::min(camY, maxCamY));

    return Vector2(camX, camY);
}

void GameStateVisual::drawPlayer() {
    // Player position relative to camera
    float screenX = player.x * GRID_SIZE - cameraPos.x;
    float screenY = player.y * GRID_SIZE - cameraPos.y;

    sRenderer->drawSprite(
        ResourceManager::getTexture("player"),
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
    int endX = startX + (screenWidth / GRID_SIZE) + 2;  // +2 for partial tiles
    int endY = startY + (screenHeight / GRID_SIZE) + 2;

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
                sRenderer->drawSprite(
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
