#version 330 core
layout (location = 0) in vec3 aSunPos;   // the position variable has attribute position 0
layout (location = 1) in vec3 aSunNormalCoords; 
layout (location = 2) in vec2 aSunTexCoords;

out vec2 sunTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view *model* vec4(aSunPos, 1.0);
    sunTexCoord = aSunTexCoords;
}   