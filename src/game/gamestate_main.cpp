#include <game/gamestate_main.h>
#include <engine/resource_manager.h>
#include <game/game.h>
#include <engine/log.h>

GameStateMain::GameStateMain(
        std::shared_ptr<SpriteRenderer> _sRenderer,
        std::shared_ptr<GeometryRenderer> _gRenderer,
        std::shared_ptr<TextRenderer> _tRenderer
) : sRenderer(std::move(_sRenderer)), gRenderer(std::move(_gRenderer)), tRenderer(std::move(_tRenderer)) {
    vis = new GameStateVisual(sRenderer, gRenderer);
    txt = new GameStateTextual(sRenderer, gRenderer, tRenderer);
}

GameStateMain::~GameStateMain() {
}

void GameStateMain::setGame(IGame* _game) {
    game = dynamic_cast<Game*>(_game);
    vis->setGame(_game);
    txt->setGame(_game);
}

void GameStateMain::load() {
    screenWidth = game->windowWidth;
    screenHeight = game->windowHeight;
    vis->load();
    txt->load();
}

void GameStateMain::clean() {
    vis->clean();
    txt->clean();
}

void GameStateMain::pause() {
    vis->pause();
    txt->pause();
}

void GameStateMain::resume() {
    vis->resume();
    txt->resume();
}

void GameStateMain::handleEvent(const InputState& inputState) {
    // toggle if tab is pressed, otherwise pass handling to toggled state
    if (inputState.keyboardState.isJustPressed(SDL_Scancode(SDL_SCANCODE_TAB))) {
        isVisToggled = !isVisToggled;
    }
    else if (isVisToggled) {
        vis->handleEvent(inputState);
    } else {
        txt->handleEvent(inputState);
    }
}

void GameStateMain::update(unsigned int dt) {
    if (isVisToggled) {
        vis->update(dt);
    } else {
        txt->update(dt);
    }
}

void GameStateMain::draw() {
    vis->draw();
    txt->draw();
}

void GameStateMain::onResize(int newWidth, int newHeight) {
    screenWidth = newWidth;
    screenHeight = newHeight;
    vis->onResize(newWidth, newHeight);
    txt->onResize(newWidth, newHeight);
}
