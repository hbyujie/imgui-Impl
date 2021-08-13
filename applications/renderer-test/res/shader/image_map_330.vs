#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;

out vec2 FragTex;

void main()
{
    FragTex = aTex;
    gl_Position = vec4(aPos, 1.0);
}