#pragma once

#include <iostream>
#include <functional>
#include <map>

#define GLFW_INCLUDE_NONE



extern bool mMoveForward;

namespace input {

    void setCursorPos();

    using Token = int; 
    using Callback = std::function<void(int, int)>;

    Token registerCallback(Callback c);
    bool unregisterCallback(Token token); 

    void onInput(int key, int action);
}
