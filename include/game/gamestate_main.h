#ifndef GAMESTATE_MAIN_H
#define GAMESTATE_MAIN_H

#include "../engine/gamestate.h"
#include "game.h"
#include "tilemap.h"
#include <game/gamestate_textual.h>
#include <game/gamestate_visual.h>

// Main scene of the game, essentially a wrapper for the 
// left/right Visual/Textual gamestates
class GameStateMain : public GameState {
public:
    GameStateMain(
            std::shared_ptr<SpriteRenderer>,
            std::shared_ptr<GeometryRenderer>,
            std::shared_ptr<TextRenderer>
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
    std::shared_ptr<SpriteRenderer> sRenderer;
    std::shared_ptr<GeometryRenderer> gRenderer;
    std::shared_ptr<TextRenderer> tRenderer;

    Game* game { nullptr };
    int screenWidth { 0 };
    int screenHeight { 0 };

    // Grid settings
    static constexpr int GRID_SIZE = 32;  // Size of each grid cell in pixels

    GameStateVisual *vis;
    GameStateTextual *txt;
    bool isVisToggled = false; // toggle between vis and text
};

#endif
