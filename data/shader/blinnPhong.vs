#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNor;
layout (location = 2) in vec2 aTex;
layout (location = 3) in vec4 aCol;
layout (location = 4) in vec2 aTan;
layout (location = 5) in vec2 aBit;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 position;
out vec3 normal;
out vec2 texcoord;

void main()
{
	position = vec3(model * vec4(aPos, 1.0));
	normal = vec3(model * vec4(aNor, 0.0));
	texcoord = aTex;

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}