#version 120 core

attribute vec3 aPos;
attribute vec2 aTex;

varying vec2 FragTex;

void main()
{
    FragTex = aTex;
    gl_Position = vec4(aPos, 1.0);
}