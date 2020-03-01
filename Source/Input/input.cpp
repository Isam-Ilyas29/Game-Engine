#include "input.hpp"

#include "../Context/context.hpp"

#include <GLFW/glfw3.h>


namespace {

    input::keyStates state;
}

namespace input {

    // Keys and Mouse buttons
    void onButton(u16 key, u16 action) {
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

    bool isPressed(u16 key) {
        return state.pressed.find(key) != state.pressed.end();
    }
    bool isHeld(u16 key) {
        return state.held.find(key) != state.held.end();
    }
    bool isReleased(u16 key) {
        return state.released.find(key) != state.released.end();
    }


    // Scroll
    f64 xOffset;
    f64 yOffset;

    void onScroll(f64 xoffset, f64 yoffset) {
        xOffset = xoffset;
        yOffset = yoffset;
    }

    f64 getXOffset() {
        return xOffset;
    }
    f64 getYOffset() {
        return yOffset;
    }


    // Cursor
    f64 xPos;
    f64 yPos;

    void onCursorPos(f64 xpos, f64 ypos) {
        xPos = xpos;
        yPos = ypos;
    }

    f64 getXPos() {
        return xPos;
    }
    f64 getYPos() {
        return yPos;
    }


    // End frame
    void endFrame() {
        // Buttons
        state.released.clear();

        for (auto i = state.pressed.cbegin(); i != state.pressed.cend(); i++) {
            state.held.insert(*i);
        }

        state.pressed.clear();

        // Scroll
        xOffset = 0.0;
        yOffset = 0.0;
    }

} // namespace input
