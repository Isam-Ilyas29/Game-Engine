#pragma once

#include <iostream>
#include <functional>
#include <map>
#include <unordered_set>


namespace input {

    // Keys and Mouse Buttons
    struct keyStates {
        std::unordered_set<int> pressed;
        std::unordered_set<int> held;
        std::unordered_set<int> released;
    };

    void onButton(int key, int action);
    keyStates const& getState();

    bool isPressed(int key);
    bool isHeld(int key);
    bool isReleased(int key);

    // Scroll
    void onScroll(double xoffset, double yoffset);

    double getXOffset();
    double getYOffset();

    // Cursor
    void onCursorPos(double xpos, double ypos);

    double getXPos();
    double getYPos();

    // End frame
    void endFrame();
}



