//
// Created by Cafer SELLI on 11.04.2024.
//

#ifndef SAMPLEGL_VAO_H
#define SAMPLEGL_VAO_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "VBO.h"

class VAO{
public:
    GLuint ID;
    VAO();

    void LinkVBO(VBO& VBO, GLuint);
    void Bind();
    void Unbind();
    void Delete();
};

#endif //SAMPLEGL_VAO_H
