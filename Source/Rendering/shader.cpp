#include "shader.h"

#include <glad/glad.h>


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
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }

    const char* vShaderCode = vertex_code.data();
    const char* fShaderCode = fragment_code.data();

    // Compile shaders
    unsigned int vertex, fragment;
    int success;
    char info_log[512];

    // Vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);

    // Check for vertex shader compile errors, if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, info_log);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << std::endl;
    };

    // Fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);

    // Check for shader compile errors
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, info_log);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << info_log << std::endl;
    }

    // Shader program
    mID = glCreateProgram();
    glAttachShader(mID, vertex);
    glAttachShader(mID, fragment);
    glLinkProgram(mID);

    // Check for linking errors
    glGetProgramiv(mID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(mID, 512, NULL, info_log);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << std::endl;
    }

    // Delete shaders since they are linked to our program and are no longer necessery
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader() {
    glDeleteProgram(mID);
}


// Activate the shader
/*------------------------------------------------------------------------*/
void Shader::use() {
    glUseProgram(mID);
}


// Utility uniform functions (query a uniform location and set its value)

/*------------------------------------------------------------------------*/
void Shader::setBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(mID, name.data()), (int)value);
}
void Shader::setInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(mID, name.data()), value);
}
void Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(mID, name.data()), value);
}
/*------------------------------------------------------------------------*/
void Shader::setVec2(const std::string& name, const glm::vec2& value) const {
    glUniform2fv(glGetUniformLocation(mID, name.data()), 1, &value[0]);
}
void Shader::setVec2(const std::string& name, float x, float y) const {
    glUniform2f(glGetUniformLocation(mID, name.data()), x, y);
}
void Shader::setVec3(const std::string& name, const glm::vec3& value) const {
    glUniform3fv(glGetUniformLocation(mID, name.data()), 1, &value[0]);
}
void Shader::setVec3(const std::string& name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(mID, name.data()), x, y, z);
}
void Shader::setVec4(const std::string& name, const glm::vec4& value) const {
    glUniform4fv(glGetUniformLocation(mID, name.data()), 1, &value[0]);
}
void Shader::setVec4(const std::string& name, float x, float y, float z, float w){
    glUniform4f(glGetUniformLocation(mID, name.data()), x, y, z, w);
}
// ------------------------------------------------------------------------
void Shader::setMat2(const std::string& name, const glm::mat2& mat) const {
    glUniformMatrix2fv(glGetUniformLocation(mID, name.data()), 1, GL_FALSE, &mat[0][0]);
}
void Shader::setMat3(const std::string& name, const glm::mat3& mat) const {
    glUniformMatrix3fv(glGetUniformLocation(mID, name.data()), 1, GL_FALSE, &mat[0][0]);
}
void Shader::setMat4(const std::string& name, const glm::mat4& mat) const {
    glUniformMatrix4fv(glGetUniformLocation(mID, name.data()), 1, GL_FALSE, &mat[0][0]);
}
/*------------------------------------------------------------------------*/

void Shader::modMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value) {
    glUniformMatrix4fv(location, count, transpose, value);
}

/*------------------------------------------------------------------------*/

// Utility function for checking shader compilation/linking errors.
void Shader::checkCompileErrors(unsigned int shader, std::string type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}
