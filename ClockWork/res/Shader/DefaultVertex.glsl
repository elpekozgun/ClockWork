#version 420 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec3 inColor;
layout (location = 3) in vec2 inTex;
layout (location = 4) in mat4 inInstancedTransform;

uniform mat4 CamMat;
uniform mat4 Model;

out vec2 TexCoord;
out vec3 FragmentNormal;
out vec3 FragmentPosition;
out vec3 FragmentColor;

uniform bool instanced;


void main()
{
	// http://www.lighthouse3d.com/tutorials/glsl-tutorial/the-normal-matrix/
//	FragmentNormal = mat3(transpose(inverse(Model))) * inNormal;
//	FragmentPosition = vec3(Model * vec4(inPos,1));
	FragmentColor = inColor;
	TexCoord = inTex;

	//if(instanced)
	//{
		FragmentNormal = mat3(transpose(inverse(inInstancedTransform))) * inNormal;
		FragmentPosition = vec3(inInstancedTransform * vec4(inPos,1));
		gl_Position =  CamMat * inInstancedTransform * vec4(inPos, 1.0);
	//}
	//else
	{
//		FragmentNormal = mat3(transpose(inverse(Model))) * inNormal;
//		FragmentPosition = vec3(Model * vec4(inPos,1));
//		gl_Position =  CamMat * Model * vec4(inPos, 1.0);
	}
};