#version 450 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNor;
layout (location = 2) in vec2 aTex;
layout (location = 3) in vec4 aCol;
layout (location = 4) in vec3 aTan;
layout (location = 5) in vec3 aBit;

uniform vec3 eyePosition;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out VS_OUT
{
	vec3 FragPos;
    vec2 TexCoords;
	
	mat3 TBN;
    vec3 TangentEyePos;
    vec3 TangentFragPos;
} vs_out;

void main()
{
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));   
    vs_out.TexCoords = aTex;

	mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 T = normalize(normalMatrix * aTan);
    vec3 N = normalize(normalMatrix * aNor);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    
	vs_out.TBN = transpose(mat3(T, B, N));
	vs_out.TangentEyePos  = vs_out.TBN * eyePosition;
    vs_out.TangentFragPos  = vs_out.TBN * vs_out.FragPos;
	
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}