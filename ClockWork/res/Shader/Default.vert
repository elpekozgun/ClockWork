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
	mat3 TBN;
	vec3 FragmentPosition;
	vec2 TexCoord;
} vs_out;


uniform bool instanced;

void main()
{
	vec3 T = normalize(vec3(Model * vec4(inTangent, 0.0)));
	vec3 N = normalize(vec3(Model * vec4(inNormal,0.0)));
	vec3 B = normalize(cross(N,T));

	vs_out.TBN = mat3(T,B,N);
	vs_out.TexCoord = inTex;
	if(instanced)
	{
		gl_Position =  CamMat * inInstancedTransform * vec4(inPos, 1.0);
		vs_out.FragmentPosition = vec3(inInstancedTransform * vec4(inPos,1));
	}
	else
	{
		gl_Position = gl_Position =  CamMat * Model * vec4(inPos, 1.0);
		vs_out.FragmentPosition = vec3(Model * vec4(inPos,1));
	}
};