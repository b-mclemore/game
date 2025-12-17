#include <game/menus/gamestate_mainmenu.h>
#include <game/menus/gamestate_settingsmenu.h>
#include <engine/resource_manager.h>
#include <game/gamestate_main.h>
#include <engine/log.h>

GameStateSettingsMenu::GameStateSettingsMenu(
        std::shared_ptr<SpriteRenderer> sr,
        std::shared_ptr<GeometryRenderer> gr,
        std::shared_ptr<TextRenderer> tr
) : GameStateMenu(std::move(sr), std::move(gr), std::move(tr)) {
}

GameStateSettingsMenu::~GameStateSettingsMenu() {
}

void GameStateSettingsMenu::clean() {
}

void GameStateSettingsMenu::handleEvent(const InputState& inputState) {
    // Leave with escape
    if (inputState.keyboardState.isJustPressed(SDL_SCANCODE_ESCAPE)) {
        game->changeState(std::make_unique<GameStateMainMenu>(sRenderer, gRenderer, tRenderer));
    }
}

void GameStateSettingsMenu::update(unsigned int dt) {
}

void GameStateSettingsMenu::draw() {
    Color textColor = Color(255, 255, 0, 255);  // yellow

    std::vector<std::string> lines = {
        "Sorry, no settings yet exist to be changed.",
        "",
        "Press ESC to go back."
    };

    int top = (screenHeight / 2) + (lines.size() / 2) * (32 + 4); // grid_size + vspacing, should get these from somewhere to be more modular &c
    int count = 0;

    for (auto& l : lines) {
        tRenderer->drawText(fontTexture, l, Vector2(screenWidth/2 - (l.size()/2 * 32), top - (count++ * (32 + 4))), textColor);
    }
}
