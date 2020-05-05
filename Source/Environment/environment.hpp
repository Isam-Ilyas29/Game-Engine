#pragma once

#include <filesystem>


namespace environment {
    extern std::filesystem::path exePath;
    extern std::filesystem::path resourcePath;

    std::filesystem::path getResourcePath(std::filesystem::path id = "");

} // namespace environment
