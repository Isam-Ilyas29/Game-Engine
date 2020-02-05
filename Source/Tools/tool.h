#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <fstream>


extern float delta_time;
extern float last_frame;

extern bool polygon_mode;


float framesPerSecond();

std::vector<std::string> readFile(std::string path);

