#include <game/gamestate_main.h>
#include <engine/resource_manager.h>
#include <game/game.h>
#include <engine/log.h>

GameStateMain::GameStateMain(
        std::shared_ptr<AtlasRenderer> _aRenderer,
        std::shared_ptr<TextRenderer> _tRenderer
) : aRenderer(std::move(_aRenderer)),
    tRenderer(std::move(_tRenderer)) {
    vis = new GameStateVisual(aRenderer);
    txt = new GameStateTextual(tRenderer);
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

void GameStateMain::toggleVis() {
    isVisToggled = !isVisToggled;
    if (isVisToggled) txt->pause();
    else txt->resume();
}

void GameStateMain::handleEvent(const InputState& inputState) {
    // toggle if tab is pressed, otherwise pass handling to toggled state
    if (inputState.keyboardState.isJustPressed(SDL_Scancode(SDL_SCANCODE_TAB))) {
        toggleVis();
    }
    else if (isVisToggled) {
        // if vis decides to print to console, catch and add the message,
        // then toggle
        vis->handleEvent(inputState);
        if (vis->should_print) {
            txt->addLine(vis->msg, sourceEnum::npcSource);
        }
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
