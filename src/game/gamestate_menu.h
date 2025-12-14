#ifndef GAMESTATE_MENU_H
#define GAMESTATE_MENU_H

#include "../engine/gamestate.h"
#include "game.h"

enum MenuOptions {start, quit};

class GameStateMenu : public GameState {
public:
    GameStateMenu(
            std::shared_ptr<SpriteRenderer>,
            std::shared_ptr<GeometryRenderer>
    );

    ~GameStateMenu() override;

    GameStateMenu() = delete;

    void load() override;

    void clean() override;

    void pause() override;

    void resume() override;

    void handleEvent(const InputState&) override;

    void update(unsigned int dt) override;

    void draw() override;

    void setGame(IGame*) override;

private:
    std::shared_ptr<SpriteRenderer> sRenderer;
    std::shared_ptr<GeometryRenderer> gRenderer;

    Game* game { nullptr };
    int screenWidth { 0 };
    int screenHeight { 0 };

    MenuOptions selectedOption { MenuOptions::start };
};

#endif
