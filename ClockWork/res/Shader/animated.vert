#version 460 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTex;
layout (location = 3) in vec3 inTangent;
layout (location = 4) in vec3 inBitangent;
layout (location = 5) in ivec4 inBoneIds;
layout (location = 6) in vec4 inWeights;

#define MAX_BONES 100
#define MAX_BONE_INFLUENCE 4

uniform mat4 CamMat;
uniform mat4 Model;
uniform mat4 BoneMats[MAX_BONES];

out VS_OUT
{
	vec2 TexCoord;
	vec3 FragmentPosition;
	vec3 FragmentNormal;
	mat3 TBN;
} vs_out;

uniform bool instanced;

void CalcNormal(mat4 modelMat)
{
	vs_out.FragmentNormal = transpose(inverse(mat3(modelMat))) * inNormal;
	mat3 normalMatrix = transpose(inverse(mat3(modelMat)));
	vec3 T = normalize(normalMatrix * inTangent);
	vec3 N = normalize(normalMatrix * inNormal);
	//vec3 B = normalize(normalMatrix * inBitangent);
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);
	vs_out.TBN = mat3(T, B, N);   
};

void CalcNormal()
{
	// http://www.lighthouse3d.com/tutorials/glsl-tutorial/the-normal-matrix/
	mat3 normalMatrix = transpose(inverse(mat3(Model)));

	vec3 N = vec3(0);
	vec3 T = vec3(0);

	for(int i = 0; i < 4; i++)
	{
		int id = int(inBoneIds[i]);
		mat3 boneMat = mat3(BoneMats[id]);
		N += boneMat * inNormal * inWeights[i];
		T += boneMat * inTangent * inWeights[i];
	}

	N = normalize(normalMatrix * N);
	T = normalize(normalMatrix * T);
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);

	vs_out.TBN = mat3(T, B, N);   
	vs_out.FragmentNormal = N;
};


void main()
{
	vs_out.TexCoord = inTex;

	vec4 totalPos = vec4(0);
	vec4 totalNormal = vec4(1);
	for(int i = 0; i < 4; i++)
	{
		int id = int(inBoneIds[i]);
		if(id == -1)
			break;
		if(id >= MAX_BONES)
		{
			totalPos = vec4(inPos,1.0);
			break;
		}
		vec4 localPosition = BoneMats[id] * vec4(inPos, 1);
		totalPos += localPosition * inWeights[i];
	}
	if(totalPos == vec4(0.0))
		totalPos = vec4(inPos,1);
	
	vs_out.FragmentPosition = vec3(Model * totalPos);
	CalcNormal();
    gl_Position =  CamMat * Model * totalPos;
};





//
//void main3()
//{
//	// http://www.lighthouse3d.com/tutorials/glsl-tutorial/the-normal-matrix/
//	vs_out.TexCoord = inTex;
//
//	mat4 bone = mat4(0);
//
//	for(int i = 0; i < MAX_BONE_INFLUENCE; i++)
//	{
//		int boneId = int(inBoneIds[i]);
//		if(boneId >= 0)
//			bone += BoneMats[boneId] * inWeights[i];
//	}
//
////	if(inBoneIds[0] < MAX_BONES && inBoneIds[0] != -1)
////		bone += BoneMats[int(inBoneIds[0])] * inWeights[0];
////
////	if(inBoneIds[1] < MAX_BONES)
////		bone += BoneMats[int(inBoneIds[1])] * inWeights[1];
////
////	if(inBoneIds[2] < MAX_BONES)
////		bone += BoneMats[int(inBoneIds[2])] * inWeights[2];
////	
////	if(inBoneIds[3] < MAX_BONES)
////		bone += BoneMats[int(inBoneIds[3])] * inWeights[3];
//
//	vec4 totalPos = bone * vec4(inPos,1);
//	vs_out.FragmentPosition = vec3(Model * totalPos);
//	CalcNormal(Model);
//    gl_Position =  CamMat * Model * totalPos;  //vec4(inPos, 1.0);
//};
//
//
//
//void main2()
//{
//	// http://www.lighthouse3d.com/tutorials/glsl-tutorial/the-normal-matrix/
//	vs_out.TexCoord = inTex;
//
//	vec4 totalPos = vec4(0);
//	if(inBoneIds[0] != -1)
//	{
//		vec4 localPos = BoneMat1 * vec4(inPos,1);
//		totalPos += localPos * inWeights[0];
//	}
//
//	if(inBoneIds[1] != -1)
//	{
//		vec4 localPos = BoneMat2 * vec4(inPos,1);
//		totalPos += localPos * inWeights[1];
//	}
//
//	if(inBoneIds[2] != -1)
//	{
//		vec4 localPos = BoneMat3 * vec4(inPos,1);
//		totalPos += localPos * inWeights[2];
//	}
//
//	if(inBoneIds[3] != -1)
//	{
//		vec4 localPos = BoneMat4 * vec4(inPos,1);
//		totalPos += localPos * inWeights[3];
//	}
//	
//	vs_out.FragmentPosition = vec3(Model * totalPos);
//	CalcNormal(Model);
//    gl_Position =  CamMat * Model * totalPos;  //vec4(inPos, 1.0);
//};
//
//