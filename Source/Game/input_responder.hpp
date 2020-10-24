#pragma once

#include "Core/std_types.hpp"
#include "Input/input.hpp"
#include "Camera/camera.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>


/*----------------------------------------------------------------------------------*/

enum class SceneStates;

void enableSceneState(SceneStates state);
void disableSceneState(SceneStates state);
bool isSceneState(SceneStates state);

/*----------------------------------------------------------------------------------*/

void keyboardResponder(char& direction);
void mouseResponder(Camera& camera);
void scrollResponder(Camera& camera);

void update(f32 delta_time, Camera& camera, char& direction, bool should_isolate);

/*----------------------------------------------------------------------------------*/
