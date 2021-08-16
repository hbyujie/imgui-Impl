#version 330 core

uniform sampler2D colorMap;

in vec2 FragTex;

out vec4 FragColor;

void main()
{
    FragColor = vec4(texture2D(colorMap, FragTex).rgb, 1.0);
}