#include <game/chess/gamestate_chess.h>
#include <engine/resource_manager.h>
#include <game/game.h>
#include <engine/log.h>

GameStateChess::GameStateChess(
        std::shared_ptr<AtlasRenderer> _aRenderer
) : mapRenderer(std::move(_aRenderer)) {
}

GameStateChess::~GameStateChess() {
}

void GameStateChess::setGame(IGame* _game) {
    game = dynamic_cast<Game*>(_game);
}

void GameStateChess::load() {
    screenWidth = game->windowWidth / 2;
    screenHeight = game->windowHeight;

    
}

void GameStateChess::clean() {
}

void GameStateChess::pause() {
}

void GameStateChess::resume() {
}

void GameStateChess::handleEvent(const InputState& inputState) {
    
}

void GameStateChess::update(unsigned int dt) {
    
}

void GameStateChess::draw() {

}



void GameStateChess::onResize(int newWidth, int newHeight) {
    screenWidth = newWidth;
    screenHeight = newHeight;
}

bool GameStateChess::isGameOver() {
    return true;
}