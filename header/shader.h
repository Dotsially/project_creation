#pragma once
#include <string>
#include "depedencies.h"
#include "glad/glad.h"



class Shader{
    u32 programID;

    u32 GenShader(std::string path, u32 type);
    std::string LoadShaderFromFile(std::string filePath);
    void CheckCompileErrors(u32 id, std::string type);

public:
    Shader(std::string vertexPath, std::string fragmentPath);
    ~Shader();

    void UseProgram();

};