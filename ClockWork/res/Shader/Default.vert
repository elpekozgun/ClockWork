#version 420 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTex;
layout (location = 3) in vec3 inTangent;
layout (location = 4) in vec3 inBitangent;
layout (location = 5) in mat4 inInstancedTransform;

uniform mat4 CamMat;
uniform mat4 Model;

out VS_OUT
{
	vec2 TexCoord;
	vec3 FragmentPosition;
	mat3 TBN;
} vs_out;

uniform bool instanced;


void main()
{
	// http://www.lighthouse3d.com/tutorials/glsl-tutorial/the-normal-matrix/
	vs_out.TexCoord = inTex;

	mat3 normalMatrix = transpose(inverse(mat3(Model)));
    vec3 T = normalize(normalMatrix * inTangent);
    vec3 N = normalize(normalMatrix * inNormal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);

	vs_out.TBN = mat3(T, B, N);   


	if(instanced)
	{
		vs_out.FragmentPosition = vec3(inInstancedTransform * vec4(inPos,1));
		gl_Position =  CamMat * inInstancedTransform * vec4(inPos, 1.0);
	}
	else
	{
		vs_out.FragmentPosition = vec3(Model * vec4(inPos,1));
     	gl_Position =  CamMat * Model * vec4(inPos, 1.0);
	}
};


//#version 460 core
//
//layout (location = 0) in vec3 inPos;
//layout (location = 1) in vec3 inNormal;
//layout (location = 2) in vec2 inTex;
//layout (location = 3) in vec3 inTangent;
//layout (location = 4) in vec3 inBitangent;
//layout (location = 5) in mat4 inInstancedTransform;
//
//uniform mat4 CamMat;
//uniform mat4 Model;
//
////out VS_OUT
////{
////	vec3 FragmentPosition;
////	vec2 TexCoord;
////	mat3 TBN;
////} vs_out;
//
//out vec2 TexCoord;
//out	vec3 FragmentPosition;
//
//uniform bool instanced;
//
//void main()
//{
//	mat3 normalMatrix = mat3(Model);
////	if(instanced)
////	{
////		gl_Position =  CamMat * inInstancedTransform * vec4(inPos, 1.0);
////		vs_out.FragmentPosition = vec3(inInstancedTransform * vec4(inPos,1));
////		normalMatrix =  mat3(inInstancedTransform); //transpose(inverse(mat3(inInstancedTransform)));
////	}
////	else
////	{
////		gl_Position = gl_Position =  CamMat * Model * vec4(inPos, 1.0);
////		vs_out.FragmentPosition = vec3(Model * vec4(inPos,1));
////		normalMatrix = mat3(Model); //transpose(inverse(mat3(Model)));
////	}
//
//	vec3 T = normalize(vec3(normalMatrix * inTangent));
//	vec3 N = normalize(vec3(normalMatrix * inNormal));
//	vec3 B = normalize(cross(N,T));
//
//	//TBN = mat3(T,B,N);
//	TexCoord = inTex;
//	FragmentPosition = vec3(Model * vec4(inPos,1));
//	gl_Position = CamMat * Model * vec4(inPos, 1.0);
//
//};