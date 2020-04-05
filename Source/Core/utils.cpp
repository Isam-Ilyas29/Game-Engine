#include "Core/utils.hpp"

#include "Core/logger.hpp"
#include "Environment/environment.hpp"

#include <fmt/format.h>

#include <fstream>

#include <iostream> //temp


/*----------------------------------------------------------------------------------*/

polygonMode polygon_mode;

/*----------------------------------------------------------------------------------*/

namespace {
	Framerate f;
}

u16 getFramesPerSecond(Time dt) {
	f.elapsed += dt;

	++f.frames;

	if (f.elapsed >= Time::seconds(1.f)) {
		f.fps = f.frames;
		f.frames = 0;
		f.elapsed = {};
	}

	return f.fps;
}

/*----------------------------------------------------------------------------------*/

std::string getCurrentTime() {
	std::time_t time = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();

	std::string buffer(128, '\0');
	buffer.resize(std::strftime(buffer.data(), buffer.size(), "%Y-%m-%d %X", std::localtime(&time)));

	return buffer;
}

/*----------------------------------------------------------------------------------*/

std::vector<std::string> readFile(std::filesystem::path path) {
	std::vector<std::string> data;

	// Open the File
	std::ifstream file(path);

	// Check if object is valid
	if (!file) {
		log(logType::ERROR, fmt::format("FILE | Cannot Open {}", path.generic_string()));
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

bool startsWith(const std::string_view target, const std::string_view test) {
	return target.substr(0, test.size()) == test;
}

/*----------------------------------------------------------------------------------*/

void directoryReader(std::filesystem::path file_name, std::filesystem::path directory) {
	std::ofstream file(environment::getResourcePath("DirectoryReader/") / file_name);

	for (auto& path : std::filesystem::recursive_directory_iterator(directory)) {
		if (!std::filesystem::is_directory(path)) {
			auto base = std::filesystem::path{ "C:/Users/ilsai/Documents/OpenGL/GladApp/" };
			auto relative = std::filesystem::relative(path, base);
			file << relative.generic_string() << "\n";
		}
	}

	file.close();
}

/*----------------------------------------------------------------------------------*/
