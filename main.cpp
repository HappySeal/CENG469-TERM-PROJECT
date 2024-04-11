#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "headers/shaderUtils.h"
#include "headers/VBO.h"
#include "headers/EBO.h"
#include "headers/VAO.h"

int main(){
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Notating we are using OpenGL 3.x
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // Notating we are using OpenGL 3.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Notating we are using OpenGL Core Profile

    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.0f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.25f,  0.0f, 0.0f,
        0.0f,  0.5f, 0.0f,
        -0.25f,  0.0f, 0.0f

    };

    GLuint indices[] = {
        0, 1, 5,
        1, 2, 3,
        3, 4, 5
    };

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if(window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glewInit();

    // Shader
    Shader shader("./resources/Shaders/vert.glsl", "./resources/Shaders/frag.glsl");

    // Create Vertex Array Object, Vertex Buffer Object and Element Buffer Object
    VAO VAO1;
    VAO1.Bind();

    VBO VBO1(vertices, sizeof(vertices));
    EBO EBO1(indices, sizeof(indices));

    VAO1.LinkVBO(VBO1, 0);
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    while (!glfwWindowShouldClose(window)){
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        shader.Activate();

        VAO1.Bind();

        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);



        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    // Clean up
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();

    shader.Delete();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}