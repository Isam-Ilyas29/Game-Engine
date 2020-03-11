#include "Core/utils.hpp"


/*----------------------------------------------------------------------------------*/

bool wireframe_mode = false;

/*----------------------------------------------------------------------------------*/

namespace {
	framerate f;
}

u16 getFramesPerSecond(Time dt) {
	f.elapsed += dt;

	++f.frames;

	if (f.elapsed >= Time::setSeconds(1.0f)) {
		f.fps = f.frames;
		f.frames = 0;
		f.elapsed = {};
	}

	return f.fps;
}

/*----------------------------------------------------------------------------------*/

std::vector<std::string> readFile(std::filesystem::path path) {
	std::vector<std::string> data;

	// Open the File
	std::ifstream file(path);

	// Check if object is valid
	if (!file) {
		std::cerr << "Cannot open " << path << std::endl;
		return data;
	}

	std::string content;
	while (std::getline(file, content)) {
		if (content.size() > 0)
			data.push_back(content);
	}

	file.close();

	return data;
}

/*----------------------------------------------------------------------------------*/
