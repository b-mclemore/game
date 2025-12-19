#include <game/visual/gamestate_visual.h>
#include <engine/resource_manager.h>
#include <game/game.h>
#include <engine/log.h>

GameStateVisual::GameStateVisual(
        std::shared_ptr<AtlasRenderer> _aRenderer
) : renderer(std::move(_aRenderer)) {
    ch = new GameStateChess(_aRenderer);
    room = new GameStateRoom(_aRenderer);
}

GameStateVisual::~GameStateVisual() {
}

void GameStateVisual::setGame(IGame* _game) {
    game = dynamic_cast<Game*>(_game);
    ch->setGame(_game);
    room->setGame(_game);
}

void GameStateVisual::load() {
    screenWidth = game->windowWidth / 2;
    screenHeight = game->windowHeight;
    ch->load();
    room->load();
}

void GameStateVisual::clean() {
    ch->clean();
    room->clean();
}

void GameStateVisual::pause() {
    ch->pause();
    room->pause();
}

void GameStateVisual::resume() {
    ch->pause();
    room->pause();
}

void GameStateVisual::handleEvent(const InputState& inputState) {
    switch (visMode) {
        case (GameMode::Room):
            room->handleEvent(inputState);
            break;
        case (GameMode::Chess):
            ch->handleEvent(inputState);
            break;
    }
    // Handle switching context
    if (room->interaction) {
        auto result = *room->interaction;
        room->interaction.reset();
        dialog = result.dialog;
        if (result.type == InteractionType::Chess) visMode = GameMode::Chess;
    } else if (ch->interaction) {
        auto result = *ch->interaction;
        ch->interaction.reset();
        dialog = result.dialog;
        visMode = GameMode::Room;
    }
}

void GameStateVisual::update(unsigned int dt) {
    switch (visMode) {
        case (GameMode::Room):
            return room->update(dt);
        case (GameMode::Chess):
            return ch->update(dt);
    }
}

void GameStateVisual::draw() {
    switch (visMode) {
        case (GameMode::Room):
            return room->draw();
        case (GameMode::Chess):
            return ch->draw();
    }
}

void GameStateVisual::onResize(int newWidth, int newHeight) {
    screenWidth = newWidth;
    screenHeight = newHeight;
    ch->onResize(newWidth, newHeight);
    room->onResize(newWidth, newHeight);
}
