#version 330 core
layout (location = 0) in vec3 aSpaceCoord; 

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    vec4 spacePos = projection * view * model * vec4(aSpaceCoord, 1.0);
    gl_Position = spacePos;
    TexCoords = vec3(aSpaceCoord.xy, - aSpaceCoord.z);
    
}