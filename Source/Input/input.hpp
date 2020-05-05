#pragma once

#include "Core/std_types.hpp"

#include <unordered_set>


namespace input {

    /*----------------------------------------------------------------------------------*/

    // Keys and Mouse Buttons
    struct states {
        std::unordered_set<u16> pressed;
        std::unordered_set<u16> held;
        std::unordered_set<u16> released;
    };

    /*----------------------------------------------------------------------------------*/

    void onButton(u16 key, u16 action);
    states const& getState();

    bool isPressed(u16 key);
    bool isHeld(u16 key);
    bool isReleased(u16 key);

    /*----------------------------------------------------------------------------------*/

    // Scroll
    void onScroll(f32 xoffset, f32 yoffset);

    f32 getXOffset();
    f32 getYOffset();

    /*----------------------------------------------------------------------------------*/

    // Cursor
    void onCursorPos(f32 xpos, f32 ypos);

    f32 getXPos();
    f32 getYPos();

    // End frame
    void endFrame();

    /*----------------------------------------------------------------------------------*/

} // namespace input
