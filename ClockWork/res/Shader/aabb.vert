#version 460 core

layout (location = 0) in vec3 inPos;

uniform mat4 CamMat;
uniform mat4 Model;

void main()
{
	gl_Position = CamMat * Model * vec4(inPos,1);
}





