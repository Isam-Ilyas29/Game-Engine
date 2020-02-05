#pragma once

#include <iostream>
#include <bitset>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "input.h"
#include "../Camera/camera.h"
#include "../Rendering/ImGUI/utils.h"



enum class sceneStates;

void enableSceneState(sceneStates state);
void disableSceneState(sceneStates state);
bool isSceneState(sceneStates state);

/*----------------------------------------------------------------------------------*/

void keyboardResponder(Camera& camera, CreateCheckbox& polygon_mode_checkbox);
void mouseResponder();

void update(float delta_time, Camera& camera, CreateCheckbox& polygon_mode_checkbox);

