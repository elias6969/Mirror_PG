#version 330 core
out vec4 FragColor;

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoord;

struct Material {
    vec3 specular;
    float shininess;
};
uniform Material material;

struct Light {
    vec3 position;
};
uniform Light light;

uniform vec3 viewPos;
uniform vec3 color;        // main object color
uniform int useTexture;

uniform sampler2D texture_diffuse; // bound by Material::apply if set

void main()
{
    // ---- Ambient ----
    vec3 ambient = 0.1 * color;

    // ---- Diffuse ----
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * color;

    // ---- Specular ----
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = material.specular * spec;

    // ---- Final ----
    vec3 finalColor = ambient + diffuse + specular;

    if(useTexture == 1) {
        vec4 texColor = texture(texture_diffuse, TexCoord);
        FragColor = vec4(finalColor, 1.0) * texColor;
    } else {
        FragColor = vec4(finalColor, 1.0);
    }
}
