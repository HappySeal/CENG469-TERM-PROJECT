//
// Created by Cafer SELLI on 2.05.2024.
//

#ifndef SAMPLEGL_CAMERA_H
#define SAMPLEGL_CAMERA_H

#define GLM_ENABLE_EXPERIMENTAL 1

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/vector_angle.hpp>
#include "shaderUtils.h"


class Camera {
public:
    glm::vec3 Position;
    glm::vec3 Orientation;
    glm::vec3 Up;
    glm::mat4 cameraMatrix;

    bool firstClick = true;

   int width;
   int height;

   float speed = 0.1f;
   float sensitivity = 100.0f;

    Camera(int _width, int _height, glm::vec3 pos = glm::vec3(0,0,0));
    void updateMatrix(float FOVdeg, float nearPlane, float farPlane);
    void Matrix(Shader& shader, const char* uniform);
    void Inputs(GLFWwindow* window);
};

#endif //SAMPLEGL_CAMERA_H
