#ifndef GAMESTATE_MAINMENU_H
#define GAMESTATE_MAINMENU_H

#include <engine/gamestate.h>
#include <game/game.h>
#include <game/menus/gamestate_menu.h>

class GameStateMainMenu : public GameStateMenu {
public:
    GameStateMainMenu(
            std::shared_ptr<AtlasRenderer>,
            std::shared_ptr<TextRenderer>
    );

    ~GameStateMainMenu() override;

    GameStateMainMenu() = delete;

    void load() override;

    void clean() override;

    void handleEvent(const InputState&) override;

    void update(unsigned int dt) override;

    void draw() override;

private:
};

#endif
