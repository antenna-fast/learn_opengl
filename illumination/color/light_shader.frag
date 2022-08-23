#version 430

out vec4 FragColor;

uniform vec4 LightColor;

void main() {
    // basic lighting model
    FragColor=vec4(1, 1, 1, 0);
}