
#ifndef GAMESTATE_VISUAL_H
#define GAMESTATE_VISUAL_H

#include <optional>
#include <engine/gamestate.h>
#include <game/game.h>
#include <game/visual/room/gamestate_room.h>
#include <game/visual/chess/gamestate_chess.h>

enum class GameMode {
    Room,
    Chess
};

// left side of screen, visual
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

    // if we want to send a dialog to be printed, set it here
    std::optional<std::string> dialog;

    // if the console wants to send a command to be received
    void handleCommand(std::string command);

private:
    std::shared_ptr<AtlasRenderer> renderer;
    Game* game { nullptr };
    int screenWidth { 0 };
    int screenHeight { 0 };

    GameStateChess *ch;
    GameStateRoom *room;

    GameMode visMode = GameMode::Room; // toggle between visual gametypes
};

#endif
