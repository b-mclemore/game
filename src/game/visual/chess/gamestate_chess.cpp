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

    chuzz.loadFromFile("./assets/chuzz/mateinone.txt");

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
    
    if (inputState.keyboardState.isJustPressed(SDL_SCANCODE_RETURN)) {
        auto next_move = chuzz.getNextMove();
        interaction = { ChessEnding::Quit, "The next move was "+next_move };
    }
    // Set interaction when game is over
    // interaction = { ChessEnding::Lose, "YOU LOSE!" };
}

void GameStateChess::update(unsigned int dt) {
    // Update piece movement animation
    if (isAnimating) {
        // Advance animation
        animProgress += dt / CHESS_MOVE_DELAY;
        float t = std::min(animProgress, 1.0f);

        // Check if animation complete
        if (t >= 1.0f) {
            isAnimating = false;

            if (isMoveCorrect) {
                triggerSuccessEffect();
                chuzz.makeMove(currentMove);
                // Start delay before setting interaction
                // so that the success effect is visible
                waitingForSuccessDelay = true;
                successDelayProgress = 0.0f;
            } else {
                // failure means no move
                interaction = { ChessEnding::ContinuePlaying, "Sorry, that's the wrong move" };
                triggerScreenShake();
            }
        }
    }

    // Update success delay
    if (waitingForSuccessDelay) {
        successDelayProgress += dt / SUCCESS_DELAY;
        if (successDelayProgress >= 1.0f) {
            waitingForSuccessDelay = false;
            interaction = { ChessEnding::Win, "That's correct!" };
        }
    }

    // Update screen shake effect
    if (isShaking) {
        shakeProgress += dt / SHAKE_DURATION;
        if (shakeProgress >= 1.0f) {
            isShaking = false;
            shakeProgress = 0.0f;
        }
    }
}

void GameStateChess::draw() {
    int centerX = screenWidth / 2;
    int centerY = screenHeight / 2;

    // Calculate shake offset
    int shakeOffsetX = 0;
    int shakeOffsetY = 0;
    if (isShaking) {
        // Shake diminishes over time
        float intensity = (1.0f - shakeProgress) * 8.0f;
        // Alternate direction based on progress
        float phase = shakeProgress * 20.0f;  // Multiple shakes
        shakeOffsetX = (int)(sin(phase) * intensity);
        shakeOffsetY = (int)(cos(phase * 1.3f) * intensity);
    }

    if (!isAnimating) {
        // Normal rendering
        renderer->drawBoard(
            chuzz.getBoardState(),
            centerX + shakeOffsetX,
            centerY + shakeOffsetY,
            showSuccessSquare,
            successSquare
        );
    } else {
        // Animation rendering
        renderer->drawBoardWithAnimation(
            chuzz.getBoardState(),
            centerX + shakeOffsetX,
            centerY + shakeOffsetY,
            isAnimating,
            animPieceIdx,
            currentMove.source_bb,
            animFrom,
            animTo,
            animProgress,
            isMoveCorrect
        );
    }
}

void GameStateChess::onResize(int newWidth, int newHeight) {
    screenWidth = newWidth;
    screenHeight = newHeight;
}

bool GameStateChess::isGameOver() {
    return true;
}

void GameStateChess::triggerScreenShake() {
    isShaking = true;
    shakeProgress = 0.0f;
}

void GameStateChess::triggerSuccessEffect() {
    showSuccessSquare = true;
    successSquare = currentMove.dest_bb;
}

void GameStateChess::handleCommand(std::string command) {
    // Validate move format
    if (command.length() > 5) return;
    if (stringBoardMap.find(command.substr(0, 2)) == stringBoardMap.end() ||
        stringBoardMap.find(command.substr(2, 2)) == stringBoardMap.end())
        return;

    // Block new moves while animating
    if (isAnimating) return;

    // Parse move
    Move m = stringToMove(command);
    Move cm = stringToMove(chuzz.getNextMove());

    // Determine if move is correct
    isMoveCorrect = (m.source_bb == cm.source_bb && m.dest_bb == cm.dest_bb);

    // Find which piece is moving (search current board state BEFORE making the move)
    animPieceIdx = -1;
    const auto& pieceBoards = chuzz.getBoardState().getPieceBoards();
    for (int pc_idx = 0; pc_idx < 12; pc_idx++) {
        if (m.source_bb & pieceBoards[pc_idx]) {
            animPieceIdx = pc_idx;
            break;
        }
    }

    // Validate piece exists at source
    if (animPieceIdx == -1) {
        interaction = { ChessEnding::ContinuePlaying, "There's no piece there!" };
        return;
    }

    // Store move and calculate screen positions
    currentMove = m;
    int boardCenterX = screenWidth / 2;
    int boardCenterY = screenHeight / 2;
    animFrom = renderer->getSquareScreenPos(m.source_bb, boardCenterX, boardCenterY);
    animTo = renderer->getSquareScreenPos(m.dest_bb, boardCenterX, boardCenterY);

    // Start animation
    isAnimating = true;
    animProgress = 0.0f;
}