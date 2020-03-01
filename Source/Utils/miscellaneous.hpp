#pragma once

#include "std_types.hpp"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>


extern bool wireframe_mode;


f64 getFramesPerSecond(f64 delta_time);


std::vector<std::string> readFile(std::filesystem::path path);
 