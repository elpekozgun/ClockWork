#version 420 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragWorldPos;

uniform sampler2D DiffuseMap;
uniform sampler2D SpecularMap;

uniform vec4 LightColor;
uniform vec3 LightPos;
uniform vec3 CamPos;


void main()
{
	float ambient = 0.2f;
	
	vec3 normal = normalize(Normal);
	vec3 lightDir = normalize(LightPos - FragWorldPos);
	float diffuseColor = max(dot(normal,lightDir),0);

	float specularLight = 0.5f;
	vec3 viewDir = normalize(CamPos - FragWorldPos);
	vec3 reflection = reflect(-lightDir,normal);
	float specular = pow(max(dot(viewDir,reflection),0), 16);
	specular *= specularLight;

	FragColor = (texture(DiffuseMap, TexCoord) * (diffuseColor + ambient) + texture(SpecularMap, TexCoord) * specular) * LightColor;
}