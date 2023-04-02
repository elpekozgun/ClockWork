#version 420 core


// Move to include----------------------
struct DirectLight
{
	vec3 direction;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
	vec3 position;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float Kconstant;
	float Klinear;
	float Kquad;
};

struct SpotLight
{
	vec3 position;
	vec3 direction;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float cutOff;
	float outerCutOff;

	float Kconstant;
	float Klinear;
	float Kquad;
};

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	float shineness;
};

vec3 CalculateDirectLight(DirectLight light, vec3 normal, vec3 viewDir);
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
// -------------------------------------

in vec2 TexCoord;
in vec3 FragmentPosition;
in vec3 FragmentNormal;
in vec3 FragmentColor;

#define POINT_LIGHTS 1

uniform sampler2D Diffuse0;
uniform sampler2D Specular0;

uniform float Shineness;

//uniform Material material; implement later
//uniform DirectLight directLight;
uniform PointLight pointLights[POINT_LIGHTS];
uniform SpotLight spotlight;

uniform vec3 eyePosition;
bool IsBlinnPhong;

out vec4 FragColor;

void main()
{
	vec3 norm = normalize(FragmentNormal);
	vec3 viewdir = normalize(eyePosition - FragmentPosition);

	vec3 result = vec3(0.0f);

	//result += CalculateDirectLight(directLight, norm, viewdir);
	
	for	(int i = 0; i < POINT_LIGHTS; i++)
	{
		result += CalculatePointLight(pointLights[i], norm, FragmentPosition, viewdir);
	}
	
	result += CalculateSpotLight(spotlight, norm, FragmentPosition, viewdir);

	FragColor = vec4(result,1);
	//FragColor = texture(Diffuse0,TexCoord);
}

vec3 CalculateDirectLight(DirectLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);

	float diff = max(dot(normal,lightDir),0.0f);

	float spec = 0.0f;
	if(IsBlinnPhong)
	{
		vec3 halfwayDir = normalize(viewDir + lightDir);
		spec = pow( max(dot(normal, halfwayDir),0.0f),/*material.*/Shineness * 4);
	}
	else
	{
		vec3 reflectDir = reflect(-lightDir,normal);
		spec = pow( max(dot(viewDir, reflectDir),0.0f),/*material.*/Shineness);
	}

	vec3 ambient = light.ambient * vec3(texture(/*material.*/Diffuse0,TexCoord));
	vec3 diffuse = light.diffuse * diff * vec3(texture(/*material.*/Diffuse0, TexCoord));
	vec3 specular = light.specular * spec;// * vec3(max(texture(material.specular,TexCoord),1.0f));
	
	if(FragmentColor.x != 0.0f || FragmentColor.y != 0.0f || FragmentColor.z != 0)
	{
		return (max(FragmentColor + diffuse * 0.2f + specular,vec3(0.0f)));
	}
	
	return (max(ambient + diffuse + specular,vec3(0.0f)));
}

vec3 CalculatePointLight(PointLight light, vec3 normal,vec3 fragPos, vec3 viewDir)
{
	vec3 lightdir = normalize(light.position - fragPos);

	float diff = max(dot(normal, lightdir), 0.0f);

	vec3 reflectDir = reflect(-lightdir,normal);
	float spec = pow( max( dot(viewDir,reflectDir),0.0f),/*material.*/Shineness);

	float distance = length(light.position - fragPos);
	float attenuation = 1.0f / (light.Kconstant + light.Klinear * distance + light.Kquad * distance * distance );

	vec3 ambient = attenuation * light.ambient * vec3(texture(/*material.*/Diffuse0, TexCoord));
	vec3 diffuse = attenuation * light.diffuse * diff * vec3(texture(/*material.*/Diffuse0, TexCoord));
	vec3 specular = attenuation * light.specular * spec * vec3(texture(/*material.*/Specular0, TexCoord));

	return (max(ambient + diffuse + specular,vec3(0.0f)));
}

vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);

	float diff = max(dot(normal, lightDir), 0.0f);
	
	float spec = 0.0f;
	if(IsBlinnPhong)
	{
		vec3 halfwayDir = normalize(viewDir + lightDir);
		spec = pow( max(dot(normal, halfwayDir),0.0f),/*material.*/Shineness * 4);
	}
	else
	{
		vec3 reflectDir = reflect(-lightDir,normal);
		spec = pow( max(dot(viewDir, reflectDir),0.0f),/*material.*/Shineness);
	}


	float theta = dot(lightDir,-light.direction);
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.cutOff) / epsilon,0.0f,1.0f);

	float distance = length(light.position - fragPos);
	float attenuation = 1.0f / (light.Kconstant + light.Klinear * distance + light.Kquad * distance * distance );

	vec3 ambient = attenuation * light.ambient * vec3(texture(/*material.*/Diffuse0, TexCoord));
	vec3 diffuse = intensity * attenuation * light.diffuse * diff * vec3(texture(Diffuse0, TexCoord));
	vec3 specular = intensity * attenuation * light.specular * spec * vec3(texture(Specular0, TexCoord));

	if(FragmentColor.x != 0.0f || FragmentColor.y != 0.0f || FragmentColor.z != 0)
	{
		return (max(FragmentColor + diffuse * 0.2f + specular,vec3(0.0f)));
	}

	return (max(ambient + diffuse + specular,vec3(0.0f)));
}


//-----------------Distance - light strength------------------
//
//			Distance 	Constant 	Linear 	Quadratic
//			7 	1.0 	0.7 	1.8
//			13 	1.0 	0.35 	0.44
//			20 	1.0 	0.22 	0.20
//			32 	1.0 	0.14 	0.07
//			50 	1.0 	0.09 	0.032
//			65 	1.0 	0.07 	0.017
//			100 	1.0 	0.045 	0.0075
//			160 	1.0 	0.027 	0.0028
//			325 	1.0 	
//			200 	1.0 	0.022 	0.00190.014 	0.0007
//			600 	1.0 	0.007 	0.0002
//			3250 	1.0 	0.0014 	0.000007