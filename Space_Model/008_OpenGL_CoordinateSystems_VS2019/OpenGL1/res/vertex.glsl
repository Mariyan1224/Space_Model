#version 330 core
layout (location = 0) in vec3 aEarthPos;   // the position variable has attribute position 0
layout (location = 1) in vec3 aEarthNormalCoords; 
layout (location = 2) in vec2 aEarthTexCoords;
  
out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 orbit_model;

void main()
{
    gl_Position = projection * view * orbit_model * model *  vec4(aEarthPos, 1.0) ;

    FragPos = vec3(model * vec4(aEarthPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aEarthNormalCoords;  
    TexCoords = aEarthTexCoords;
  
  
}   