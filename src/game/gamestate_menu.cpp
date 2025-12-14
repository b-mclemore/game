#include "gamestate_menu.h"
#include "gamestate_main.h"
#include "../engine/log.h"

GameStateMenu::GameStateMenu(
        std::shared_ptr<SpriteRenderer> _sRenderer,
        std::shared_ptr<GeometryRenderer> _gRenderer
) : sRenderer(std::move(_sRenderer)), gRenderer(std::move(_gRenderer)) {
}

GameStateMenu::~GameStateMenu() {
}

void GameStateMenu::setGame(IGame* _game) {
    game = dynamic_cast<Game*>(_game);
}

void GameStateMenu::load() {
    screenWidth = game->windowWidth;
    screenHeight = game->windowHeight;
    selectedOption = MenuOptions::start;
}

void GameStateMenu::clean() {
}

void GameStateMenu::pause() {
}

void GameStateMenu::resume() {
}

void GameStateMenu::handleEvent(const InputState& inputState) {
    // Navigate menu with W/S or Up/Down
    if (inputState.keyboardState.isJustPressed(SDL_SCANCODE_W) ||
        inputState.keyboardState.isJustPressed(SDL_SCANCODE_UP)) {
        selectedOption = MenuOptions::start;
    }

    if (inputState.keyboardState.isJustPressed(SDL_SCANCODE_S) ||
        inputState.keyboardState.isJustPressed(SDL_SCANCODE_DOWN)) {
        selectedOption = MenuOptions::quit;
    }

    // Confirm with Enter
    if (inputState.keyboardState.isJustPressed(SDL_SCANCODE_RETURN)) {
        if (selectedOption == MenuOptions::start) {
            game->changeState(std::make_unique<GameStateMain>(sRenderer, gRenderer));
        } else {
            game->gameIsRunning = false;
        }
    }
}

void GameStateMenu::update(unsigned int dt) {
    // No update logic needed for menu
}

void GameStateMenu::draw() {
    // Colors for menu items
    // (Using rectangles as placeholders)
    Color selectedColor = Color(255, 255, 0, 255);  // Yellow bar
    Color normalColor = Color(0, 0, 0, 255);  // Black

    int startY = screenHeight / 2 + 50;
    int quitY = screenHeight / 2 - 50;

    if (selectedOption == MenuOptions::start) {
        gRenderer->drawRect(Vector2(screenWidth/2 - 100, startY), Vector2(200, 40), 0.0f, selectedColor);
    } else {
        gRenderer->drawRect(Vector2(screenWidth/2 - 100, startY), Vector2(200, 40), 0.0f, normalColor);
    }

    if (selectedOption == MenuOptions::quit) {
        gRenderer->drawRect(Vector2(screenWidth/2 - 100, quitY), Vector2(200, 40), 0.0f, selectedColor);
    } else {
        gRenderer->drawRect(Vector2(screenWidth/2 - 100, quitY), Vector2(200, 40), 0.0f, normalColor);
    }
}
