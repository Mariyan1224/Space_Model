#version 330 core
out vec4 FragColor;  

in vec2 sunTexCoord;

uniform sampler2D sunTexture;

void main()
{
    FragColor = texture(sunTexture, sunTexCoord);

}