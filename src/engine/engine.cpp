#include <engine/engine.h>
#include <engine/log.h>

Engine::Engine(std::unique_ptr<IGame>&& _game, std::string _title, int _screenWidth, int _screenHeight) :
        game(std::move(_game)),
        title(std::move(_title)),
        screenWidth(_screenWidth),
        screenHeight(_screenHeight) {
    bool fullscreen = true;
    window = IWindow::create(title);
    if (!window->init(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, fullscreen)) {
        LOG(Warning) << "Window failed to properly initialize";
        return;
    }
    if (fullscreen) {
        window->getDrawableSize(screenWidth, screenHeight);
        LOG(Info) << "Got fullscreen dims (" << screenWidth << ", " << screenHeight << ")";
    }
    window->logGlParams();
}


void Engine::start() {
    // Main game elements loading
    LOG(Info) << "Starting with dims (" << screenWidth << ", " << screenHeight << ")";
    game->init(screenWidth, screenHeight);
    game->load();
}

void Engine::run() {
    // Game loop
    while (game->isRunning()) {
        dt = timer.computeDeltaTime();
        window->updateFpsCounter(dt);

        game->handleInputs();
        game->update(dt);

        window->clear();
        game->render();
        window->swapBuffer();

        // Delay frame if game runs too fast
        timer.delayTime();
    }
}

void Engine::shutdown() {
    // Exit game
    game->clean();
    window->clean();
}