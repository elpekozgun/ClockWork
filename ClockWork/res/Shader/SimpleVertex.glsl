#version 420 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inColor;

out vec4 VertexColor;
uniform float scale;

void main()
{
   gl_Position = vec4(inPos.x * (1 + scale), inPos.y * (1 + scale), inPos.z * (1 + scale), 1.0);
   VertexColor = vec4(inColor, 1);
};