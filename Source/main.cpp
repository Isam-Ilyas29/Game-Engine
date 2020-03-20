#include "Game/gameloop.hpp"

#include "Core/profiler.hpp"

#include <iostream>


int main(int argc, char* argv[]) {

	PROFILE_BEGIN_SESSION("Entire Profile Session", "../ExeDirectory/profile.json");
	bool success = gameloop::run(argc, argv);
	PROFILE_END_SESSION();

	if (!success) {
		return 1;
	}
	return 0;
}
