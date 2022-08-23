#version 430

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D ourTexture;

void main() {
    // "FragColor = vec4(1, 1, 0, 1);           // fixed color
    FragColor = texture(ourTexture, TexCoord);  // texture: buid-in function<sampler, tex coordinate>
}