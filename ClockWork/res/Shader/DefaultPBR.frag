#version 460 core


in VS_OUT
{
	vec2 TexCoord;
	vec3 FragmentPosition;
	vec3 FragmentNormal;
	mat3 TBN;
} fs_in;


uniform sampler2D Diffuse0;
uniform sampler2D Normal0;
uniform sampler2D Metallic0;
uniform sampler2D Roughness0;

uniform bool HasNormalMap;
uniform float NormalStrength;
uniform vec3 CamPosition;

uniform float metallnessModifier;
uniform float smoothnessModifier;

struct SpotLight
{
	vec3 Position;
};

struct DirectLight
{
	vec3 Direction;
	vec3 Color;
	float Intensity;
};

uniform DirectLight directLight;
//uniform PointLight pointLights[POINT_LIGHTS];
uniform SpotLight spotlight;

out vec4 FragColor;


const float PI = 3.14159265359;

// done
vec3 FresnelSchlick(float vDotH, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - vDotH, 5.0);
}

// done
float NDF_GGX(float nDotH, float roughness) 
{
	float a2 = roughness * roughness;
    float d = (nDotH * nDotH * (a2  - 1) + 1);
    float denom = PI * d * d;

    return a2 / denom;
}

// 
float GeometrySchlickGGX(float nDotx, float roughness) 
{
    //float r = (roughness + 1.0);
    
	// TODO:if direct lighint (r + 1)2 / 8
	// if IBL r2 / 2
	//float k = (r * r) / 8.0;
	float k = roughness * roughness / 2;

    float denom = nDotx * (1.0 - k) + k ;

    return nDotx / denom;
}

float GeometrySmith(float nDotV, float nDotL, float roughness) 
{
	return GeometrySchlickGGX(nDotV, roughness) * GeometrySchlickGGX(nDotL, roughness);
}

vec3 GetNormal()
{
	vec3 normal = vec3(0.0);
	if(HasNormalMap)
	{
		normal = texture(Normal0,fs_in.TexCoord).xyz;
		normal = normal * 2.0 - 1.0;
		normal = normalize(fs_in.TBN * normal);
		normal *= NormalStrength;
	}
	else
	{
		normal = normalize(fs_in.FragmentNormal);
	}
	return normal;
}




void main()
{
	vec3 normal = GetNormal();	
	vec3 albedo = texture(Diffuse0, fs_in.TexCoord).rgb;

	float metallic = texture(Metallic0, fs_in.TexCoord).r * metallnessModifier;
	float roughness = max(texture(Roughness0, fs_in.TexCoord).r * (1 - smoothnessModifier), 0.04f);

	vec3 viewDir = normalize(CamPosition - fs_in.FragmentPosition); 
	vec3 ks = mix(vec3(0.04), albedo, metallic); // ks
	vec3 kd = (vec3(1.0) - ks) * (1 - metallic);

	// calculate these for each lights.
	vec3 lightDir = -directLight.Direction;
	vec3 halfway = normalize(viewDir + lightDir);
    
	float nDotL = max(dot(normal,lightDir), 0.000001);
	float nDotV = max(dot(normal,viewDir), 0.000001);
	float nDotH = max(dot(normal,halfway), 0.0);
	float vDotH = max(dot(viewDir,halfway), 0.0);

	float D = NDF_GGX(nDotH,roughness);
	vec3 F = FresnelSchlick(vDotH,ks);
	float G = GeometrySmith(nDotV,nDotL, roughness);
	

	vec3 fCookTorrence = (D * F * G) / (4 * nDotV * nDotL );
	vec3 fLambert = albedo / PI;
	

	vec3 BRDF = kd * fLambert + fCookTorrence;

	vec3 color = BRDF * directLight.Color * nDotL * directLight.Intensity;
	
	// add ambient factor
	color *= 1.03; 

	// apply gamma correction
	color = color / (color + vec3(1));

	color = pow(color, vec3(1 / 2.2));
	FragColor = vec4(color,1);
}

