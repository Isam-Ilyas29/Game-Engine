#pragma once

#include <iostream>
#include <functional>
#include <map>

#include <GLFW/glfw3.h>



namespace input
{
    using Token = int;  // later shared_ptr<int>
    using Callback = std::function<void(int, int)>;

    Token registerCallback(Callback c);
    bool unregisterCallback(Token token);  // not required with shared_ptr

    void foo();
}
