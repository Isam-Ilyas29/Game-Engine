#include "Input/input.hpp"

#include "Context/context.hpp"

#include <GLFW/glfw3.h>


namespace {

    input::states state;
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
    states const& getState() {
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
    f32 xOffset;
    f32 yOffset;

    void onScroll(f32 xoffset, f32 yoffset) {
        xOffset = xoffset;
        yOffset = yoffset;
    }

    f32 getXOffset() {
        return xOffset;
    }
    f32 getYOffset() {
        return yOffset;
    }


    // Cursor
    f64 xPos;
    f64 yPos;

    void onCursorPos(f32 xpos, f32 ypos) {
        xPos = xpos;
        yPos = ypos;
    }

    f32 getXPos() {
        return xPos;
    }
    f32 getYPos() {
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
        xOffset = 0.f;
        yOffset = 0.f;
    }

} // namespace input
