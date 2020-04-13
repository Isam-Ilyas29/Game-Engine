#include "Environment/environment.hpp"


std::filesystem::path environment::exePath;
std::filesystem::path environment::resourcePath;

std::filesystem::path environment::getResourcePath(std::filesystem::path id) {
	if (id == "") {
		return resourcePath;
	}
	return resourcePath / id;
}
