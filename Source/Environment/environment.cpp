#include "Environment/environment.hpp"


namespace environment{
	std::filesystem::path exeDirectory;
	std::filesystem::path resourcesPath;
}

std::filesystem::path environment::ResourcePath(std::filesystem::path id){
	return resourcesPath / id;
}
