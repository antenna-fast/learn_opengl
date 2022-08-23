#version 430

out vec4 FragColor;

uniform vec4 ObjectColor;
uniform vec4 LightColor;

void main() {
    // basic lighting model
    float ambientStrength = 0.3;
    FragColor = ambientStrength * LightColor * ObjectColor;
}