#include <game/game.h>
#include <engine/resource_manager.h>
#include <engine/gamestate.h>
#include <game/gamestate_main.h>
#include <game/menus/gamestate_mainmenu.h>

void Game::init(int screenWidth, int screenHeight) {
    windowWidth = screenWidth;
    windowHeight = screenHeight;
    gameIsRunning = true;

    inputManager = std::make_unique<InputManager>();
}

void Game::load() {
    // Load shaders
    ResourceManager::loadShader("assets/shaders/text.vert", "assets/shaders/text.frag", "", "atlas");
    // Compute projection matrix
    auto fWindowWidth = static_cast<float>(windowWidth);
    auto fWindowHeight = static_cast<float>(windowHeight);
    Matrix4 projection = Matrix4::createOrtho(fWindowWidth, fWindowHeight, -1.0f, 1.0f);
    Matrix4 twoDimTranslation = Matrix4::createTranslation(Vector3(-fWindowWidth / 2.f, -fWindowHeight / 2.f, 0.0f));
    Matrix4 finalProjection = twoDimTranslation * projection;
    // Configure shaders
    ResourceManager::getShader("atlas").use();
    ResourceManager::getShader("atlas").setMatrix4("projection", finalProjection);
    // Set render-specific controls
    aRenderer = std::make_shared<AtlasRenderer>(ResourceManager::getShader("atlas"));
    tRenderer = std::make_shared<TextRenderer>(ResourceManager::getShader("atlas"));
    // Set text size
    // tRenderer->setGlyphDim(32);

    // Game state - Start with menu
    changeState(std::make_unique<GameStateMainMenu>(aRenderer, tRenderer));
}

void Game::handleInputs() {
    inputManager->prepareForUpdate();
    gameIsRunning = inputManager->pollInputs();

    // Check for window resize
    if (inputManager->hasWindowResized()) {
        int newWidth, newHeight;
        inputManager->getNewWindowSize(newWidth, newHeight);
        handleResize(newWidth, newHeight);
    }

    const InputState& inputState = inputManager->getState();
    gameStates.back()->handleEvent(inputState);
}

void Game::update(unsigned int dt) {
    gameStates.back()->update(dt);
}

void Game::render() {
    gameStates.back()->draw();
}

void Game::clean() {
    ResourceManager::clear();
}

void Game::changeState(std::unique_ptr<GameState>&& state) {
    // Cleanup the current state
    if (!gameStates.empty()) {
        gameStates.back()->clean();
        gameStates.pop_back();
    }

    // Store and load the new state
    state->setGame(this);
    gameStates.push_back(std::move(state));
    gameStates.back()->load();
}

void Game::pushState(std::unique_ptr<GameState>&& state) {
    // Pause current state
    if (!gameStates.empty()) {
        gameStates.back()->pause();
    }

    // Store and init the new state
    gameStates.push_back(std::move(state));
    gameStates.back()->load();
}

void Game::popState() {
    // Cleanup the current state
    if (!gameStates.empty()) {
        gameStates.back()->clean();
        gameStates.pop_back();
    }

    // Resume previous state
    if (!gameStates.empty()) {
        gameStates.back()->resume();
    }
}

void Game::handleResize(int newWidth, int newHeight) {
    windowWidth = newWidth;
    windowHeight = newHeight;

    // Update viewport
    glViewport(0, 0, newWidth, newHeight);

    // Recalculate and upload projection matrix
    updateProjectionMatrix();

    // Notify current game state of resize
    if (!gameStates.empty()) {
        gameStates.back()->onResize(newWidth, newHeight);
    }
}

void Game::updateProjectionMatrix() {
    auto fWindowWidth = static_cast<float>(windowWidth);
    auto fWindowHeight = static_cast<float>(windowHeight);

    Matrix4 projection = Matrix4::createOrtho(fWindowWidth, fWindowHeight, -1.0f, 1.0f);
    Matrix4 twoDimTranslation = Matrix4::createTranslation(
        Vector3(-fWindowWidth / 2.f, -fWindowHeight / 2.f, 0.0f)
    );
    Matrix4 finalProjection = twoDimTranslation * projection;

    ResourceManager::getShader("atlas").use();
    ResourceManager::getShader("atlas").setMatrix4("projection", finalProjection);
}