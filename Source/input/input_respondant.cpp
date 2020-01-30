#include "input_respondant.h"


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
	bool w_HELD;
	w_HELD = input::isHeld(GLFW_KEY_W);

	bool a_HELD;
	a_HELD = input::isHeld(GLFW_KEY_A);

	bool s_HELD;
	s_HELD = input::isHeld(GLFW_KEY_S);

	bool d_HELD;
	d_HELD = input::isHeld(GLFW_KEY_D);

	if (w_HELD) {
		camera.moveForward();
	}
	if (a_HELD) {
		camera.moveLeft();
	}
	if (s_HELD) {
		camera.moveBackward();
	}
	if (d_HELD) {
		camera.moveRight();
	}
}
void mouseResponder() {

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
		mouseResponder();
	}
	else if (isSceneState(sceneStates::STATE_HALTED)) {
		std::cout << "YOU ARE ISOLATED FROM ALL OTHER SCENE STATES!" << std::endl;
	}
}

