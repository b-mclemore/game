#include <game/gamestate_menu.h>
#include <engine/resource_manager.h>
#include <game/gamestate_main.h>
#include <engine/log.h>

GameStateMenu::GameStateMenu(
        std::shared_ptr<SpriteRenderer> _sRenderer,
        std::shared_ptr<GeometryRenderer> _gRenderer,
        std::shared_ptr<TextRenderer> _tRenderer
) : sRenderer(std::move(_sRenderer)), gRenderer(std::move(_gRenderer)), tRenderer(std::move(_tRenderer)) {
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
    fontTexture = ResourceManager::loadTexture("./assets/textures/font_atlas.png", "font_atlas");
    fontTexture.setFiltering(GL_NEAREST, GL_NEAREST);
}

void GameStateMenu::clean() {
}

void GameStateMenu::pause() {
}

void GameStateMenu::resume() {
}

void GameStateMenu::handleEvent(const InputState& inputState) {
    // Navigate menu
    if (inputState.keyboardState.isJustPressed(SDL_SCANCODE_W) ||
        inputState.keyboardState.isJustPressed(SDL_SCANCODE_UP)) {
        selectedOption = (MenuOptions)((selectedOption + 3) % 4);
    }

    if (inputState.keyboardState.isJustPressed(SDL_SCANCODE_S) ||
        inputState.keyboardState.isJustPressed(SDL_SCANCODE_DOWN)) {
        selectedOption = (MenuOptions)((selectedOption + 1) % 4);
    }

    // Confirm with Enter
    if (inputState.keyboardState.isJustPressed(SDL_SCANCODE_RETURN)) {
        switch (selectedOption) {
            case MenuOptions::start:
                game->changeState(std::make_unique<GameStateMain>(sRenderer, gRenderer, tRenderer));
                break;
            default:
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
    Color selectedColor = Color(255, 255, 0, 255);  // yellow
    Color normalColor = Color(64, 64, 0, 255);  // darker yellow

    Color startColor = normalColor;
    Color helpColor = normalColor;
    Color settingsColor = normalColor;
    Color quitColor = normalColor;
    switch (selectedOption) {
        case MenuOptions::start:
            startColor = selectedColor;
            break;
        case MenuOptions::help:
            helpColor = selectedColor;
            break;
        case MenuOptions::settings:
            settingsColor = selectedColor;
            break;
        case MenuOptions::quit:
            quitColor = selectedColor;
            break;
    }

    int startY = screenHeight / 2 + 150;
    int helpY = screenHeight / 2 + 50;
    int settingsY = screenHeight / 2 - 50;
    int quitY = screenHeight / 2 - 150;

    tRenderer->drawText(fontTexture, "start", Vector2(screenWidth/2 - 100, startY), startColor);
    tRenderer->drawText(fontTexture, "help", Vector2(screenWidth/2 - 100, helpY), helpColor);
    tRenderer->drawText(fontTexture, "settings", Vector2(screenWidth/2 - 100, settingsY), settingsColor);
    tRenderer->drawText(fontTexture, "quit", Vector2(screenWidth/2 - 100, quitY), quitColor);
}
