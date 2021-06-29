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

layout(location = 0) out Vertex
{
	vec3 position;
	vec3 normal;
	vec2 texcoord;
} vout;

void main()
{
	vout.position = vec3(model * vec4(aPos, 1.0));
	vout.normal = vec3(model * vec4(aNor, 0.0));
	vout.texcoord = aTex;

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}