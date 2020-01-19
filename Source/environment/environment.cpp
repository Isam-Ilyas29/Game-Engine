#include "environment.h"

namespace environment{
	std::filesystem::path exeDirectory;
	std::filesystem::path resourcesPath;
}

std::filesystem::path environment::ResourcePath(std::string_view id){
	std::filesystem::path ret = resourcesPath.string();
	ret += "/";
	ret += id;
	return ret;
}
