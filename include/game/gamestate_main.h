#ifndef GAMESTATE_MAIN_H
#define GAMESTATE_MAIN_H

#include "../engine/gamestate.h"
#include "game.h"
#include "tilemap.h"

// Player structure for grid-based movement
struct Player {
    int x;  // Grid x position
    int y;  // Grid y position
};

// Main scene of the game, contains grid-based room movement
class GameStateMain : public GameState {
public:
    GameStateMain(
            std::shared_ptr<SpriteRenderer>,
            std::shared_ptr<GeometryRenderer>
    );

    ~GameStateMain() override;

    GameStateMain() = delete;

    void load() override;

    void clean() override;

    void pause() override;

    void resume() override;

    void handleEvent(const InputState&) override;

    void update(unsigned int dt) override;

    void draw() override;

    void setGame(IGame*) override;

    void onResize(int newWidth, int newHeight) override;

private:
    // Movement keys
    int moveLeftKey { 0 };
    int moveRightKey { 0 };
    int moveUpKey { 0 };
    int moveDownKey { 0 };

    std::shared_ptr<SpriteRenderer> sRenderer;
    std::shared_ptr<GeometryRenderer> gRenderer;

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
