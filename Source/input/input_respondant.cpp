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

// Input states and has the correspoding functions and bitset.
// Also holds all input functions.

enum class inputStates {
	STATE_MOVE_FORWARD,
	STATE_MOVE_LEFT,
	STATE_MOVE_BACKWARD,
	STATE_MOVE_RIGHT,

	_COUNT
};

std::bitset<size_t(inputStates::_COUNT)> input_state;

void enableInputState(inputStates state) {
	input_state[size_t(state)] = true;
}
void disableInputState(inputStates state) {
	input_state[size_t(state)] = false;
}
bool isInputState(inputStates state) {
	bool state_to_check = input_state[size_t(state)];
	return state_to_check;
}


void keyboardResponder(){
	bool w_HELD;
	w_HELD = input::isHeld(GLFW_KEY_W);

	bool a_HELD;
	a_HELD = input::isHeld(GLFW_KEY_A);

	bool s_HELD;
	s_HELD = input::isHeld(GLFW_KEY_S);

	bool d_HELD;
	d_HELD = input::isHeld(GLFW_KEY_D);

	if (w_HELD) {
		disableInputState(inputStates::STATE_MOVE_LEFT);
		disableInputState(inputStates::STATE_MOVE_BACKWARD);
		disableInputState(inputStates::STATE_MOVE_RIGHT);

		enableInputState(inputStates::STATE_MOVE_FORWARD);
	}
	else if (a_HELD) {
		disableInputState(inputStates::STATE_MOVE_FORWARD);
		disableInputState(inputStates::STATE_MOVE_BACKWARD);
		disableInputState(inputStates::STATE_MOVE_RIGHT);

		enableInputState(inputStates::STATE_MOVE_LEFT);
	}
	else  if (s_HELD) {
		disableInputState(inputStates::STATE_MOVE_FORWARD);
		disableInputState(inputStates::STATE_MOVE_LEFT);
		disableInputState(inputStates::STATE_MOVE_RIGHT);

		enableInputState(inputStates::STATE_MOVE_BACKWARD);
	}
	else if (d_HELD) {
		disableInputState(inputStates::STATE_MOVE_FORWARD);
		disableInputState(inputStates::STATE_MOVE_LEFT);
		disableInputState(inputStates::STATE_MOVE_BACKWARD);

		enableInputState(inputStates::STATE_MOVE_RIGHT);
	}
	else {
		disableInputState(inputStates::STATE_MOVE_FORWARD);
		disableInputState(inputStates::STATE_MOVE_LEFT);
		disableInputState(inputStates::STATE_MOVE_BACKWARD);
		disableInputState(inputStates::STATE_MOVE_RIGHT);
	}

	/*---------------------------------------------------------*/

	if (isInputState(inputStates::STATE_MOVE_FORWARD)) {
		std::cout << "MOVE FORWARD FUNCTION" << std::endl;
	}
	else if (isInputState(inputStates::STATE_MOVE_LEFT)) {
		std::cout << "MOVE LEFT FUNCTION" << std::endl;
	}
	else if (isInputState(inputStates::STATE_MOVE_BACKWARD)) {
		std::cout << "MOVE BACKWARD FUNCTION" << std::endl;
	}
	else if (isInputState(inputStates::STATE_MOVE_RIGHT)) {
		std::cout << "MOVE RIGHT FUNCTION" << std::endl;
	}
}
void mouseResponder() {

}

void update(float delta_time) {
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
		keyboardResponder();
		mouseResponder();
	}
	else if (isSceneState(sceneStates::STATE_HALTED)) {
		std::cout << "YOU ARE ISOLATED FROM ALL OTHER SCENE STATES!" << std::endl;
	}
}


