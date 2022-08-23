#version 430

layout (location=0) in vec3 pos;
layout (location=1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// for frag shader
out vec3 fragPos;
out vec3 Normal;  

void main() {
    gl_Position = projection * view * model * vec4(pos, 1);

    fragPos = vec3(model * vec4(pos, 1));  // trans to world coordinate
    // Normal = normal;  // TODO
    Normal = mat3(transpose(inverse(model))) * normal;
}
