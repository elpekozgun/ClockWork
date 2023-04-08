#version 460 core

out vec4 FragColor;
in vec3 TexCoords;

uniform samplerCube Skybox;

void main()
{    
    //gamma correction
    vec3 color =texture(Skybox, TexCoords).rgb; 
    color = pow(color, vec3(1.0/2.2));
    FragColor = vec4(color,1);
}