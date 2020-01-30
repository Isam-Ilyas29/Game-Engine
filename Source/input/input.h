#pragma once

#include <iostream>
#include <functional>
#include <map>
#include <unordered_set>


namespace input {

    struct keyStates {
        std::unordered_set<int> pressed;
        std::unordered_set<int> held;
        std::unordered_set<int> released;
    };

    void endFrame();
    void onInput(int key, int action);
    keyStates const& getState();

    bool isPressed(int key);
    bool isHeld(int key);
    bool isReleased(int key);
}



