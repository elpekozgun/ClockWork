#version 460 core

layout(location = 0) in vec3 inPos;

out vec3 FragmentPosition;

uniform mat4 Projection;
uniform mat4 View;

void main()
{
	FragmentPosition = inPos;
	gl_Position = Projection * View * vec4(FragmentPosition, 1);
}