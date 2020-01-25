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

/*
// input.cpp
namespace
{
Input::State g_state;
}

void Input::startFrame()
{
g_state.released.clear();
}

void Input::onInput(int key, int action)
{
switch (action)
{
default:
{
break;
}
case GLFW_PRESS:
{
g_released.erase(key);
if (isPressed(key))
{
g_state.pressed.erase(key);
g_state.held.insert(key);
}
else
{
g_state.pressed.insert(key);
}
break;
}
case GLFW_RELEASE:
{
g_state.held.erase(key);
g_state.pressed.erase(key);
g_state.released.insert(key);
break;
}
}
}

Input::State const& Input::getState()
{
return g_state;
}

bool Input::isPressed(int key)
{
return g_state.pressed.find(key) != g_state.pressed.end();
}

bool Input::isHeld(int key)
{
return g_state.held.find(key) != g_state.held.end();
}

bool Input::isReleased(int key)
{
return g_state.released.find(key) != g_state.released.end();
}
*/

