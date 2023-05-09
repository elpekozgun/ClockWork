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

uniform samplerCube IrradianceMap;
uniform samplerCube PrefilterMap;
uniform sampler2D BrdfLut;

uniform bool HasNormalMap;
uniform float NormalStrength;
uniform vec3 CamPosition;

uniform vec3 AlbedoColor;

uniform float metallnessModifier;
uniform float smoothnessModifier;

struct SpotLight
{
	vec3 Position;
	vec3 Color;
	vec3 Direction;
	float CutOff;
	float OuterCutoff;
};

struct PointLight
{
	vec3 Position;
	vec3 Color;
};

struct DirectLight
{
	vec3 Direction;
	vec3 Color;
	float Intensity;
};

uniform DirectLight directLight;
uniform PointLight pointLight;
uniform SpotLight spotLight;

out vec4 FragColor;


const float PI = 3.14159265359;
const float MAX_LOD = 4.0;


// done
vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0 ,1), 5.0);
}

// includes surface roughness as parameter for diffuse IBL
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}   

// done
float NDF_GGX(float nDotH, float roughness) 
{
	float a = roughness * roughness;
	float a2 = a * a ;
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

vec3 CalculateDirectionalLight(DirectLight light, vec3 albedo, vec3 normal, float metallic, float roughness, vec3 f0)
{
	vec3 viewDir = normalize(CamPosition - fs_in.FragmentPosition); 

	// calculate these for each lights.
	vec3 lightDir = -light.Direction;
	vec3 halfway = normalize(viewDir + lightDir);
    
	float nDotL = max(dot(normal,lightDir), 0.000001);
	float nDotV = max(dot(normal,viewDir), 0.000001);
	float nDotH = max(dot(normal,halfway), 0.0);
	float vDotH = max(dot(viewDir,halfway), 0.0);

	float D = NDF_GGX(nDotH,roughness);
	vec3 F = FresnelSchlick(vDotH,f0);
	float G = GeometrySmith(nDotV,nDotL, roughness);
	
	vec3 fCookTorrence = (D * F * G) / (4 * nDotV * nDotL + 0.000001);
	vec3 fLambert = albedo / PI;

	//ks = F;
	vec3 kd = (vec3(1.0) - F) * (1 - metallic);

	vec3 BRDF = kd * fLambert + fCookTorrence;

	vec3 color = BRDF * light.Color * nDotL;

	return color;
}

vec3 CalculatePointLight(PointLight light, vec3 albedo, vec3 normal, float roughness, float metallic, vec3 f0)
{
	vec3 viewDir = normalize(CamPosition - fs_in.FragmentPosition); 

	// calculate these for each lights.
	vec3 lightDir = normalize(light.Position - fs_in.FragmentPosition);
	vec3 halfway = normalize(viewDir + lightDir);

	float dist = length(light.Position - fs_in.FragmentPosition);
	float attenuation = 1 / (dist * dist);
	vec3 radiance = light.Color * attenuation;
    
	float nDotL = max(dot(normal,lightDir), 0.000001);
	float nDotV = max(dot(normal,viewDir), 0.000001);
	float nDotH = max(dot(normal,halfway), 0.0);
	float vDotH = max(dot(viewDir,halfway), 0.0);

	float D = NDF_GGX(nDotH,roughness);
	vec3 F = FresnelSchlick(vDotH,f0);
	float G = GeometrySmith(nDotV,nDotL, roughness);
	

	vec3 fCookTorrence = (D * F * G) / (4 * nDotV * nDotL + 0.000001);
	vec3 fLambert = albedo / PI;
	
	vec3 ks = F;
	vec3 kd = (vec3(1.0) - ks) * (1 - metallic);


	vec3 BRDF = kd * fLambert + fCookTorrence;

	return BRDF * radiance * nDotL ;
}

vec3 CalculateSpotLight(SpotLight light, vec3 albedo, vec3 normal, float roughness, float metallic, vec3 f0)
{

	vec3 viewDir = normalize(CamPosition - fs_in.FragmentPosition); 

	// calculate these for each lights.
	vec3 lightDir = normalize(light.Position - fs_in.FragmentPosition);
	vec3 halfway = normalize(viewDir + lightDir);

	float theta = dot(lightDir, -light.Direction);
	float epsilon = light.CutOff - light.OuterCutoff;
	float intensity = clamp((theta - light.CutOff)/ epsilon, 0.0, 1.0);


	float dist = length(light.Position - fs_in.FragmentPosition);
	float attenuation = 1 / (dist * dist);
	
	vec3 radiance = light.Color * attenuation;
    
	float nDotL = max(dot(normal,lightDir), 0.000001);
	float nDotV = max(dot(normal,viewDir), 0.000001);
	float nDotH = max(dot(normal,halfway), 0.0);
	float vDotH = max(dot(viewDir,halfway), 0.0);

	float D = NDF_GGX(nDotH,roughness);
	vec3 F = FresnelSchlick(vDotH,f0);
	float G = GeometrySmith(nDotV,nDotL, roughness);
	
	vec3 fCookTorrence = (D * F * G) / (4 * nDotV * nDotL + 0.000001);
	vec3 fLambert = albedo / PI;
	vec3 ks = F;
	
	vec3 kd = (vec3(1.0) - ks) * (1 - metallic);
	vec3 BRDF = kd * fLambert + fCookTorrence;

	return BRDF * radiance * nDotL ;//* directLight.Intensity;
}

void main()
{
	vec3 normal = GetNormal();	
	
	// convert albedo color to linear space from sRGB
	vec3 albedo = pow(texture(Diffuse0, fs_in.TexCoord).rgb * AlbedoColor, vec3(2.2)) ;
	//vec3 albedo = pow(texture(Diffuse0, fs_in.TexCoord).rgb, vec3(2.2)) ;

	//float metallic = metallnessModifier;
	float metallic = texture(Metallic0, fs_in.TexCoord).r * metallnessModifier;
	//float metallic = texture(Metallic0, fs_in.TexCoord).r;
	
	//float roughness =  (1 - clamp(smoothnessModifier,0.0001,1.0));
	float roughness = texture(Roughness0, fs_in.TexCoord).r * (1 - clamp(smoothnessModifier,0.0001,0.9999));
	//float roughness = texture(Roughness0, fs_in.TexCoord).r;

	
	vec3 f0 = mix(vec3(0.04), albedo, metallic); 

	// DirectLight;
	vec3 color = vec3(0);
	
	color += CalculateDirectionalLight(directLight, albedo, normal, metallic, roughness, f0);
	color += CalculatePointLight(pointLight, albedo, normal, roughness, metallic, f0);
	color += CalculateSpotLight(spotLight, albedo, normal, roughness, metallic, f0);

	vec3 viewDir = normalize(CamPosition - fs_in.FragmentPosition); 

	vec3 F = fresnelSchlickRoughness(max(dot(normal, viewDir),0),f0, roughness);
	vec3 kd = (1 - F) * (1 - metallic);

	//switch mipLevel

	vec3 indirectDiffuse = texture(IrradianceMap, normal).rgb * albedo;

	vec3 prefilteredColor = textureLod(PrefilterMap, reflect(-viewDir, normal),roughness * MAX_LOD).rgb;
	vec2 brdf = texture(BrdfLut, vec2(max(dot(normal,viewDir),0.0)), roughness).rg;
	vec3 specular = prefilteredColor * (F * brdf.r + brdf.g);

	vec3 ambient = kd * indirectDiffuse + specular;

	color += ambient;

	// tone map
	color /= (color + vec3(1));

	// HDR tone mapping.
//	float exposure = 0.01;
//	color = vec3(1) - exp(-color * exposure);

	// apply gamma correction
	color = pow(color, vec3(1 / 2.2));
	FragColor = vec4(color,1);

	//FragColor = vec4(normal,1);
}

