#ifndef GAMESTATE_SETTINGSMENU_H
#define GAMESTATE_SETTINGSMENU_H

#include <engine/gamestate.h>
#include <game/game.h>
#include <game/menus/gamestate_menu.h>

class GameStateSettingsMenu : public GameStateMenu {
public:
    GameStateSettingsMenu(
            std::shared_ptr<SpriteRenderer>,
            std::shared_ptr<GeometryRenderer>,
            std::shared_ptr<TextRenderer>
    );

    ~GameStateSettingsMenu() override;

    GameStateSettingsMenu() = delete;

    void clean() override;

    void handleEvent(const InputState&) override;

    void update(unsigned int dt) override;

    void draw() override;

private:
};

#endif