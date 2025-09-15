#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform vec3 viewPos;
uniform vec3 color; // ImGui tint

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;

struct Material {
    vec3 specular;
    float shininess;
};
uniform Material material;

void main()
{
    // texture base color
    vec3 texColor = texture(texture1, TexCoord).rgb;

    // ambient
    vec3 ambient = light.ambient * texColor;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texColor;

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * material.specular;

    // combine lighting and ImGui tint
    vec3 result = (ambient + diffuse + specular) * color;
    FragColor = vec4(result, 1.0);
}
