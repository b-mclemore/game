#include <game/menus/gamestate_mainmenu.h>
#include <game/menus/gamestate_helpmenu.h>
#include <engine/resource_manager.h>
#include <game/gamestate_main.h>
#include <engine/log.h>

GameStateHelpMenu::GameStateHelpMenu(
        std::shared_ptr<AtlasRenderer> ar,
        std::shared_ptr<TextRenderer> tr
) : GameStateMenu(std::move(ar), std::move(tr)) {
}

GameStateHelpMenu::~GameStateHelpMenu() {
}

void GameStateHelpMenu::clean() {
}

void GameStateHelpMenu::handleEvent(const InputState& inputState) {
    // Leave with escape
    if (inputState.keyboardState.isJustPressed(SDL_SCANCODE_ESCAPE)) {
        game->changeState(std::make_unique<GameStateMainMenu>(aRenderer, tRenderer));
    }
}

void GameStateHelpMenu::update(unsigned int dt) {
}

void GameStateHelpMenu::draw() {
    Color textColor = Color(255, 255, 0, 255);  // yellow

    std::vector<std::string> lines = {
        "Enter 'h' in the game for help",
        "",
        "(Press ESC to go back)"
    };

    int top = (screenHeight / 2) + (lines.size() / 2) * (32 + 4); // grid_size + vspacing, should get these from somewhere to be more modular &c
    int count = 0;

    for (auto& l : lines) {
        tRenderer->drawText(fontTexture, l, Vector2(screenWidth/2 - (l.size()/2 * 32), top - (count++ * (32 + 4))), textColor);
    }
}
