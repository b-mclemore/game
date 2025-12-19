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
    ch = new GameStateChess(aRenderer);
}

GameStateMain::~GameStateMain() {
}

void GameStateMain::setGame(IGame* _game) {
    game = dynamic_cast<Game*>(_game);
    vis->setGame(_game);
    txt->setGame(_game);
    ch->setGame(_game);
}

void GameStateMain::load() {
    screenWidth = game->windowWidth;
    screenHeight = game->windowHeight;
    vis->load();
    txt->load();
    ch->load();
}

void GameStateMain::clean() {
    vis->clean();
    txt->clean();
    ch->clean();
}

void GameStateMain::pause() {
    vis->pause();
    txt->pause();
    ch->pause();
}

void GameStateMain::resume() {
    vis->resume();
    txt->resume();
    ch->resume();
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
        switch (leftMode) {
            case LeftMode::Visual:
                vis->handleEvent(inputState);
                // if vis decides to print to console, catch and add the message,
                // then toggle
                if (vis->interaction) {
                    auto result = *vis->interaction;
                    vis->interaction.reset();

                    txt->addLine(result.dialog, sourceEnum::npcSource);

                    if (result.type == InteractionType::Chess) {
                        leftMode = LeftMode::Chess;
                    }
                }
                break;
            case LeftMode::Chess:
                ch->handleEvent(inputState);

                if (ch->isGameOver()) {
                    txt->addLine("Game over!", sourceEnum::npcSource);
                    leftMode = LeftMode::Visual;
                }
                break;
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
