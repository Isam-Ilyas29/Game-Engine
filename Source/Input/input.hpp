#pragma once

#include "Utils/std_types.hpp"

#include <iostream>
#include <functional>
#include <map>
#include <unordered_set>


namespace input {

    // Keys and Mouse Buttons
    struct keyStates {
        std::unordered_set<u16> pressed;
        std::unordered_set<u16> held;
        std::unordered_set<u16> released;
    };

    void onButton(u16 key, u16 action);
    keyStates const& getState();

    bool isPressed(u16 key);
    bool isHeld(u16 key);
    bool isReleased(u16 key);

    // Scroll
    void onScroll(f64 xoffset, f64 yoffset);

    f64 getXOffset();
    f64 getYOffset();

    // Cursor
    void onCursorPos(f64 xpos, f64 ypos);

    f64 getXPos();
    f64 getYPos();

    // End frame
    void endFrame();
}
