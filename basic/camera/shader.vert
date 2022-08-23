#version 430
layout (location=0) in vec3 pos;  
layout (location=1) in vec2 tex;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(pos, 1);
    // gl_Position = projection * view * vec4(pos, 1);
    // gl_Position = vec4(pos, 1);
    TexCoord = tex;
}