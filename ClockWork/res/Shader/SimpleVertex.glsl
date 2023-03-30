#version 420 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec2 inTex;

out vec4 VertexColor;
out vec2 TexCoord;
uniform float scale;

uniform mat4 camMat;

//uniform mat4 Model;
//uniform mat4 View;
//uniform mat4 Projection;

void main()
{
   gl_Position = camMat * vec4(inPos.x * (1 + scale), inPos.y * (1 + scale), inPos.z * (1 + scale), 1.0);
   //gl_Position = Projection * View * Model * vec4(inPos.x * (1 + scale), inPos.y * (1 + scale), inPos.z * (1 + scale), 1.0);
   VertexColor = vec4(inColor, 1);
   TexCoord = inTex;
};