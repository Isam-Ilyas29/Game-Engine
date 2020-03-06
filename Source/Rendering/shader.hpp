#pragma once

#include "Utils/std_types.hpp"

#include <glm/glm.hpp>
#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>


class Shader {
public:
    // Program ID
    unsigned int mID;

    Shader(std::filesystem::path vertex_path, std::filesystem::path fragment_path);
    ~Shader();

    void use();

    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, u16 value) const;
    void setFloat(const std::string& name, f32 value) const;

    void setVec2(const std::string& name, const glm::vec2& value) const;
    void setVec2(const std::string& name, f32 x, f32 y) const;
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setVec3(const std::string& name, f32 x, f32 y, f32 z) const;
    void setVec4(const std::string& name, const glm::vec4& value) const;
    void setVec4(const std::string& name, f32 x, f32 y, f32 z, f32 w);

    void setMat2(const std::string& name, const glm::mat2& mat) const;
    void setMat3(const std::string& name, const glm::mat3& mat) const;
    void setMat4(const std::string& name, const glm::mat4& mat) const;

    void modMatrix4fv(u16 location, u16 count, unsigned char transpose, const f32* value);

private:
    void checkCompileErrors(u8 shader, std::string type);
};
