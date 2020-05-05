#pragma once

#include "Core/std_types.hpp"
#include "Core/time.hpp"

#include <string>
#include <vector>
#include <filesystem>


/*----------------------------------------------------------------------------------*/

// Switch between polygon mode's
enum class polygonMode {
	FILL,
	LINE,
	POINT
};

extern polygonMode polygon_mode;

/*----------------------------------------------------------------------------------*/

// Accurate FPS reader
struct Framerate {
	Time elapsed;
	u16 frames = 0;
	u16 fps = 0;
};

u16 getFramesPerSecond(Time dt);

/*----------------------------------------------------------------------------------*/

// Get current timestamp
std::string getCurrentTime();

/*----------------------------------------------------------------------------------*/

// Reads file and returns vector filled with content
std::vector<std::string> readFile(std::filesystem::path path);
 
/*----------------------------------------------------------------------------------*/

// Checks if a given string begins with the given prefix/test
bool startsWith(const std::string_view target, const std::string_view test);

/*----------------------------------------------------------------------------------*/

// Finds all file paths for a given directory and appends to txt file
void directoryReader(std::filesystem::path file_name, std::filesystem::path directory);

/*----------------------------------------------------------------------------------*/
