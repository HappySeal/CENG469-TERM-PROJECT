#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform mat4 camMatrix;

out vec3 color;

uniform float scale;

void main(){
   gl_Position = camMatrix * vec4(aPos, 1.0);
   color = aColor;
}