#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float iTime;
uniform vec2 iResolution;

#define PI 3.1415926538

// wormhole settings
float a = 2.0;       // wormhole throat length
float M = 0.1;       // wormhole smoothness
float dt = 0.1;      // integration step
int maxSteps = 1000; // maximum steps

// camera settings
float camL = 5.0;    // camera distance
float zoom = 1.5;    // camera zoom

// wormhole function r(l)
float LtoR(float l) {
    float x = max(0.0, 2.0 * (abs(l) - a) / PI / M);
    return 1.0 + M * (x * atan(x) - 0.5 * log(1.0 + x * x));
}

// wormhole derivative
float LtoDR(float l) {
    float x = max(0.0, 2.0 * (abs(l) - a) / (PI * M));
    return 2.0 * atan(x) * sign(l) / PI;
}

mat3 rotationZ(float angle) {
    float s = sin(angle);
    float c = cos(angle);
    return mat3(
        c, -s, 0.0,
        s, c, 0.0,
        0.0, 0.0, 1.0
    );
}

vec2 DirToUV(vec3 dir)
{
    vec3 dirNormalized = normalize(dir);
    vec2 uv = vec2((atan(-dirNormalized.z,dirNormalized.x) + 1)/(2*PI), acos(-dirNormalized.y)/PI);
    return uv;
}

void main()
{
    camL = cos(iTime * 0.2) * 10.0;
    float deltaSpace = cos(iTime * 0.5 + PI) * PI / 2.0 + PI / 2.0;

    // ray projection
    vec2 uv = TexCoords - 0.5;
    vec3 vel = normalize(vec3(-zoom, uv));
    vec2 beta = normalize(vel.yz);

    // ray tracing
    float l = camL;
    float r = LtoR(camL);
    float dl = vel.x;
    float H = r * length(vel.yz);
    float phi = 0.0;
    float dr;

    int steps = 0;
    while (abs(l) < max(abs(camL) * 2.0, a + 2.0) && steps < maxSteps) {
        dr = LtoDR(l);
        r = LtoR(l);
        l += dl * dt;
        phi += H / (r * r) * dt;
        dl += H * H * dr / (r * r * r) * dt;
        steps++;
    }

    // sky direction
    float dx = dl * dr * cos(phi) - H / r * sin(phi);
    float dy = dl * dr * sin(phi) + H / r * cos(phi);
    vec3 vec = normalize(vec3(dx, dy * beta));
    vec3 cubeVec = vec3(-vec.x, vec.z, -vec.y);

    // set pixel color
    if (l < 0.0) {
        FragColor = texture(texture1, DirToUV(cubeVec));
    } else {
        FragColor = texture(texture2, DirToUV(cubeVec));
    }
}
