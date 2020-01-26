#pragma once

#include <iostream>
#include <functional>
#include <map>
#include <unordered_set>


namespace input {

    void setCursorPos();


    struct State {
        std::unordered_set<int> pressed;
        std::unordered_set<int> held;
        std::unordered_set<int> released;
    };

    void startFrame();
    void onInput(int key, int action);
    State const& getState();

    bool isPressed(int key);
    bool isHeld(int key);
    bool isReleased(int key);
}



