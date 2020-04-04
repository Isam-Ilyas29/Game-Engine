#include "Rendering/shader.hpp"

#include "Core/logger.hpp"
#include "Rendering/graphic.hpp"

#include <fstream>
#include <sstream>


// Constructor reads and builds the shader
Shader::Shader(std::filesystem::path vertex_path, std::filesystem::path fragment_path) {
    // Retrieves the vertex/fragment source code from filePath
    std::string vertex_code;
    std::string fragment_code;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // Ensures 'ifstream' objects can throw exceptions
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        vShaderFile.open(vertex_path);
        fShaderFile.open(fragment_path);
        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertex_code = vShaderStream.str();
        fragment_code = fShaderStream.str();
    }
    catch (std::ifstream::failure e) {
        log(logType::ERROR, "[ERROR] SHADER ERROR | File Not Successfully Read");
    }

    const char* vShaderCode = vertex_code.data();
    const char* fShaderCode = fragment_code.data();

    // Compile shaders
    unsigned int vertex, fragment;
    int success;
    char info_log[512];

    // Vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    GLAD_CHECK_ERROR(glShaderSource(vertex, 1, &vShaderCode, NULL));
    GLAD_CHECK_ERROR(glCompileShader(vertex));

    // Check for vertex shader compile errors, if any
    GLAD_CHECK_ERROR(glGetShaderiv(vertex, GL_COMPILE_STATUS, &success));
    if (!success) {
        GLAD_CHECK_ERROR(glGetShaderInfoLog(vertex, 512, NULL, info_log));
        log(logType::ERROR, fmt::format("[ERROR] SHADER ERROR | Vertex Compilation Failed | {}", info_log));
    };

    // Fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    GLAD_CHECK_ERROR(glShaderSource(fragment, 1, &fShaderCode, NULL));
    GLAD_CHECK_ERROR(glCompileShader(fragment));

    // Check for shader compile errors
    GLAD_CHECK_ERROR(glGetShaderiv(fragment, GL_COMPILE_STATUS, &success));
    if (!success) {
        GLAD_CHECK_ERROR(glGetShaderInfoLog(fragment, 512, NULL, info_log));
        log(logType::ERROR, fmt::format("[ERROR] SHADER ERROR | Fragment Compilation Failed | {}", info_log));
    }

    // Shader program
    mID = glCreateProgram();
    GLAD_CHECK_ERROR(glAttachShader(mID, vertex));
    GLAD_CHECK_ERROR(glAttachShader(mID, fragment));
    GLAD_CHECK_ERROR(glLinkProgram(mID));

    // Check for linking errors
    GLAD_CHECK_ERROR(glGetProgramiv(mID, GL_LINK_STATUS, &success));
    if (!success) {
        GLAD_CHECK_ERROR(glGetProgramInfoLog(mID, 512, NULL, info_log));
        log(logType::ERROR, fmt::format("[ERROR] SHADER ERROR | Shader Program Linking Failed | {}", info_log));
    }

    // Delete shaders since they are linked to our program and are no longer necessery
    GLAD_CHECK_ERROR(glDeleteShader(vertex));
    GLAD_CHECK_ERROR(glDeleteShader(fragment));
}

Shader::~Shader() {
    GLAD_CHECK_ERROR(GLAD_CHECK_ERROR(glDeleteProgram(mID)));
}


// Activate the shader
/*------------------------------------------------------------------------*/
void Shader::use() {
    GLAD_CHECK_ERROR(glUseProgram(mID));
}


// Utility uniform functions (query a uniform location and set its value)

/*------------------------------------------------------------------------*/
void Shader::setBool(const std::string_view& name, bool value) const {
    GLAD_CHECK_ERROR(glUniform1i(glGetUniformLocation(mID, name.data()), (int)value));
}
void Shader::setInt(const std::string_view& name, u16 value) const {
    GLAD_CHECK_ERROR(glUniform1i(glGetUniformLocation(mID, name.data()), value));
}
void Shader::setFloat(const std::string_view& name, f32 value) const {
    GLAD_CHECK_ERROR(glUniform1f(glGetUniformLocation(mID, name.data()), value));
}
/*------------------------------------------------------------------------*/
void Shader::setVec2(const std::string_view& name, const glm::vec2& value) const {
    GLAD_CHECK_ERROR(glUniform2fv(glGetUniformLocation(mID, name.data()), 1, &value[0]));
}
void Shader::setVec2(const std::string_view& name, f32 x, f32 y) const {
    GLAD_CHECK_ERROR((glGetUniformLocation(mID, name.data()), x, y));
}
void Shader::setVec3(const std::string_view& name, const glm::vec3& value) const {
    GLAD_CHECK_ERROR((glGetUniformLocation(mID, name.data()), 1, &value[0]));
}
void Shader::setVec3(const std::string_view& name, f32 x, f32 y, f32 z) const {
    GLAD_CHECK_ERROR((glGetUniformLocation(mID, name.data()), x, y, z));
}
void Shader::setVec4(const std::string_view& name, const glm::vec4& value) const {
    GLAD_CHECK_ERROR(glUniform4fv(glGetUniformLocation(mID, name.data()), 1, &value[0]));
}
void Shader::setVec4(const std::string_view& name, f32 x, f32 y, f32 z, f32 w){
    GLAD_CHECK_ERROR(glUniform4f(glGetUniformLocation(mID, name.data()), x, y, z, w));
}
// ------------------------------------------------------------------------
void Shader::setMat2(const std::string_view& name, const glm::mat2& mat) const {
    GLAD_CHECK_ERROR(glUniformMatrix2fv(glGetUniformLocation(mID, name.data()), 1, GL_FALSE, &mat[0][0]));
}
void Shader::setMat3(const std::string_view& name, const glm::mat3& mat) const {
    GLAD_CHECK_ERROR(glUniformMatrix3fv(glGetUniformLocation(mID, name.data()), 1, GL_FALSE, &mat[0][0]));
}
void Shader::setMat4(const std::string_view& name, const glm::mat4& mat) const {
    GLAD_CHECK_ERROR(glUniformMatrix4fv(glGetUniformLocation(mID, name.data()), 1, GL_FALSE, &mat[0][0]));
}
/*------------------------------------------------------------------------*/

void Shader::modMatrix4fv(u16 location, u16 count, unsigned char transpose, const f32* value) {
    GLAD_CHECK_ERROR(glUniformMatrix4fv(location, count, transpose, value));
}

/*------------------------------------------------------------------------*/

// Utility function for checking shader compilation/linking errors.
void Shader::checkCompileErrors(u8 shader, std::string_view type) {
    int success;
    char info_log[1024];
    if (type != "PROGRAM") {
        GLAD_CHECK_ERROR(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));
        if (!success) {
            GLAD_CHECK_ERROR(glGetShaderInfoLog(shader, 1024, NULL, info_log));
            log(logType::ERROR, fmt::format("[ERROR] SHADER ERROR | Shader Compilation Error Of Type: {} | {}", type, info_log));
        }
    }
    else {
        GLAD_CHECK_ERROR(glGetProgramiv(shader, GL_LINK_STATUS, &success));
        if (!success) {
            GLAD_CHECK_ERROR(glGetProgramInfoLog(shader, 1024, NULL, info_log));
            log(logType::ERROR, fmt::format("[ERROR] SHADER ERROR | Program Linking Error Of Type: {} | {}", type, info_log));
        }
    }
}
