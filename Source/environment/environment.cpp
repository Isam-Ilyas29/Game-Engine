#include "environment.h"

namespace environment{
	std::filesystem::path exeDirectory;
	std::filesystem::path resourcesPath;
}

std::filesystem::path environment::ResourcePath(std::filesystem::path id){
	std::filesystem::path ret = resourcesPath.string();
	return resourcesPath / id;
}
