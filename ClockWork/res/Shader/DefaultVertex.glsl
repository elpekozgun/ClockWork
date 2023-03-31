#version 420 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec3 inColor;
layout (location = 3) in vec2 inTex;

uniform float scale	;
uniform mat4 CamMat;
uniform mat4 Model;

out vec2 TexCoord;
out vec3 FragmentNormal;
out vec3 FragmentPosition;
out vec3 FragmentColor;

void main()
{
	// http://www.lighthouse3d.com/tutorials/glsl-tutorial/the-normal-matrix/
	FragmentNormal = mat3(transpose(inverse(Model))) * inNormal;
	FragmentPosition = vec3(Model * vec4(inPos,1));
	FragmentColor = inColor;
	TexCoord = inTex;
	
	gl_Position = CamMat * vec4(inPos.x * (1 + scale), inPos.y * (1 + scale), inPos.z * (1 + scale), 1.0);
};