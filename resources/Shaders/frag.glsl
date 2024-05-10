#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 currentPos;

uniform vec4 lightColor;
uniform vec4 color;
uniform vec3 lightPos;
uniform vec3 cameraPos;

uniform mat4 skyboxMatrix;
uniform samplerCube skybox;

void main(){
   float ambient = 0.2f;

   vec3 mnormal = normalize(Normal);
   vec3 lightDir = normalize(lightPos - currentPos);
   vec3 I = normalize(currentPos - cameraPos);
   vec3 R = reflect(I, mnormal);

   float diff = max(dot(mnormal, lightDir), 0.0);

   float specularStrength = 0.5;
   vec3 viewDir = normalize(cameraPos - currentPos);
   vec3 reflectDir = reflect(-lightDir, mnormal);
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

   float specular = specularStrength * spec;

   FragColor = vec4(texture(skybox, R).rgb, 1.0);


}