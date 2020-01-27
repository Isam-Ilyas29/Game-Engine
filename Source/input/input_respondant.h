#pragma once

#include <iostream>
#include <bitset>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "input.h"
#include "../Camera/camera.h"



enum class sceneStates;

void enableSceneState(sceneStates state);
void disableSceneState(sceneStates state);
bool isSceneState(sceneStates state);

/*----------------------------------------------------------------------------------*/

enum class inputStates;

void enableInputState(inputStates state);
void disableInputState(inputStates state);
bool isInputState(inputStates state);

void keyboardResponder();
void mouseResponder();
void update(float delta_time);



