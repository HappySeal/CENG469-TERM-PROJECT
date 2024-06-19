#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#define STB_IMAGE_IMPLEMENTATION
#include "headers/stb_image.h"

#include "headers/shaderUtils.h"
#include "headers/Camera.h"

// Shader loading utility programs (not shown here for brevity)

//Global variables
float gCamL = 5.0f;
float gWormholeLenght = 2.0f;
float gWormholeSmoothness = 0.1f;
int gIntegrationStep = 1000;
bool gStopMotion = false;


bool firstClick = true;
float sensitivity = 100.0f;
float gWidth, gHeight;
glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, int button, int action, int mods);


void updateCam() {

}

void reshape(GLFWwindow* window, int w, int h)
{
	w = w < 1 ? 1 : w;
	h = h < 1 ? 1 : h;

    gWidth = w;
    gHeight = h;

	glViewport(0, 0, w, h);
}

glm::mat4 rotateZMat4(float angle) {
    return glm::mat4(
            glm::vec4(cos(angle), -sin(angle), 0.0f, 0.0f),
            glm::vec4(sin(angle), cos(angle), 0.0f, 0.0f),
            glm::vec4(0.0f, 0.0f, 1.0f, 0.0f),
            glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
    );
    
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
    glfwSwapInterval(1);

	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}


    glfwSetWindowSizeCallback(window, reshape);
	reshape(window, 800, 600); // need to call this once ourselves

    std::cout << "Created GLFW window\n" << std::endl;

    // Load shaders
    Shader wormhole("./resources/shaders/default.vert", "./resources/shaders/default.frag");
    Camera camera(gWidth, gHeight, glm::vec3(0.0f, 0.0f, 0.0f), 90.0f, 0.1f, 100.0f);

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
    unsigned char* data = stbi_load("./resources/textures/sky1.jpg", &width, &height, &nrChannels, 0);

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

    unsigned char* data2 = stbi_load("./resources/textures/sky2.jpg", &width, &height, &nrChannels, 0);

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
    // glfwSetMouseButtonCallback(window, mouse_callback);


    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    const char* glsl_version = "#version 330 core";
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Main render loop
    while (!glfwWindowShouldClose(window)) {
        // Input handling
        camera.HandleControl(window);

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
        wormhole.SetFloat("uCamL", gCamL);
        wormhole.SetFloat("uWormholeLenght", gWormholeLenght);
        wormhole.SetFloat("uWormholeSmoothness", gWormholeSmoothness);
        wormhole.SetInt("uIntegrationStep", gIntegrationStep);
        wormhole.SetBool("uStopMotion", gStopMotion);
        wormhole.SetMat4("uCamLocalToWorldMatrix", &camera.viewMatrix);
        wormhole.SetVec3f("uViewParams", &camera.viewParams);


        // Render quad
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            ImGui::Begin("Wormhole Settings");                          // Create a window called "Hello, world!" and append into it.

            //ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Stop", &gStopMotion);      // Edit bools storing our window open/close state
            //ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("Cam L", &gCamL, -10.0f, 10.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::SliderFloat("Wormhole Length", &gWormholeLenght, 0.2f, 10.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::SliderFloat("Wormhole Smoothness", &gWormholeSmoothness, 0.01f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::SliderInt("Integration Step", &gIntegrationStep, 0, 2000);            // Edit 1 float using a slider from 0.0f to 1.0f
            
            //ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            // if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            //     counter++;
            // ImGui::SameLine();
            // ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }


        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        // glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        //glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // Clean up
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}



// A, D: camL between -10 and 10, increasing and decreasing with 0.1
// W, S: wormholeLenght between 0.2 and 10, increasing and decreasing with 0.1
// Q, E: wormholeSmoothness between 0.01 and 1, increasing and decreasing with 0.01
// Z, X: integrationStep between 100 and 2000, increasing and decreasing with 100
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if ( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
    if (key == GLFW_KEY_A && action == GLFW_PRESS)
    {
        if(gCamL > -10.0f)
            gCamL -= 0.1f;
    }
    if (key == GLFW_KEY_D && action == GLFW_PRESS)
    {
        if(gCamL < 10.0f)
            gCamL += 0.1f;
    }
    if (key == GLFW_KEY_W && action == GLFW_PRESS)
    {
        if(gWormholeLenght < 10.0f)
            gWormholeLenght += 0.1f;
    }
    if (key == GLFW_KEY_S && action == GLFW_PRESS)
    {
        if(gWormholeLenght > 0.2f)
            gWormholeLenght -= 0.1f;
    }
    if (key == GLFW_KEY_E && action == GLFW_PRESS)
    {
        if(gWormholeSmoothness < 1.0f)
            gWormholeSmoothness += 0.01f;
    }
    if (key == GLFW_KEY_Q && action == GLFW_PRESS)
    {
        if(gWormholeSmoothness > 0.01f)
            gWormholeSmoothness -= 0.01f;
    }
    if (key == GLFW_KEY_X && action == GLFW_PRESS)
    {
        if(gIntegrationStep < 2000)
            gIntegrationStep += 100;
    }
    if (key == GLFW_KEY_Z && action == GLFW_PRESS)
    {
        if(gIntegrationStep > 100)
            gIntegrationStep -= 100;
    }


}

void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
    
    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS){
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        if(firstClick){
            glfwSetCursorPos(window, gWidth/2, gHeight/2);
            firstClick = false;
        }

        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        float rotX = sensitivity * (float)(mouseY - gHeight/2) / gHeight;
        float rotY = sensitivity * (float)(mouseX - gWidth/2) / gWidth;

        glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

        if(!(glm::angle(newOrientation, Up) <= glm::radians(5.0f) || glm::angle(newOrientation, -Up) <= glm::radians(5.0f))){
            Orientation = newOrientation;
        }

        Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

        glfwSetCursorPos(window, gWidth/2, gHeight/2);

    }else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE){
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        firstClick = true;
    }
}


