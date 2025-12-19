
#ifndef GAMESTATE_VISUAL_H
#define GAMESTATE_VISUAL_H

#include <optional>
#include <engine/gamestate.h>
#include <game/game.h>
#include <game/tilemap.h>
#include <game/visual/player.h>
#include <game/visual/npc.h>

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

    // Simple boolean check for whether a movekey isJustPressed
    bool movementJustPressed();
    // Simple boolean check that sees if any of the movekeys are isDown
    bool movementKeyDown();
    // Moves the player according to key
    void movePlayer(const InputState& inputState);

    std::optional<InteractionResult> interaction;

private:
    // Movement keys
    int moveLeftKey { 0 };
    int moveRightKey { 0 };
    int moveUpKey { 0 };
    int moveDownKey { 0 };
    // Movement map, holds (dx, dy)
    std::vector<std::pair<int, int>> movementMap = {
        {0, 1},  // N
        {-1, 0}, // W
        {1, 0},  // E
        {0, -1}  // S
    };

    Player *player = new Player();
    std::vector<Npc> npcs = std::vector<Npc>();

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
    void drawNpcs();
    bool isValidPosition(int x, int y);
    Vector2 calculateCameraPosition();
};

#endif
