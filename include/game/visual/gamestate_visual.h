
#ifndef GAMESTATE_VISUAL_H
#define GAMESTATE_VISUAL_H

#include <engine/gamestate.h>
#include <game/game.h>
#include <game/tilemap.h>

// left side of screen, visual

// Player structure for grid-based movement
struct Player {
    int x;
    int y;
};

// Which direction the player faces
enum PlayerFacing {
    S = 0, // default
    N = 1,
    E = 2,
    W = 3
};

// Visual scene of the game, contains grid-based room movement
class GameStateVisual : public GameState {
public:
    GameStateVisual(
            std::shared_ptr<AtlasRenderer>
    );

    ~GameStateVisual() override;

    GameStateVisual() = delete;

    void load() override;

    void clean() override;

    void pause() override;

    void resume() override;

    void handleEvent(const InputState&) override;

    void update(unsigned int dt) override;

    void draw() override;

    void setGame(IGame*) override;

    void onResize(int newWidth, int newHeight) override;

    // Whether to print something to console after interaction, and what to print
    bool should_print = false;
    std::string msg;

private:
    // Movement keys
    int moveLeftKey { 0 };
    int moveRightKey { 0 };
    int moveUpKey { 0 };
    int moveDownKey { 0 };

    PlayerFacing dir = PlayerFacing::S;

    // renderer for character
    std::shared_ptr<AtlasRenderer> playerRenderer;

    // renderer for map
    std::shared_ptr<AtlasRenderer> mapRenderer;

    Game* game { nullptr };
    int screenWidth { 0 };
    int screenHeight { 0 };

    // Grid settings
    static constexpr int GRID_SIZE = 32;  // Size of each grid cell in pixels

    Player player {};

    // Map and camera
    std::unique_ptr<TileMap> tileMap;
    Vector2 cameraPos;

    // Movement timing
    float movementAccumulator { 0.0f };
    static constexpr float MOVEMENT_DELAY = 150.0f;  // Milliseconds between moves

    void drawPlayer();
    void drawMap();
    bool isValidPosition(int x, int y);
    Vector2 calculateCameraPosition();
};

#endif
