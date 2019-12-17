#pragma once
#include <string>

namespace environment {
    extern std::string exeDirectory;
    extern std::string resourcesPath;
    std::string ResourcePath(std::string_view id);
}