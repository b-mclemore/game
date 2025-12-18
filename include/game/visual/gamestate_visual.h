
#ifndef GAMESTATE_VISUAL_H
#define GAMESTATE_VISUAL_H

#include <engine/gamestate.h>
#include <game/game.h>
#include <game/tilemap.h>
#include <game/visual/player.h>

// left side of screen, visual

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

    Player *player = new Player();

    // renderer for character
    std::shared_ptr<AtlasRenderer> playerRenderer;

    // renderer for map
    std::shared_ptr<AtlasRenderer> mapRenderer;

    Game* game { nullptr };
    int screenWidth { 0 };
    int screenHeight { 0 };

    // Grid settings
    static constexpr int GRID_SIZE = 64;  // Size of each grid cell in pixels

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
