#include <game/menus/gamestate_menu.h>
#include <engine/resource_manager.h>
#include <game/gamestate_main.h>
#include <engine/log.h>

// class for displaying menus (main menu, settings, etc)
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
    fontTexture = ResourceManager::loadTexture("./assets/textures/font_atlas.png", "font_atlas");
    fontTexture.setFiltering(GL_NEAREST, GL_NEAREST);
}