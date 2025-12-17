#ifndef GAMESTATE_MENU_H
#define GAMESTATE_MENU_H

#include <engine/gamestate.h>
#include <game/game.h>

// class for displaying menus (main menu, settings, etc)
class GameStateMenu : public GameState {
public:
    GameStateMenu(
            std::shared_ptr<SpriteRenderer>,
            std::shared_ptr<GeometryRenderer>,
            std::shared_ptr<TextRenderer>
    );

    ~GameStateMenu() override;

    GameStateMenu() = delete;

    void load() override;

    void pause() override {}
    void resume() override {}

    void setGame(IGame*) override;

protected:
    std::shared_ptr<SpriteRenderer> sRenderer;
    std::shared_ptr<GeometryRenderer> gRenderer;
    std::shared_ptr<TextRenderer> tRenderer;

    Texture2D fontTexture;

    Game* game { nullptr };
    int screenWidth { 0 };
    int screenHeight { 0 };

    uint selectedOption = 0;
    size_t numOptions = 0;
};

#endif
