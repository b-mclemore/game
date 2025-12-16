#include <engine/input_manager.h>
#include <cstring>

InputManager::InputManager() {
    init();
}

void InputManager::init() {
    // Keyboard

    // Assign current state pointer
    inputState.keyboardState.currentValue = SDL_GetKeyboardState(nullptr);
    // Clear previous state memory
    memset(inputState.keyboardState.previousValue, 0, SDL_NUM_SCANCODES);
}


void InputManager::prepareForUpdate() {
    // Keyboard: Copy current state to previous
    memcpy(inputState.keyboardState.previousValue, inputState.keyboardState.currentValue, SDL_NUM_SCANCODES);
    // Reset resize flag each frame
    windowResized = false;
}

bool InputManager::pollInputs() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                return false;

            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    windowResized = true;
                    newWindowWidth = event.window.data1;
                    newWindowHeight = event.window.data2;
                }
                break;

            default:
                break;
        }
    }
    return true;
}

const InputState& InputManager::getState() const {
    return inputState;
}