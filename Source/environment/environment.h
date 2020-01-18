#pragma once

#include <string>
#include <filesystem>

namespace environment {
    extern std::filesystem::path exeDirectory;
    extern std::filesystem::path resourcesPath;
    std::string ResourcePath(std::string_view id);
}