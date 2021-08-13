#version 120 core

varying vec2 FragTex;

uniform sampler2D depthMap;

void main()
{             
    float depthValue = texture2D(depthMap, FragTex).r;
    gl_FragColor = vec4(vec3(depthValue), 1.0);
}