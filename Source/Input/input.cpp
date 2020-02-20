#include "input.h"

#include "../Context/context.h"

#include <GLFW/glfw3.h>


namespace {

    input::keyStates state;
}

namespace input {

    // Keys and Mouse buttons
    void endFrame() {
        state.released.clear();

        for (auto i = state.pressed.cbegin(); i != state.pressed.cend(); i++) {
            state.held.insert(*i);
        }

        state.pressed.clear();
    }
    void onInput(int key, int action) {
        switch (action) {

        case GLFW_PRESS:
            state.released.erase(key);

            state.pressed.insert(key);

            break;

        case GLFW_RELEASE:
            state.held.erase(key);
            state.pressed.erase(key);

            state.released.insert(key);

            break;

        default:
            break;
        }
    }
    keyStates const& getState() {
        return state;
    }

    bool isPressed(int key) {
        return state.pressed.find(key) != state.pressed.end();
    }
    bool isHeld(int key) {
        return state.held.find(key) != state.held.end();
    }
    bool isReleased(int key) {
        return state.released.find(key) != state.released.end();
    }

    // Curosr
    double xPos;
    double yPos;

    void onCursorPos(double xpos, double ypos) {
        xPos = xpos;
        yPos = ypos;
    }

    double getXPos() {
        return xPos;
    }
    double getYPos() {
        return yPos;
    }

    // Scroll
    double xOffset;
    double yOffset;

    void onScroll(double xoffset, double yoffset) {
        xOffset = xoffset;
        yOffset = yoffset;
    }

    double getXOffset() {
        return xOffset;
    }
    double getYOffset() {
        return yOffset;
    }
}

