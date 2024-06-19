//
// Created by Cafer SELLI on 2.05.2024.
//

#include "../headers/Camera.h"

Camera::Camera(int _width, int _height, glm::vec3 pos, float _FOVdeg, float _nearPlane, float _farPlane){
    Position = pos;
    Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
    Up = glm::vec3(0.0f, 1.0f, 0.0f);
    cameraMatrix = glm::mat4(1.0f);
    width = _width;
    height = _height;

    float planeHeight = _nearPlane * tan(glm::radians(_FOVdeg) / 2.0f) * 2.0f;
    float planeWidth = planeHeight * (float)width / (float)height;
    viewParams = glm::vec3(planeHeight, planeWidth, _nearPlane);

    projectionMatrix = glm::perspective(glm::radians(_FOVdeg), (float)width / (float)height, _nearPlane, _farPlane);
}

void Camera::updateMatrix(){
    //viewMatrix = glm::lookAt(Position, Position + Orientation, Up);
    //cameraMatrix = projectionMatrix * viewMatrix;

    glm::vec3 direction = Orientation;
    direction = glm::normalize(direction);

    // Define the default up vector
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    // Calculate the right vector
    glm::vec3 right = glm::normalize(glm::cross(up, direction));

    // Recompute the up vector to ensure orthogonality
    up = glm::cross(direction, right);

    // Create and populate the rotation matrix
    glm::mat4 rotationMatrix(1.0f); // Initialize to identity matrix
    rotationMatrix[0] = glm::vec4(right, 0.0f);    // Set the right vector
    rotationMatrix[1] = glm::vec4(up, 0.0f);       // Set the up vector
    rotationMatrix[2] = glm::vec4(direction, 0.0f);// Set the direction vector

    viewMatrix = rotationMatrix;
    //cameraMatrix = projectionMatrix * viewMatrix;

}

void Camera::Matrix(Shader &shader, const char *uniform) {
    shader.Activate();
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}



void Camera::HandleControl(GLFWwindow *window) {
    if(glfwGetKey(window, FORWARD_KEY[this->keyBinding]) == GLFW_PRESS){
        Position += speed * Orientation;
    }
    if(glfwGetKey(window, BACKWARD_KEY[this->keyBinding]) == GLFW_PRESS){
        Position -= speed * Orientation;
    }
    if(glfwGetKey(window, LEFT_KEY[this->keyBinding]) == GLFW_PRESS){
        Position -= glm::normalize(glm::cross(Orientation, Up)) * speed;
    }
    if(glfwGetKey(window, RIGHT_KEY[this->keyBinding]) == GLFW_PRESS){
        Position += glm::normalize(glm::cross(Orientation, Up)) * speed;
    }
    if(glfwGetKey(window, UP_KEY[this->keyBinding]) == GLFW_PRESS){
        Position += Up * speed;
    }
    if(glfwGetKey(window, DOWN_KEY[this->keyBinding]) == GLFW_PRESS){
        Position -= Up * speed;
    }
    if(glfwGetKey(window, LEFT_ROTATE_KEY[this->keyBinding]) == GLFW_PRESS){
        Orientation = glm::rotate(Orientation, glm::radians(-speed * sensitivity / 10), Up);
    }

    if(glfwGetKey(window, RIGHT_ROTATE_KEY[this->keyBinding]) == GLFW_PRESS){
        Orientation = glm::rotate(Orientation, glm::radians(speed * sensitivity / 10), Up);
    }

    if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS){
        speed = 0.5f;
    }else if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE){
        speed = 0.1f;
    }

    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS){
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        if(firstClick){
            glfwSetCursorPos(window, width/2, height/2);
            firstClick = false;
        }

        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        float rotX = sensitivity * (float)(mouseY - height/2) / height;
        float rotY = sensitivity * (float)(mouseX - width/2) / width;

        glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

        if(!(glm::angle(newOrientation, Up) <= glm::radians(5.0f) || glm::angle(newOrientation, -Up) <= glm::radians(5.0f))){
            Orientation = newOrientation;
        }

        Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

        glfwSetCursorPos(window, width/2, height/2);

    }else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE){
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        firstClick = true;
    }

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }

    updateMatrix();
}