#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "headers/stb_image.h"

#include "headers/shaderUtils.h"

// Shader loading utility programs (not shown here for brevity)

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

GLuint loadTexture(const std::string& path) {
    GLuint texture = 0;

//    GLuint texture;
//    std::cout << "BRO YOU ARE NOT EVEN COMING HERE";
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
//    // Set texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    // Load image, create texture and generate mipmaps
    int width, height, nrChannels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
//    if (data) {
//        GLenum format;
//        if (nrChannels == 1)
//            format = GL_RED;
//        else if (nrChannels == 3)
//            format = GL_RGB;
//        else if (nrChannels == 4)
//            format = GL_RGBA;
//        else{
//            std::cerr << "Failed to load texture: " << path << std::endl;
//            exit(-1);
//        }
////
//        try {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, data);
//            glGenerateMipmap(GL_TEXTURE_2D);
//        } catch (const std::exception& e) {
//            std::cerr << "Failed to load texture: " << path << std::endl;
//            exit(-1);
//        }
//    } else {
//        std::cerr << "Failed to load texture: " << path << std::endl;
//    }
//    stbi_image_free(data);
    return texture;
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    std::cout << "Initialized GLFW\n" << std::endl;

    // Set up OpenGL context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Wormhole", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glewInit();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);


    std::cout << "Created GLFW window\n" << std::endl;

    // Load shaders
    Shader wormhole("./resources/shaders/default.vert", "./resources/shaders/default.frag");

    // Set up vertex data and buffers and configure vertex attributes
    float vertices[] = {
            // positions          // texture coords
            -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
            1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
            1.0f,  1.0f, 0.0f,  1.0f, 1.0f
    };
    unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
    };

    std::cout << "Loaded shaders\n" << std::endl;

    assert(glGetError() == GL_NO_ERROR);

    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    std::cout << "Generated VAO, VBO, EBO\n" << std::endl;

    assert(glGetError() == GL_NO_ERROR);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    std::cout << "Bound VBO\n" << std::endl;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    std::cout << "Bound EBO\n" << std::endl;

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    std::cout << "Enabled vertex attribute\n" << std::endl;

    // Texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    std::cout << "Enabled texture attribute\n" << std::endl;

    assert(glGetError() == GL_NO_ERROR);

    // Load and create textures
    GLuint texture1;
    GLuint texture2;
    std::cout << "Loading textures\n" << std::endl;
    int width, height, nrChannels;
    unsigned char* data = stbi_load("resources/textures/sky1.jpg", &width, &height, &nrChannels, 0);


    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);

    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

//    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
//    }

    unsigned char* data2 = stbi_load("resources/textures/sky2.png", &width, &height, &nrChannels, 0);

    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);


//    if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data2);
//    }

    std::cout << "Textures loaded\n" << std::endl;

    // Bind textures and set texture parameters here (omitted for brevity)
    wormhole.Activate();

    assert(glGetError() == GL_NO_ERROR);

    glfwSetKeyCallback(window, key_callback);

    // Main render loop
    while (!glfwWindowShouldClose(window)) {
        // Input handling

        // Render
        glClear(GL_COLOR_BUFFER_BIT);

        // Bind textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        wormhole.SetInt("texture1", 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        wormhole.SetInt("texture2", 1);

        // Set uniforms
        wormhole.SetFloat("iTime", (float)glfwGetTime());
        wormhole.SetVec2f("iResolution", new glm::vec2(800.0f, 600.0f));


        // Render quad
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

