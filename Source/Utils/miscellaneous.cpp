#include "miscellaneous.h"



bool polygon_mode = false;


double getFramesPerSecond(double delta_time) {
	float frame_rate = 1.0 / delta_time;
	return frame_rate;
}


std::vector<std::string> readFile(std::filesystem::path path) {
	std::vector<std::string> vector;

	// Open the File
	std::ifstream file(path);

	// Check if object is valid
	if (!file) {
		std::cerr << "Cannot open " << path << std::endl;
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

