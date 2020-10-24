#include "Input/input.hpp"

#include "Context/context.hpp"

#include <GLFW/glfw3.h>


namespace {

    input::States state;
} // namespace

/*----------------------------------------------------------------------------------*/

// Keys and Mouse buttons
void input::onButton(u16 key, u16 action) {
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
input::States const& input::getState() {
    return state;
}

bool input::isPressed(u16 key) {
    return state.pressed.find(key) != state.pressed.end();
}
bool input::isHeld(u16 key) {
    return state.held.find(key) != state.held.end();
}
bool input::isReleased(u16 key) {
    return state.released.find(key) != state.released.end();
}

/*----------------------------------------------------------------------------------*/

// Scroll
f32 xOffset;
f32 yOffset;

void input::onScroll(f32 xoffset, f32 yoffset) {
    xOffset = xoffset;
    yOffset = yoffset;
}

f32 input::getXOffset() {
    return xOffset;
}
f32 input::getYOffset() {
    return yOffset;
}

/*----------------------------------------------------------------------------------*/

// Cursor
f64 xPos;
f64 yPos;

void input::onCursorPos(f32 xpos, f32 ypos) {
    xPos = xpos;
    yPos = ypos;
}

f32 input::getXPos() {
    return xPos;
}
f32 input::getYPos() {
    return yPos;
}

/*----------------------------------------------------------------------------------*/

// End frame
void input::endFrame() {
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

/*----------------------------------------------------------------------------------*/
