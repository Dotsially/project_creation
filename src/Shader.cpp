#include "shader.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(std::string vertexPath, std::string fragmentPath)
{
    int vertexShader = GenShader("resources/shaders/" + vertexPath, GL_VERTEX_SHADER);
    int fragmentShader = GenShader("resources/shaders/" + fragmentPath, GL_FRAGMENT_SHADER);

    this->programID = glCreateProgram();

    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);
    glLinkProgram(programID);
    CheckCompileErrors(programID, "PROGRAM");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader(){
    glDeleteProgram(this->programID);
}

void Shader::UseProgram()
{
    glUseProgram(this->programID);
}

u32 Shader::GenShader(std::string path, u32 type){
    u32 shader = glCreateShader(type);
    std::string shaderCode = LoadShaderFromFile(path);

    const char* shaderSource = shaderCode.c_str();
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);
    CheckCompileErrors(shader, "SHADER");

    return shader;
}

std::string Shader::LoadShaderFromFile(std::string filePath){
    std::string shaderCode;
    std::ifstream shaderFile;
    
    // ensure ifstream objects can throw exceptions:
    shaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try 
    {
        // open files
        shaderFile.open(filePath);
        std::stringstream shaderStream;
        // read file's buffer contents into streams
        shaderStream << shaderFile.rdbuf();
        // close file handlers
        shaderFile.close();
        // convert stream into string
        shaderCode = shaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }   
    return shaderCode;
}

void Shader::CheckCompileErrors(u32 id, std::string type){
    i32 success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(id, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(id, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}




