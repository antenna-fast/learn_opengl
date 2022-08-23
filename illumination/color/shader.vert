#version 430
layout (location=0) in vec3 pos;
layout (location=1) in vec2 normal;

out vec3 Normal;  // for frag shader
out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(pos, 1);
}
