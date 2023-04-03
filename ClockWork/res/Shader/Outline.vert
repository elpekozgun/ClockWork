#version 460 core

layout (location = 0) in vec3 inPos;

uniform mat4 CamMat;
uniform mat4 Model;
uniform mat4 Translation;
uniform mat4 Rotation;
uniform mat4 Scale;
uniform float Thickness;

void main()
{
	vec3 pos = vec3(Model * Translation * Rotation * Scale * Thickness * vec4(inPos, 1.0f));
	gl_Position = CamMat * vec4(pos,1.0f);
}
