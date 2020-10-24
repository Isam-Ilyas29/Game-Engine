#include "Game/input_responder.hpp"

#include "Context/context.hpp"
#include "Core/utils.hpp"

#include <imgui.h>

#include <bitset>


/*----------------------------------------------------------------------------------*/

// Scene states and has the correspoding functions and bitset

enum class SceneStates {
	STATE_HALTED,
	STATE_CAMERA,

	_COUNT
};

std::bitset<size_t(SceneStates::_COUNT)> scene_state;

void enableSceneState(SceneStates state) {
	scene_state[size_t(state)] = true;
}
void disableSceneState(SceneStates state) {
	scene_state[size_t(state)] = false;
}
bool isSceneState(SceneStates state) {
	bool state_to_check = scene_state[size_t(state)];
	return state_to_check;
}

/*----------------------------------------------------------------------------------*/

void keyboardResponder(char& direction) {

	/*-----------------------------------------*/

	if (input::isHeld(GLFW_KEY_W)) {
		direction = 'u';
	}

	/*-----------------------------------------*/

	if (input::isHeld(GLFW_KEY_A)) {
		direction = 'l';
	}

	/*-----------------------------------------*/

	if (input::isHeld(GLFW_KEY_S)) {
		direction = 'd';
	}

	/*-----------------------------------------*/

	if (input::isHeld(GLFW_KEY_D)) {
		direction = 'r';
	}

	/*-----------------------------------------*/

	if (input::isPressed(GLFW_KEY_ESCAPE)) {
		context::window::closeWindow();
	}

	/*-----------------------------------------*/
}

void mouseResponder(Camera& camera) {

	if (input::isPressed(GLFW_MOUSE_BUTTON_2)) {
		camera.beginCursorRotation();
	}
	if (input::isHeld(GLFW_MOUSE_BUTTON_2)) {
		camera.cursorRotation(input::getXPos(), input::getYPos());
	}
}

void scrollResponder(Camera& camera) {
	camera.zoom(input::getXOffset(), input::getYOffset());
}

/*----------------------------------------------------------------------------------*/

void update(f32 delta_time, Camera& camera, char& direction, bool should_isolate) {

	if (should_isolate) {
		disableSceneState(SceneStates::STATE_CAMERA);
		enableSceneState(SceneStates::STATE_HALTED);
	}
	else {
		disableSceneState(SceneStates::STATE_HALTED);
		enableSceneState(SceneStates::STATE_CAMERA);
	}

	/*---------------------------------------------*/

	if (isSceneState(SceneStates::STATE_CAMERA)) {
		keyboardResponder(direction);
		mouseResponder(camera);
		scrollResponder(camera);
	}
	else if (isSceneState(SceneStates::STATE_HALTED)) {
		//std::cout << "YOU ARE ISOLATED FROM ALL OTHER SCENE STATES!" << std::endl;
	}
}

/*----------------------------------------------------------------------------------*/
