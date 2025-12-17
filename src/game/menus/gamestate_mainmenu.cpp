#include <game/menus/gamestate_mainmenu.h>
#include <game/menus/gamestate_helpmenu.h>
#include <game/menus/gamestate_settingsmenu.h>
#include <engine/resource_manager.h>
#include <game/gamestate_main.h>
#include <engine/log.h>

GameStateMainMenu::GameStateMainMenu(
        std::shared_ptr<SpriteRenderer> sr,
        std::shared_ptr<GeometryRenderer> gr,
        std::shared_ptr<TextRenderer> tr
) : GameStateMenu(std::move(sr), std::move(gr), std::move(tr)) {
}

GameStateMainMenu::~GameStateMainMenu() {
}

void GameStateMainMenu::load() {
    GameStateMenu::load();

    numOptions = 4;
}

void GameStateMainMenu::clean() {
}

void GameStateMainMenu::handleEvent(const InputState& inputState) {
    // Navigate menu
    if (inputState.keyboardState.isJustPressed(SDL_SCANCODE_W) ||
        inputState.keyboardState.isJustPressed(SDL_SCANCODE_UP)) {
        selectedOption = ((selectedOption - 1) % numOptions);
    }

    if (inputState.keyboardState.isJustPressed(SDL_SCANCODE_S) ||
        inputState.keyboardState.isJustPressed(SDL_SCANCODE_DOWN)) {
        selectedOption = ((selectedOption + 1) % numOptions);
    }

    // Confirm with Enter
    if (inputState.keyboardState.isJustPressed(SDL_SCANCODE_RETURN)) {
        switch (selectedOption) {
            case 0:
                game->changeState(std::make_unique<GameStateMain>(sRenderer, gRenderer, tRenderer));
                break;
            case 1:
                game->changeState(std::make_unique<GameStateHelpMenu>(sRenderer, gRenderer, tRenderer));
                break;
            case 2:
                game->changeState(std::make_unique<GameStateSettingsMenu>(sRenderer, gRenderer, tRenderer));
                break;
            default:
                game->gameIsRunning = false;
        }
    }
}

void GameStateMainMenu::update(unsigned int dt) {
}

void GameStateMainMenu::draw() {
    // Colors for menu items
    // (Using rectangles as placeholders)
    Color selectedColor = Color(255, 255, 0, 255);  // yellow
    Color normalColor = Color(64, 64, 0, 255);  // darker yellow

    Color startColor = normalColor;
    Color helpColor = normalColor;
    Color settingsColor = normalColor;
    Color quitColor = normalColor;
    switch (selectedOption) {
        case 0:
            startColor = selectedColor;
            break;
        case 1:
            helpColor = selectedColor;
            break;
        case 2:
            settingsColor = selectedColor;
            break;
        case 3:
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
