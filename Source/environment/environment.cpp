#include "environment.h"

namespace environment{
	std::string exeDirectory;
	std::string resourcesPath;
}

std::string environment::ResourcePath(std::string_view id){
	std::string ret = resourcesPath;
	ret += "/";
	ret += id;
	return ret;
}
