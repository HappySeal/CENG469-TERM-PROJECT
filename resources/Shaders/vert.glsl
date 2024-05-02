#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

uniform mat4 model;
out vec3 color;

uniform float scale;

void main(){
   gl_Position = model * vec4(aPos.x + aPos.x * scale, aPos.y + aPos.y * scale, aPos.z + aPos.z + scale, 1.0);
   color = aColor;
}