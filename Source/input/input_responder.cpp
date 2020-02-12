#include "input_responder.h"

#include "../Tools/tool.h"
#include "../Context/context.h"


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

	bool w_HELD;
	w_HELD = input::isHeld(GLFW_KEY_W);

	if (w_HELD) {
		camera.moveForward();
	}

	/*-----------------------------------------*/

	bool a_HELD;
	a_HELD = input::isHeld(GLFW_KEY_A);

	if (a_HELD) {
		camera.moveLeft();
	}

	/*-----------------------------------------*/

	bool s_HELD;
	s_HELD = input::isHeld(GLFW_KEY_S);

	if (s_HELD) {
		camera.moveBackward();
	}

	/*-----------------------------------------*/

	bool d_HELD;
	d_HELD = input::isHeld(GLFW_KEY_D);

	if (d_HELD) {
		camera.moveRight();
	}

	/*-----------------------------------------*/

	bool esc_PRESSED;
	esc_PRESSED = input::isPressed(GLFW_KEY_ESCAPE);

	if (esc_PRESSED) {
		context::window::closeWindow();
	}
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

void update(float delta_time, Camera& camera) {
	bool zero_HELD;
	zero_HELD = input::isHeld(GLFW_KEY_0);

	if (zero_HELD) {
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
		std::cout << "YOU ARE ISOLATED FROM ALL OTHER SCENE STATES!" << std::endl;
	}
}

/*----------------------------------------------------------------------------------*/

bool polygon_mode_responder(bool polygon_mode_checked) {
	bool return_checked;

	bool left_shift_PRESSED;
	left_shift_PRESSED = input::isReleased(GLFW_KEY_LEFT_SHIFT);

	if (left_shift_PRESSED) {
		return_checked = !polygon_mode_checked;
		return return_checked;
	}
	return polygon_mode_checked;
}
