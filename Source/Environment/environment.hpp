#pragma once

#include <string>
#include <filesystem>


namespace environment {
    extern std::filesystem::path exeDirectory;
    extern std::filesystem::path resourcesPath;
    std::filesystem::path ResourcePath(std::filesystem::path id);
}
