
#ifndef GAMESTATE_CHESS_H
#define GAMESTATE_CHESS_H

#include <optional>
#include <engine/gamestate.h>
#include <game/game.h>

enum class ChessEnding {
    Win,
    Loss,
    Draw,
    Quit
};

struct ChessResult {
    ChessEnding type;
    std::string dialog;
};

// chess minigame
class GameStateChess : public GameState {
public:
    GameStateChess(
            std::shared_ptr<AtlasRenderer>
    );

    ~GameStateChess() override;

    GameStateChess() = delete;

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

private:
    // renderer for map
    std::shared_ptr<AtlasRenderer> mapRenderer;

    Game* game { nullptr };
    int screenWidth { 0 };
    int screenHeight { 0 };

    // Grid settings
    static constexpr int GRID_SIZE = 64;  // Size of each grid cell in pixels
};

#endif
