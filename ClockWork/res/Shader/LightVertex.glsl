#version 330 core

layout (location = 0) in vec3 inPos;

uniform mat4 Model;
uniform mat4 CamMat;

void main()
{
	gl_Position = CamMat * Model * vec4(inPos, 1.0f);
}