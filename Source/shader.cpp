#include "shader.h"

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

//Constructor reads and builds the shader
Shader::Shader(const char* vertex_path, const char* fragment_path) {
    //Retrieves the vertex/fragment source code from filePath
    std::string vertex_code;
    std::string fragment_code;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    //Ensures 'ifstream' objects can throw exceptions
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        //Opens files
        vShaderFile.open(vertex_path);
        fShaderFile.open(fragment_path);
        std::stringstream vShaderStream, fShaderStream;

        //Reads file's buffer contents into stream
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        //Closes file handlers
        vShaderFile.close();
        fShaderFile.close();

        //Converts stream into string
        vertex_code = vShaderStream.str();
        fragment_code = fShaderStream.str();
    }
    //If it fails then display error (Except failure)
    catch (std::ifstream::failure e) {
        //Shader filePath error message
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }

    //Points to the vertex/fragment code
    const char* vShaderCode = vertex_code.c_str();
    const char* fShaderCode = fragment_code.c_str();

    //Compile shaders
    unsigned int vertex, fragment;
    int success;
    char info_log[512];

    //Vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);

    //Check for vertex shader compile errors, if any
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, info_log);
        //Vertex shader error message
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << std::endl;
    };

    //Fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);

    //Check for shader compile errors
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, info_log);
        //Fragment shader error message
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << info_log << std::endl;
    }

    //Shader program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    //Check for linking errors
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 512, NULL, info_log);
        //Shader program error message
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << std::endl;
    }

    //Delete shaders since they are linked to our program and are no longer necessery
    glDeleteShader(vertex);
    glDeleteShader(fragment);

}

//Activate the shader
/*------------------------------------------------------------------------*/
void Shader::use() {
    glUseProgram(ID);
}
//Utility uniform functions (query a uniform location and set its value)
/*------------------------------------------------------------------------*/
void Shader::set_bool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
/*------------------------------------------------------------------------*/
void Shader::set_int(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
/*------------------------------------------------------------------------*/
void Shader::set_float(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
/*------------------------------------------------------------------------*/

//Utility function for checking shader compilation/linking errors.
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
