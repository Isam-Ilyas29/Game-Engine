#include "game/gameloop.h"
#include <iostream>

int main(int argc, char* argv[]) {

	bool success = gameloop::run(argc, argv);

	if (!success) {
		return 1;
	}
	return 0;
}

