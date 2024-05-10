#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "headers/shaderUtils.h"
#include "headers/VBO.h"
#include "headers/EBO.h"
#include "headers/VAO.h"
#include "headers/Camera.h"
#include "headers/Model.h"
#include "headers/Cubemap.h"

#define WIDTH 800
#define HEIGHT 600


GLfloat vertices[] = {
        -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,
        -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,
        0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,
        0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,
        0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,
};

GLuint indices[] =
{
                0, 1, 2,
                0, 2, 3,
                0, 1, 4,
                1, 2, 4,
                2, 3, 4,
                3, 0, 4
};

GLfloat lightVertices[] =
        { //     COORDINATES     //
                -0.1f, -0.1f,  0.1f,
                -0.1f, -0.1f, -0.1f,
                0.1f, -0.1f, -0.1f,
                0.1f, -0.1f,  0.1f,
                -0.1f,  0.1f,  0.1f,
                -0.1f,  0.1f, -0.1f,
                0.1f,  0.1f, -0.1f,
                0.1f,  0.1f,  0.1f
        };

GLuint lightIndices[] =
        {
                0, 1, 2,
                0, 2, 3,
                0, 4, 7,
                0, 7, 3,
                3, 7, 6,
                3, 6, 2,
                2, 6, 5,
                2, 5, 1,
                1, 5, 4,
                1, 4, 0,
                4, 5, 6,
                4, 6, 7
        };

int main(){
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Notating we are using OpenGL 3.x
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // Notating we are using OpenGL 3.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Notating we are using OpenGL Core Profile

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL);
    if(window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glewInit();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    Camera camera = Camera(WIDTH, HEIGHT, glm::vec3(0.0f, 0.0f, 3.0f), 45.0f, 0.1f, 100.0f);


    Cubemap skybox = Cubemap("./resources/hdr/Beach.hdr", camera.projectionMatrix);

    skybox.width = WIDTH;
    skybox.height = HEIGHT;
    // Shader
    Shader shader("./resources/Shaders/vert.glsl", "./resources/Shaders/frag.glsl");


    Shader lightShader("./resources/Shaders/light.vert","./resources/Shaders/light.frag");

    VAO lightVAO;
    lightVAO.Bind();

    VBO lightVBO(lightVertices, sizeof(lightVertices));
    EBO lightEBO(lightIndices, sizeof(lightIndices));

    lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(GLfloat), (void*)0);

    lightVAO.Unbind();
    lightVBO.Unbind();
    lightEBO.Unbind();



    Model model = Model("./resources/Models/sphere.obj", glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f), glm::vec4(1.0f, 1.0f, 0.2f, 1.0f));


    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    glm::vec3 lightPos = glm::vec3(1.2f, -1.0f, 2.0f);
    glm::mat4 lightModel = glm::mat4(1.0f);
    lightModel = glm::translate(lightModel, lightPos);

    glm::mat4 modelMatrix = model.ModelingMatrix();

    lightShader.Activate();
    lightShader.SetMat4("model", &lightModel);
    lightShader.SetVec4f("lightColor", &lightColor);

    shader.Activate();
    shader.SetMat4("model", &modelMatrix);
    shader.SetVec4f("lightColor", &lightColor);
    shader.SetVec3f("lightPos", &lightPos);
    shader.SetVec4f("color",model.color.toVec4f());

//    skybox.Bind();
//    skybox.skybox->SetMat4("projection", &camera.projectionMatrix);

    assert(glGetError() == GL_NO_ERROR);

    // Enable depth testing

    int scrWidth, scrHeight;
    glfwGetFramebufferSize(window, &scrWidth, &scrHeight);
    glViewport(0, 0, scrWidth, scrHeight);

    while (!glfwWindowShouldClose(window)){
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        // Clear the color buffer and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw the triangle
        camera.HandleInputs(window);
        skybox.HandleInput(window);


        camera.updateMatrix();

        skybox.BindIrradiance();

        shader.Activate();
        shader.SetVec3f("cameraPos", &camera.Position);
        camera.Matrix(shader, "camMatrix");
        model.bindMesh();
        skybox.Bind();
        shader.SetMat4("skyboxMatrix", &skybox.skyboxModelMatrix);
        model.drawMesh();

        lightShader.Activate();
        camera.Matrix(lightShader, "camMatrix");
        lightVAO.Bind();
        glDrawElements(GL_TRIANGLES, sizeof(lightIndices)/sizeof(int), GL_UNSIGNED_INT, 0);

        skybox.Draw(camera.viewMatrix);
        assert(glGetError() == GL_NO_ERROR);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    shader.Delete();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}