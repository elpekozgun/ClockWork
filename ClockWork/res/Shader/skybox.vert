#version 460 core

layout (location = 0) in vec3 inPos;

out vec3 TexCoords;

uniform mat4 CamMat;

void main()
{
	vec4 pos = CamMat * vec4(inPos,1.0f);
	gl_Position = pos.xyww;
	TexCoords = inPos;
}
