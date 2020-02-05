#include "tool.h"


float delta_time = 0.0f;
float last_frame = 0.0f;

bool polygon_mode = false;


float framesPerSecond() {
	float frame_rate = 1.0 / delta_time;
	return frame_rate;
}


std::vector<std::string> readFile(std::string path) {
	std::vector<std::string> vector;

	// Open the File
	std::ifstream file(path);

	// Check if object is valid
	if (!file) {
		std::cerr << "Cannot open the File" << std::endl;
		return vector;
	}

	std::string content;
	while (std::getline(file, content)) {
		if (content.size() > 0)
			vector.push_back(content);
	}

	file.close();

	return vector;
}

