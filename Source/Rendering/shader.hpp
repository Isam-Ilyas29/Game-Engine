#pragma once

#include "Core/std_types.hpp"

#include <glad/glad.h>
#include <fmt/format.h>
#include <glm/glm.hpp>

#include <filesystem>
#include <string>


class Shader {
private:
    std::filesystem::path mVertexPath;
    std::filesystem::path mFragmentPath;

public:
    // Program ID
    unsigned int mID;

    Shader(std::filesystem::path vertex_path, std::filesystem::path fragment_path);
    ~Shader();

    void use();

    void setBool(std::string_view name, bool value) const;
    void setInt(std::string_view name, s32 value) const;
    void setUint(std::string_view name, u32 value) const;
    void setFloat(std::string_view name, f32 value) const;
    void setDouble(std::string_view name, f64 value) const;

    void setVec2(std::string_view name, const glm::vec2& value) const;
    void setVec2(std::string_view name, f32 x, f32 y) const;
    void setVec3(std::string_view name, const glm::vec3& value) const;
    void setVec3(std::string_view name, f32 x, f32 y, f32 z) const;
    void setVec4(std::string_view name, const glm::vec4& value) const;
    void setVec4(std::string_view name, f32 x, f32 y, f32 z, f32 w);

    void setMat2(std::string_view name, const glm::mat2& mat) const;
    void setMat3(std::string_view name, const glm::mat3& mat) const;
    void setMat4(std::string_view name, const glm::mat4& mat) const;

    void modMatrix4fv(u16 location, u16 count, unsigned char transpose, const f32* value);

private:
    void checkCompileErrors(u8 shader, std::string_view type);
};
