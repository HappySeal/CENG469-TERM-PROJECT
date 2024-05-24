#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "headers/shaderUtils.h"
#include "headers/Camera.h"
#include "headers/Model.h"
#include "headers/Cubemap.h"
#include "headers/MedianCut.h"

#define WIDTH 800
#define HEIGHT 600
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

Camera* camera;
Cubemap* skybox;
MedianCut* medianCut;

enum RenderMode{
    LIGHTPROBE,
    MIRROR,
    GLASS,
    GLOSSY,
    SPECULARDISCO
} renderMode = LIGHTPROBE;

bool enableSpecular = false;



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

    camera = new Camera(WIDTH, HEIGHT, glm::vec3(0.0f, 0.0f, 3.0f), 90.0f, 0.1f, 100.0f);
    skybox = new Cubemap("./resources/hdr/Thumersbach.hdr", camera->projectionMatrix);
    medianCut = new MedianCut("./resources/hdr/Thumersbach.hdr");
    medianCut->calculate();

    skybox->width = WIDTH;
    skybox->height = HEIGHT;
    // Shader
    Shader mirrorShader("./resources/Shaders/vert.glsl", "./resources/Shaders/mirrorfrag.glsl");
    Shader lightProbeShader("./resources/Shaders/vert.glsl", "./resources/Shaders/lightprobefrag.glsl");
    Shader glassShader("./resources/Shaders/vert.glsl", "./resources/Shaders/glass.frag");
    Shader glossyShader("./resources/Shaders/vert.glsl", "./resources/Shaders/glossy.frag");
    Shader specularDiscoShader("./resources/Shaders/vert.glsl", "./resources/Shaders/disco.frag");


    Model model = Model("./resources/Models/sphere.obj", glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f), glm::vec4(1.0f, 1.0f, 0.2f, 1.0f));




    glm::mat4 modelMatrix = model.ModelingMatrix();

    mirrorShader.Activate();
    mirrorShader.SetMat4("model", &modelMatrix);
    mirrorShader.SetVec4f("color", model.color.toVec4f());

    lightProbeShader.Activate();
    lightProbeShader.SetMat4("model", &modelMatrix);

    glossyShader.Activate();
    glossyShader.SetMat4("model", &modelMatrix);

    specularDiscoShader.Activate();
    specularDiscoShader.SetMat4("model", &modelMatrix);

    glassShader.Activate();
    glassShader.SetMat4("model", &modelMatrix);

//    skybox->Bind();
//    skybox->skybox->SetMat4("projection", &camera->projectionMatrix);

    assert(glGetError() == GL_NO_ERROR);

    // Enable depth testing

    int scrWidth, scrHeight;
    glfwGetFramebufferSize(window, &scrWidth, &scrHeight);
    glViewport(0, 0, scrWidth, scrHeight);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
//
//    std::cout << "Light count " << (int)(medianCut->lightPointsByIter[medianCut->iterationCount]).size() << std::endl;
//    for (int i = 0; i < medianCut->lightPointsByIter[medianCut->iterationCount].size(); ++i) {
//        std::cout << "Light point " << i << ": " << medianCut->lightPointsByIter[medianCut->iterationCount][i].pos.x << " " << medianCut->lightPointsByIter[medianCut->iterationCount][i].pos.y << " " << medianCut->lightPointsByIter[medianCut->iterationCount][i].pos.z << " " << medianCut->lightPointsByIter[medianCut->iterationCount][i].color.r << " " << medianCut->lightPointsByIter[medianCut->iterationCount][i].color.g << " " << medianCut->lightPointsByIter[medianCut->iterationCount][i].color.b << std::endl;
//    }


    while (!glfwWindowShouldClose(window)){
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        // Clear the color buffer and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera->updateMatrix();
        camera->HandleControl(window);
        skybox->HandleControl(window);

        skybox->BindIrradiance();

        switch (renderMode) {
            case LIGHTPROBE:
                lightProbeShader.Activate();
                lightProbeShader.SetInt("environmentMap", 0);
                lightProbeShader.SetInt("numLights", medianCut->numberOfLights);
                lightProbeShader.SetFloat("exposure", skybox->exposure);
                lightProbeShader.SetVec3f("cameraPos", &camera->Position);
                for (int i = 0; i < medianCut->lights.size(); ++i) {
                    lightProbeShader.SetVec3f("lights[" + std::to_string(i) + "].pos", &medianCut->lights[i].pos);
                    lightProbeShader.SetVec3f("lights[" + std::to_string(i) + "].color", &medianCut->lights[i].color);
                }
                lightProbeShader.SetInt("enableSpecular", enableSpecular);
                lightProbeShader.SetMat4("skyboxMatrix", &skybox->skyboxModelMatrix);
                camera->Matrix(lightProbeShader, "camMatrix");
                //print
                break;
            case MIRROR:
                mirrorShader.Activate();
                mirrorShader.SetVec3f("cameraPos", &camera->Position);
                mirrorShader.SetFloat("exposure", skybox->exposure);
                mirrorShader.SetMat4("skyboxMatrix", &skybox->skyboxModelMatrix);
                camera->Matrix(mirrorShader, "camMatrix");
                break;
            case GLASS:
                glassShader.Activate();
                glassShader.SetVec3f("cameraPos", &camera->Position);
                glassShader.SetFloat("exposure", skybox->exposure);
                glassShader.SetMat4("skyboxMatrix", &skybox->skyboxModelMatrix);
                camera->Matrix(glassShader, "camMatrix");
                break;
            case GLOSSY:
                glossyShader.Activate();
                glossyShader.SetInt("environmentMap", 0);
                glossyShader.SetInt("numLights", medianCut->numberOfLights);
                glossyShader.SetFloat("exposure", skybox->exposure);
                glossyShader.SetVec3f("cameraPos", &camera->Position);
                for (int i = 0; i < medianCut->lights.size(); ++i) {
                    glossyShader.SetVec3f("lights[" + std::to_string(i) + "].pos", &medianCut->lights[i].pos);
                    glossyShader.SetVec3f("lights[" + std::to_string(i) + "].color", &medianCut->lights[i].color);
                }
                glossyShader.SetInt("enableSpecular", enableSpecular);

                glossyShader.SetVec3f("cameraPos", &camera->Position);
                glossyShader.SetFloat("exposure", skybox->exposure);
                glossyShader.SetMat4("skyboxMatrix", &skybox->skyboxModelMatrix);
                camera->Matrix(glossyShader, "camMatrix");
                break;
            case SPECULARDISCO:
                specularDiscoShader.Activate();
                specularDiscoShader.SetInt("environmentMap", 0);
                specularDiscoShader.SetInt("numLights", medianCut->numberOfLights);
                specularDiscoShader.SetFloat("exposure", skybox->exposure);
                specularDiscoShader.SetVec3f("cameraPos", &camera->Position);
                for (int i = 0; i < medianCut->lights.size(); ++i) {
                    specularDiscoShader.SetVec3f("lights[" + std::to_string(i) + "].pos", &medianCut->lights[i].pos);
                    specularDiscoShader.SetVec3f("lights[" + std::to_string(i) + "].color", &medianCut->lights[i].color);
                }
                specularDiscoShader.SetInt("enableSpecular", enableSpecular);
                specularDiscoShader.SetMat4("skyboxMatrix", &skybox->skyboxModelMatrix);
                camera->Matrix(specularDiscoShader, "camMatrix");
                break;
        }

        model.bindMesh();
        skybox->Bind();
        model.drawMesh();

        skybox->Draw(camera->viewMatrix);
        assert(glGetError() == GL_NO_ERROR);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    mirrorShader.Delete();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    skybox->OnKeyInput(key, action);
    medianCut->OnKeyInput(key, action);

    if (key == GLFW_KEY_F && action == GLFW_PRESS) {
        enableSpecular = !enableSpecular;
        std::cout << "Specular: " << enableSpecular << std::endl;
    }

    if(key == GLFW_KEY_1 && action == GLFW_PRESS){
        renderMode = LIGHTPROBE;
        std::cout << "Light Probe" << std::endl;
    }

    if(key == GLFW_KEY_2 && action == GLFW_PRESS){
        renderMode = MIRROR;
        std::cout << "Mirror" << std::endl;
    }

    if(key == GLFW_KEY_3 && action == GLFW_PRESS){
        renderMode = GLASS;
        std::cout << "Glass" << std::endl;
    }

    if(key == GLFW_KEY_4 && action == GLFW_PRESS){
        renderMode = GLOSSY;
        std::cout << "Glossy" << std::endl;
    }

    if(key == GLFW_KEY_5 && action == GLFW_PRESS){
        renderMode = SPECULARDISCO;
        std::cout << "Specular Disco" << std::endl;
    }

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}