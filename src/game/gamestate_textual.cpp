#include <game/gamestate_textual.h>
#include <engine/resource_manager.h>
#include <game/game.h>
#include <engine/log.h>

GameStateTextual::GameStateTextual(
        std::shared_ptr<SpriteRenderer> _sRenderer,
        std::shared_ptr<GeometryRenderer> _gRenderer,
        std::shared_ptr<TextRenderer> _tRenderer
) : sRenderer(std::move(_sRenderer)), gRenderer(std::move(_gRenderer)), tRenderer(std::move(_tRenderer)) {
}

GameStateTextual::~GameStateTextual() {
}

void GameStateTextual::setGame(IGame* _game) {
    game = dynamic_cast<Game*>(_game);
}

void GameStateTextual::load() {
    screenWidth = game->windowWidth / 2;
    screenHeight = game->windowHeight;
    offset = game->windowWidth / 2;
    fontTexture = ResourceManager::loadTexture("./assets/textures/font_atlas.png", "font_atlas");
    fontTexture.setFiltering(GL_NEAREST, GL_NEAREST);
}

void GameStateTextual::clean() {
}

void GameStateTextual::pause() {
}

void GameStateTextual::resume() {
}

void GameStateTextual::handleEvent(const InputState& inputState) {
    if (inputState.keyboardState.isJustPressed(SDL_SCANCODE_RETURN)) {
        game->gameIsRunning = false;
    }
}

void GameStateTextual::update(unsigned int dt) {
}

void GameStateTextual::draw() {
    drawText();
}

void GameStateTextual::drawText() {
    // @TODO colors for console/text items should be stored in the class
    Color consoleColor = Color(32, 196, 32, 255);

    int startX = screenWidth/2 + offset - 32 * 9;
    int startY = screenHeight / 2;

    tRenderer->drawText(fontTexture, "tab+enter to quit", Vector2(startX, startY), consoleColor);
}

void GameStateTextual::onResize(int newWidth, int newHeight) {
    screenWidth = newWidth;
    screenHeight = newHeight;
}
