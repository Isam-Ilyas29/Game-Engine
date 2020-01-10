#include "keyboard.h"


void Input::processInput(GLFWwindow* window, int key, int scancode, int action, int mods) {
	Input::onInput(key, action);
}

namespace {
	Input::StateStruct gState;
}

void Input::startFrame() {
	gState.released.clear();
}

void Input::onInput(int key, int action) {
	switch (action) {

	case GLFW_PRESS: {
		gState.released.erase(key);
		std::cout << gState.pressed.size() << std::endl;
		if (isPressed(key)) {
			gState.pressed.erase(key);
			gState.held.insert(key);
		}
		else {
			gState.pressed.insert(key);
		}
		break;
	}

	case GLFW_RELEASE: {
		gState.held.erase(key);
		//gState.pressed.erase(key);
		gState.released.insert(key);
		break;
	}

	default: {
		break;
	}
	}
}

Input::StateStruct const& Input::getState() {
	return gState;
}

bool Input::isPressed(int key) {
	return gState.pressed.find(key) != gState.pressed.end();
}

bool Input::isHeld(int key) {
	return gState.held.find(key) != gState.held.end();
}

bool Input::isReleased(int key) {
	return gState.released.find(key) != gState.released.end();
}


