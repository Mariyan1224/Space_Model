#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube spaceTex;

void main()
{    
    FragColor = texture(spaceTex, TexCoords);
}