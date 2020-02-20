#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>


extern bool polygon_mode;


float framesPerSecond(double delta_time);


std::vector<std::string> readFile(std::filesystem::path path);
 