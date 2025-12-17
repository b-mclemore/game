#include <game/textual/gamestate_textual.h>
#include <engine/resource_manager.h>
#include <game/game.h>
#include <engine/log.h>
#include <game/textual/text_colors.h>

GameStateTextual::GameStateTextual(
        std::shared_ptr<SpriteRenderer> _sRenderer,
        std::shared_ptr<GeometryRenderer> _gRenderer,
        std::shared_ptr<TextRenderer> _tRenderer
) : sRenderer(std::move(_sRenderer)), gRenderer(std::move(_gRenderer)), tRenderer(std::move(_tRenderer)) {
}

GameStateTextual::~GameStateTextual() {
}

void GameStateTextual::setGame(IGame* _game) {
    game = dynamic_cast<Game*>(_game);
}

void GameStateTextual::load() {
    screenWidth = game->windowWidth / 2;
    screenHeight = game->windowHeight;
    offset = game->windowWidth / 2;
    cbuff = new ConsoleBuffer(screenWidth / GRID_SIZE - 2, screenHeight / (GRID_SIZE + vSpacing), GRID_SIZE);
    fontTexture = ResourceManager::loadTexture("./assets/textures/font_atlas.png", "font_atlas");
    fontTexture.setFiltering(GL_NEAREST, GL_NEAREST);
}

void GameStateTextual::clean() {
}

void GameStateTextual::pause() {
    is_active = false;
}

void GameStateTextual::resume() {
    is_active = true;
}

void GameStateTextual::handleEvent(const InputState& inputState) {
    // RETURN executes the command
    if (inputState.keyboardState.isJustPressed(SDL_SCANCODE_RETURN)) {
        std::string currLine = cbuff->getActiveLine();
        cbuff->nextLine(sourceEnum::playerSource);
        handleCommand(currLine);
    } 
    // DEL deletes the last character entered
    else if (inputState.keyboardState.isJustPressed(SDL_SCANCODE_BACKSPACE)) {
        cbuff->deleteChar();
    // UP ARROW copies the last used player command
    } else if (inputState.keyboardState.isJustPressed(SDL_SCANCODE_UP)) {
        // cbuff->previousCommand();
    }
    // otherwise, ASCII if possible
    else {
        for (int sc = 4; sc < 29; sc++) {
            if (inputState.keyboardState.isJustPressed((SDL_Scancode)sc)) {
                cbuff->addChar((char)(sc + 93));
            }
        }
    }
}

void GameStateTextual::update(unsigned int dt) {
}

void GameStateTextual::draw() {
    drawText();
}

void GameStateTextual::drawText() {
    auto lines = cbuff->getAllLines();
    int lineNum = 0;
    for (auto& [l, c] : lines) {
        if (lineNum == 0) {
            auto col = getColor(sourceEnum::consoleSource, is_active);
            tRenderer->drawText(fontTexture, " >"+l, Vector2(offset, 0), col);
        }
        else {
            auto col = getColor(c, is_active);
            tRenderer->drawText(fontTexture, "  "+l, Vector2(offset, lineNum * (GRID_SIZE + vSpacing)), col);
        }
        lineNum += 1;
    }
}

// Add line wrapper
void GameStateTextual::addLine(const std::string& s, const sourceEnum& src) {
    cbuff->addLine(s, src);
}

void GameStateTextual::onResize(int newWidth, int newHeight) {
    screenWidth = newWidth;
    screenHeight = newHeight;
    offset = newWidth / 2;
    LOG(Error) << "Console cannot handle resizing yet";
    exit(1);
}

std::string cleanUpCommand(const std::string& command) {
    // strip leading/trailing whitespace from command
    size_t first = command.find_first_not_of(" \t\n\r\f\v");
    if (first == std::string::npos)
        return ""; // string is all whitespace
    size_t last = command.find_last_not_of(" \t\n\r\f\v");
    return command.substr(first, last - first + 1);
}

void GameStateTextual::handleCommand(const std::string& command) {
    auto c = cleanUpCommand(command);
    if (c == "quit" || c == "q") {
        game->gameIsRunning = false;
    } 
    else if (c == "help" || c == "h") {
        cbuff->addLine("Press tab to switch.", sourceEnum::genericSource);
        cbuff->addLine("Use WASD to move.", sourceEnum::genericSource);
        cbuff->addLine("Press ENTER to interact.", sourceEnum::genericSource);
        cbuff->addLine("Enter 'h' for help.", sourceEnum::genericSource);
        cbuff->addLine("Enter 'q' to quit.", sourceEnum::genericSource);
    } else if (c == "activate") {
        cbuff->addLine("SYSTEM ACTIVATED!!!", sourceEnum::genericSource);
    }
}