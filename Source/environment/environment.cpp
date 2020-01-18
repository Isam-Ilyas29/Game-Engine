#include "environment.h"

namespace environment{
	std::filesystem::path exeDirectory;
	std::filesystem::path resourcesPath;
}

std::string environment::ResourcePath(std::string_view id){
	std::string ret = resourcesPath.generic_string();
	ret += "/";
	ret += id;
	return ret;
}
