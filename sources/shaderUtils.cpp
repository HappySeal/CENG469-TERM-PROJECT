//
// Created by Cafer SELLI on 11.04.2024.
//

#include "../headers/shaderUtils.h"

std::string get_file_content(const std::string& filePath){
    std::ifstream fileStream(filePath);
    std::stringstream buffer;
    buffer << fileStream.rdbuf();
    if(buffer.str().empty()){
        std::cerr << "Error: File " << filePath << " not found or empty" << std::endl;
        exit(1);
    }
    return buffer.str();
}

Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath){
    std::string vertexShaderSource = get_file_content(vertexShaderPath);
    std::string fragmentShaderSource = get_file_content(fragmentShaderPath);

    const GLchar * vertexShaderSourceC = (GLchar *) vertexShaderSource.c_str();
    const GLchar* fragmentShaderSourceC = (GLchar *) fragmentShaderSource.c_str();

    ID = glCreateProgram();
    // Create Vertex Shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSourceC, NULL);
    // Compile Vertex Shader
    glCompileShader(vertexShader);

    // Create Fragment Shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSourceC, NULL);
    // Compile Fragment Shader
    glCompileShader(fragmentShader);

    // Attach Vertex and Fragment Shaders to Shader Program
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    // Link Shader Program
    glLinkProgram(ID);
    // Delete Shaders (for getting rid of intermediate files)
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::Activate(){
    glUseProgram(ID);
}

void Shader::Delete(){
    glDeleteProgram(ID);
}