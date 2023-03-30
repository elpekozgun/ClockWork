#version 420 core

out vec4 FragColor;
in vec4 VertexColor;
in vec2 TexCoord;

uniform sampler2D diffuse;

void main()
{
	//FragColor = vec4(0.8f,0.3f, 0.02f, 1.0f);
	FragColor = texture(diffuse, TexCoord);

	//FragColor = VertexColor;
}