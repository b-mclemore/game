
#ifndef GAMESTATE_CHESS_H
#define GAMESTATE_CHESS_H

#include <optional>
#include <engine/gamestate.h>
#include <game/game.h>
#include <game/visual/chess/chesspuzzle.h>
#include <game/visual/chess/renderer_board.h>

enum class ChessEnding {
    Win,
    Loss,
    Draw,
    Quit,
    ContinuePlaying
};

struct ChessResult {
    ChessEnding type;
    std::string dialog;
};

// chess minigame
class GameStateChess : public GameState {
public:
    GameStateChess();

    ~GameStateChess() override;

    void load() override;

    void clean() override;

    void pause() override;

    void resume() override;

    void handleEvent(const InputState&) override;

    void update(unsigned int dt) override;

    void draw() override;

    void setGame(IGame*) override;

    void onResize(int newWidth, int newHeight) override;

    bool isGameOver();

    std::optional<ChessResult> interaction;

    void handleCommand(std::string command);

private:
    // renderer for board and pieces
    std::shared_ptr<BoardRenderer> renderer;

    Game* game { nullptr };
    int screenWidth { 0 };
    int screenHeight { 0 };

    // Grid settings
    static constexpr int GRID_SIZE = 64;  // Size of each grid cell in pixels

    // puzzle logic/state
    ChessPuzzle chuzz;

    // Animation state
    bool isAnimating = false;
    float animProgress = 0.0f;           // [0, 1] animation progress
    Vector2 animFrom;                     // Source position in screen coordinates
    Vector2 animTo;                       // Destination position in screen coordinates
    Move currentMove;                     // The move being animated
    int animPieceIdx;                     // Which piece type (0-11) is animating
    bool isMoveCorrect = false;          // True if move matches solution

    // Effect state
    bool isShaking = false;
    float shakeProgress = 0.0f;
    bool showSuccessSquare = false;
    U64 successSquare = 0;
    float successDelayProgress = 0.0f;
    bool waitingForSuccessDelay = false;

    // Animation timing constants
    static constexpr float CHESS_MOVE_DELAY = 300.0f;  // milliseconds
    static constexpr float SHAKE_DURATION = 200.0f;    // milliseconds
    static constexpr float SUCCESS_DELAY = 500.0f;     // milliseconds to show success before ending

    // Placeholder animation triggers
    void triggerScreenShake();
    void triggerSuccessEffect();
};

#endif
