#include "Environment/environment.hpp"


namespace environment{
	std::filesystem::path exePath;
	std::filesystem::path resourcePath;

	std::filesystem::path environment::getResourcePath(std::filesystem::path id) {
		if (id == "") {
			return resourcePath;
		}
		return resourcePath / id;
	}
}
