
#ifndef GAMESTATE_ROOM_H
#define GAMESTATE_ROOM_H

#include <optional>
#include <map>
#include <engine/gamestate.h>
#include <game/game.h>
#include <game/visual/room/tilemap.h>
#include <game/visual/room/player.h>
#include <game/visual/room/npc.h>

// room state for visual screen

// Room scene of the game, contains grid-based room movement
class GameStateRoom : public GameState {
public:
    GameStateRoom(
            std::shared_ptr<AtlasRenderer>
    );

    ~GameStateRoom() override;

    GameStateRoom() = delete;

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
    // Gets the player direction according to key
    Facing getFacingFromKey(const InputState& inputState);
    
    void tryStartMovePlayer(Facing f);

    std::optional<InteractionResult> interaction;

private:
    // Movement keys
    int moveLeftKey { 0 };
    int moveRightKey { 0 };
    int moveUpKey { 0 };
    int moveDownKey { 0 };
    // Movement map, holds (dx, dy)
    std::map<Facing, std::pair<int, int>> movementMap = {
        { Facing::N, {0, 1}},
        { Facing::W, {-1, 0}},
        { Facing::E, {1, 0}},
        { Facing::S, {0, -1}},
    };

    Player *player = new Player();
    bool lastSwungLeft = true;
    std::vector<Npc> npcs = std::vector<Npc>();

    // renderer for character
    std::shared_ptr<AtlasRenderer> characterRenderer;

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
    static constexpr float MOVEMENT_DELAY = 250.0f;  // Milliseconds between moves

    void drawCharacter(Character &p, int px, int py, const Texture2D &texture);
    void drawMap();
    void drawNpcs();
    bool isValidPosition(int x, int y);
    Vector2 calculateCameraPosition();
    void toggleAnimationFrame();
};

#endif
