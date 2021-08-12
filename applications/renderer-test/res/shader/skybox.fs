#version 450 core

in vec3 FragTex;

layout(location = 0) out vec4 FragColor;

layout(binding = 0) uniform samplerCube skyboxMap;

void main()
{    
    FragColor = texture(skyboxMap, FragTex);
}