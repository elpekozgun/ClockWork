#version 460 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTex;
layout (location = 3) in vec3 inTangent;
layout (location = 4) in vec3 inBitangent;
layout (location = 5) in vec4 inBoneIds;
layout (location = 6) in vec4 inWeights;

#define MAX_BONES 100
#define MAX_BONE_INFLUENCE 4


uniform mat4 BoneMat1;
uniform mat4 BoneMat2;
uniform mat4 BoneMat3;
uniform mat4 BoneMat4;

uniform mat4 CamMat;
uniform mat4 Model;

struct BoneMap
{
	vec4 BoneMatRow1;
	vec4 BoneMatRow2;
	vec4 BoneMatRow3;
	vec4 BoneMatRow4;

};

uniform BoneMap BoneMaps[MAX_BONES];

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


void main()
{
	// http://www.lighthouse3d.com/tutorials/glsl-tutorial/the-normal-matrix/
	vs_out.TexCoord = inTex;

	//vec4 totalPos = vec4(0);

//	mat4 b0 = mat4
//	(
//		BoneMaps[int(inBoneIds[0])].BoneMatRow1,
//		BoneMaps[int(inBoneIds[0])].BoneMatRow2,
//		BoneMaps[int(inBoneIds[0])].BoneMatRow3,
//		BoneMaps[int(inBoneIds[0])].BoneMatRow4
//	);
//
//	mat4 b1 = mat4
//	(
//		BoneMaps[int(inBoneIds[1])].BoneMatRow1,
//		BoneMaps[int(inBoneIds[1])].BoneMatRow2,
//		BoneMaps[int(inBoneIds[1])].BoneMatRow3,
//		BoneMaps[int(inBoneIds[1])].BoneMatRow4
//	);
//
//	mat4 b2 = mat4
//	(
//		BoneMaps[int(inBoneIds[2])].BoneMatRow1,
//		BoneMaps[int(inBoneIds[2])].BoneMatRow2,
//		BoneMaps[int(inBoneIds[2])].BoneMatRow3,
//		BoneMaps[int(inBoneIds[2])].BoneMatRow4
//	);
//
//	mat4 b3 = mat4
//	(
//		BoneMaps[int(inBoneIds[3])].BoneMatRow1,
//		BoneMaps[int(inBoneIds[3])].BoneMatRow2,
//		BoneMaps[int(inBoneIds[3])].BoneMatRow3,
//		BoneMaps[int(inBoneIds[3])].BoneMatRow4
//	);
//
//	mat4 bone = b0 * inWeights[0];
//	bone += b1 * inWeights[1];
//	bone += b2 * inWeights[2];
//	bone += b3 * inWeights[3];

	mat4 bone = BoneMats[int(inBoneIds[0])] * inWeights[0];
	bone += BoneMats[int(inBoneIds[1])] * inWeights[1];
	bone += BoneMats[int(inBoneIds[2])] * inWeights[2];
	bone += BoneMats[int(inBoneIds[3])] * inWeights[3];

	vec4 totalPos = bone * vec4(inPos,1);
	vs_out.FragmentPosition = vec3(Model * totalPos);
	CalcNormal(Model);
    gl_Position =  CamMat * Model * totalPos;  //vec4(inPos, 1.0);
};



void main2()
{
	// http://www.lighthouse3d.com/tutorials/glsl-tutorial/the-normal-matrix/
	vs_out.TexCoord = inTex;

	vec4 totalPos = vec4(0);
	if(inBoneIds[0] != -1)
	{
		vec4 localPos = BoneMat1 * vec4(inPos,1);
		totalPos += localPos * inWeights[0];
	}

	if(inBoneIds[1] != -1)
	{
		vec4 localPos = BoneMat2 * vec4(inPos,1);
		totalPos += localPos * inWeights[1];
	}

	if(inBoneIds[2] != -1)
	{
		vec4 localPos = BoneMat3 * vec4(inPos,1);
		totalPos += localPos * inWeights[2];
	}

	if(inBoneIds[3] != -1)
	{
		vec4 localPos = BoneMat4 * vec4(inPos,1);
		totalPos += localPos * inWeights[3];
	}
	
	vs_out.FragmentPosition = vec3(Model * totalPos);
	CalcNormal(Model);
    gl_Position =  CamMat * Model * totalPos;  //vec4(inPos, 1.0);
};



//void main()
//{
//	// http://www.lighthouse3d.com/tutorials/glsl-tutorial/the-normal-matrix/
//	vs_out.TexCoord = inTex;
//
//	vec4 totalPos = vec4(0);
//	for(int i = 0; i < 4; i++)
//	{
//		if(inBoneIds[i] == -1.0)
//		{
//			totalPos = vec4(inPos,1);
//			continue;
//		}
////		if(inBoneIds[i] >= MAX_BONES)
////		{
////			totalPos = vec4(inPos,1.0);
////			break;
////		}
//		vec4 localPosition = BoneMats[inBoneIds[i]] * vec4(inPos, 1);
//		totalPos += localPosition * inWeights[i] ;
//		//vec3 localNormal = mat3(finalBonesMatrices[boneIds[i]]) * inNormal;
//	}
//	
//	vs_out.FragmentPosition = vec3(Model * totalPos);
//	CalcNormal(Model);
//    gl_Position =  CamMat * Model * totalPos;  //vec4(inPos, 1.0);
//};
//


