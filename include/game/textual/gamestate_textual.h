#ifndef GAMESTATE_TEXTUAL_H
#define GAMESTATE_TEXTUAL_H

#include <engine/gamestate.h>
#include <engine/console_buffer.h>
#include <game/game.h>

// right side of screen, text

// Textual scene of the game, contains "console"
class GameStateTextual : public GameState {
public:
    GameStateTextual(
            std::shared_ptr<SpriteRenderer>,
            std::shared_ptr<GeometryRenderer>,
            std::shared_ptr<TextRenderer>
    );

    ~GameStateTextual() override;

    GameStateTextual() = delete;

    void load() override;

    void clean() override;

    void pause() override;

    void resume() override;

    void handleEvent(const InputState&) override;

    void update(unsigned int dt) override;

    void draw() override;

    void setGame(IGame*) override;

    void onResize(int newWidth, int newHeight) override;

    // wrapper for cbuff so that we can add lines from outside this gamestate (i.e. in gamestatemain)
    void addLine(const std::string& s, const sourceEnum& src);

private:
    std::shared_ptr<SpriteRenderer> sRenderer;
    std::shared_ptr<GeometryRenderer> gRenderer;
    std::shared_ptr<TextRenderer> tRenderer;
    
    Texture2D fontTexture;

    Game* game { nullptr };
    int screenWidth { 0 };
    int screenHeight { 0 };
    int vSpacing { 4 };
    int offset { 0 };

    static constexpr int GRID_SIZE = 32;  // Size of each grid cell in pixels

    // buffer for text rendering
    ConsoleBuffer *cbuff;

    Color consoleColor;
    Color textColor;
    Color genericColor;
    Color npcColor;

    void drawText();
    // wrapper for whatever controller class we want to create later.
    // for now it just quits out.
    void handleCommand(const std::string& command);
};

#endif
