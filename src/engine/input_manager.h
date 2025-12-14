#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include "input_state.h"

class InputManager {
public:
    InputManager();

    // Set input state before polling
    void prepareForUpdate();

    // Poll inputs and return quit status
    bool pollInputs();

    // Access general state of inputs
    [[nodiscard]] const InputState& getState() const;

    // Window resize detection
    bool hasWindowResized() const { return windowResized; }
    void getNewWindowSize(int& outWidth, int& outHeight) const {
        outWidth = newWindowWidth;
        outHeight = newWindowHeight;
    }

private:
    // Setup initial input state
    void init();

    // General state of inputs
    InputState inputState {};

    // Window resize tracking
    bool windowResized = false;
    int newWindowWidth = 0;
    int newWindowHeight = 0;
};

#endif