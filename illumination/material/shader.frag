#version 430

in vec3 fragPos;    // in world coord
in vec3 Normal;     // normalized normal

// uniform vec3 ObjectColor;  // we use material to replace object color
uniform vec3 LightColor;
uniform vec3 lightPos;  // in world coord
uniform vec3 viewPos;

struct Material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform Material material;

struct Light{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;

out vec4 FragColor;

void main() {
    // lighting model
    // 1. environment lighting
    // vec3 ambient = material.ambient * LightColor;
    // vec3 ambient = vec3(0.1) * material.ambient * LightColor;
    vec3 ambient = light.ambient * material.ambient;

    // 2. diffuse lighting
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(Normal, lightDir), 0.0);
    vec3 diffuse = (diff * material.diffuse) * light.diffuse;

    // 3. specular
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, Normal);  // reflect need the input as from light to frag, so we add -
    float spec = pow(max(dot(reflectDir, viewDir), 0), material.shininess); 
    vec3 specular = (material.specular * spec) * light.specular;

    // vec3 res = (ambient + diffuse + specular) * ObjectColor;
    vec3 res = ambient + diffuse + specular;  // we now use material to replace "ObjectColor"
    FragColor = vec4(res, 1.0); 
}