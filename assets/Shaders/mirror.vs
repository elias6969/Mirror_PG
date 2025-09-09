#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // Expand 2D quad vertices into 3D plane (z=0)
    vec4 worldPos = model * vec4(aPos.x, aPos.y, 0.0, 1.0);
    gl_Position = projection * view * worldPos;
    TexCoord = aTexCoord;
}
