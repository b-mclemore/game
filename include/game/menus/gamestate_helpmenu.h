#ifndef GAMESTATE_HELPMENU_H
#define GAMESTATE_HELPMENU_H

#include <engine/gamestate.h>
#include <game/game.h>
#include <game/menus/gamestate_menu.h>

class GameStateHelpMenu : public GameStateMenu {
public:
    GameStateHelpMenu(
            std::shared_ptr<SpriteRenderer>,
            std::shared_ptr<GeometryRenderer>,
            std::shared_ptr<TextRenderer>
    );

    ~GameStateHelpMenu() override;

    GameStateHelpMenu() = delete;

    void clean() override;

    void handleEvent(const InputState&) override;

    void update(unsigned int dt) override;

    void draw() override;

private:
};

#endif