#version 420 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragWorldPos;

uniform sampler2D diffuse;
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
	float spec = pow(max(dot(viewDir,reflection),0), 32);
	float specular = spec * specularLight;


	FragColor = texture(diffuse, TexCoord) * LightColor * (diffuseColor + ambient + specular);
}