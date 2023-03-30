#version 420 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec2 inTex;
layout (location = 3) in vec3 inNormal;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragWorldPos;

uniform float scale	;

uniform mat4 CamMat;
uniform mat4 Model;

void main()
{
	FragWorldPos = vec3(Model * vec4(inPos,1));

	gl_Position = CamMat * vec4(inPos.x * (1 + scale), inPos.y * (1 + scale), inPos.z * (1 + scale), 1.0);
	
	TexCoord = inTex;
	// http://www.lighthouse3d.com/tutorials/glsl-tutorial/the-normal-matrix/
	Normal = mat3(transpose(inverse(Model))) * inNormal;
};