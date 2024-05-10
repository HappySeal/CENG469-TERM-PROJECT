//
// Created by Cafer SELLI on 7.05.2024.
//

#ifndef SAMPLEGL_CUBEMAP_H
#define SAMPLEGL_CUBEMAP_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <string>
#include "../headers/Model.h"
#include "../headers/shaderUtils.h"


class Cubemap {
public:
    GLuint ID;
    unsigned int envCubemap;
    unsigned int irradianceMap;
    unsigned int captureFBO, captureRBO;
    int width, height;
    Model *model;
    Shader *skybox;
    Shader *equirectangularToCubemap;
    Shader *irradianceShader;
    float angle;
    glm::mat4 skyboxModelMatrix;

    Cubemap(const std::string& texturePath, glm::mat4 &projection);
    void Bind();
    void Unbind();
    void Delete();
    void BindIrradiance();
    void Draw(glm::mat4 &view);
    void HandleInput(GLFWwindow *window);
};

#endif //SAMPLEGL_CUBEMAP_H
