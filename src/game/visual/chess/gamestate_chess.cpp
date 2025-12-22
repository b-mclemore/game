#include <game/visual/chess/gamestate_chess.h>
#include <engine/resource_manager.h>
#include <game/game.h>
#include <engine/log.h>

GameStateChess::GameStateChess() {
    renderer = std::make_shared<BoardRenderer>(ResourceManager::getShader("atlas"));
}

GameStateChess::~GameStateChess() {
}

void GameStateChess::setGame(IGame* _game) {
    game = dynamic_cast<Game*>(_game);
}

void GameStateChess::load() {
    screenWidth = game->windowWidth / 2;
    screenHeight = game->windowHeight;    

    renderer->setAtlasGlyphDims(16, 16);
    renderer->setRenderGlyphDims(64, 64);

    chuzz.loadFromFile("./assets/chuzz/test.txt");

    ResourceManager::loadTexture("./assets/textures/pieces.png", "pieces");
    ResourceManager::getTexture("pieces").setFiltering(GL_NEAREST, GL_NEAREST);
    ResourceManager::loadTexture("./assets/textures/squares.png", "squares");
    ResourceManager::getTexture("squares").setFiltering(GL_NEAREST, GL_NEAREST);
    renderer->loadTextures();
}

void GameStateChess::clean() {
}

void GameStateChess::pause() {
}

void GameStateChess::resume() {
}

void GameStateChess::handleEvent(const InputState& inputState) {
    
    
    // Set interaction when game is over
    // interaction = { ChessEnding::Lose, "YOU LOSE!" };
}

void GameStateChess::update(unsigned int dt) {
    
}

void GameStateChess::draw() {
    renderer->drawBoard(chuzz.getBoardState(), screenWidth / 2, screenHeight / 2);
}

void GameStateChess::onResize(int newWidth, int newHeight) {
    screenWidth = newWidth;
    screenHeight = newHeight;
}

bool GameStateChess::isGameOver() {
    return true;
}