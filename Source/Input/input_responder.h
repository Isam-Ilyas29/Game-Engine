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

void keyboardResponder(Camera& camera);
void mouseResponder(Camera& camera);
void scrollResponder(Camera& camera);

void update(float delta_time, Camera& camera, bool should_isolate);


