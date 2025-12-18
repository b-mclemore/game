#include <game/menus/gamestate_menu.h>
#include <engine/resource_manager.h>
#include <game/gamestate_main.h>
#include <engine/log.h>

// class for displaying menus (main menu, settings, etc)
GameStateMenu::GameStateMenu(
        std::shared_ptr<AtlasRenderer> _aRenderer,
        std::shared_ptr<TextRenderer> _tRenderer
) : aRenderer(std::move(_aRenderer)), tRenderer(std::move(_tRenderer)) {
}

GameStateMenu::~GameStateMenu() {
}

void GameStateMenu::setGame(IGame* _game) {
    game = dynamic_cast<Game*>(_game);
}

void GameStateMenu::load() {
    screenWidth = game->windowWidth;
    screenHeight = game->windowHeight;
    fontTexture = ResourceManager::loadTexture("./assets/textures/font_atlas.png", "font_atlas");
    fontTexture.setFiltering(GL_NEAREST, GL_NEAREST);
}