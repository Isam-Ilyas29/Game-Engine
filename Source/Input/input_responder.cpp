#include "Input/input_responder.hpp"

#include "Context/context.hpp"
#include "Core/utils.hpp"

#include <imgui.h>

#include <bitset>


/*----------------------------------------------------------------------------------*/

// Scene states and has the correspoding functions and bitset

enum class sceneStates {
	STATE_HALTED,
	STATE_CAMERA,

	_COUNT
};

std::bitset<size_t(sceneStates::_COUNT)> scene_state;

void enableSceneState(sceneStates state) {
	scene_state[size_t(state)] = true;
}
void disableSceneState(sceneStates state) {
	scene_state[size_t(state)] = false;
}
bool isSceneState(sceneStates state) {
	bool state_to_check = scene_state[size_t(state)];
	return state_to_check;
}

/*----------------------------------------------------------------------------------*/

void keyboardResponder(Camera& camera) {

	/*-----------------------------------------*/

	if (input::isHeld(GLFW_KEY_W)) {
		camera.moveForward();
	}

	/*-----------------------------------------*/

	if (input::isHeld(GLFW_KEY_A)) {
		camera.moveLeft();
	}

	/*-----------------------------------------*/

	if (input::isHeld(GLFW_KEY_S)) {
		camera.moveBackward();
	}

	/*-----------------------------------------*/

	if (input::isHeld(GLFW_KEY_D)) {
		camera.moveRight();
	}

	/*-----------------------------------------*/

	if (input::isHeld(GLFW_KEY_LEFT_SHIFT)) {
		polygon_mode = polygonMode::LINE;
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

void update(f32 delta_time, Camera& camera, bool should_isolate) {

	if (should_isolate) {
		disableSceneState(sceneStates::STATE_CAMERA);
		enableSceneState(sceneStates::STATE_HALTED);
	}
	else {
		disableSceneState(sceneStates::STATE_HALTED);
		enableSceneState(sceneStates::STATE_CAMERA);
	}

	/*---------------------------------------------*/

	if (isSceneState(sceneStates::STATE_CAMERA)) {
		keyboardResponder(camera);
		mouseResponder(camera);
		scrollResponder(camera);
	}
	else if (isSceneState(sceneStates::STATE_HALTED)) {
		//std::cout << "YOU ARE ISOLATED FROM ALL OTHER SCENE STATES!" << std::endl;
	}
}

/*----------------------------------------------------------------------------------*/
