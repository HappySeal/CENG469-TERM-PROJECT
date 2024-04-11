//
// Created by Cafer SELLI on 11.04.2024.
//

#ifndef SAMPLEGL_SHADERUTILS_H
#define SAMPLEGL_SHADERUTILS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <iostream>
#include <fstream>
#include <sstream>

std::string get_file_content(const std::string& filePath);

class Shader{
public:
    GLuint ID;
    Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

    void Activate();
    void Delete();
};


#endif //SAMPLEGL_SHADERUTILS_H
