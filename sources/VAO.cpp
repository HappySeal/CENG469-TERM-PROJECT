//
// Created by Cafer SELLI on 11.04.2024.
//

#include "../headers/VAO.h"

VAO::VAO(){
    glGenVertexArrays(1, &ID);
}

void VAO::LinkVBO(VBO& VBO, GLuint layout){
    Bind();
    VBO.Bind();

    glVertexAttribPointer(layout, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(layout);

    VBO.Unbind();
    Unbind();
}

void VAO::Bind(){
    glBindVertexArray(ID);
}

void VAO::Unbind(){
    glBindVertexArray(0);
}

void VAO::Delete(){
    glDeleteVertexArrays(1, &ID);
}