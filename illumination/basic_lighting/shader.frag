#version 430

in vec3 fragPos;  // in world coord
in vec3 Normal;

uniform vec3 ObjectColor;
uniform vec3 LightColor;

uniform vec3 lightPos;  // in world coord
uniform vec3 viewPos;

out vec4 FragColor;

void main() {
    // lighting model
    float ambientStrength = 0.3;  // environment lighting
    vec3 ambient = ambientStrength * LightColor;

    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(Normal, lightDir), 0.0);
    vec3 diffuse = diff * LightColor;

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, Normal);  // reflect need the input as from light to frag, so we add -
    float spec = pow(max(dot(reflectDir, viewDir), 0), 32); 
    vec3 specular = 0.3 * spec * LightColor;

    vec3 res = (ambient + diffuse + specular) * ObjectColor;
    FragColor = vec4(res, 1.0); 
}