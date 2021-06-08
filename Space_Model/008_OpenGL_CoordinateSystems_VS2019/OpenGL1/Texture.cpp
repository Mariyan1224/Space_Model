#include "Texture.h"

#include <iostream>
#include <glad/glad.h>
#include <stb_image/stb_image.h>

Texture::Texture(const std::string& fileName)
{
    glGenTextures(1, &_texture);
	
    glBindTexture(GL_TEXTURE_2D, _texture);
	
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); // !!! Warning !!!!
 	
    // load and generate the texture
    int nrChannels;
    unsigned char* data = stbi_load(fileName.c_str(), &_width, &_height, &nrChannels, 0);
	
    if (data)
    {
        GLenum format = 1;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;

        glTexImage2D(GL_TEXTURE_2D, 0, format, _width, _height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
        return;
    }
	
    stbi_image_free(data);
}

Texture::Texture(const std::vector<std::string> textures_faces)
{
    glGenTextures(1, &_spaceTex);

    glBindTexture(GL_TEXTURE_CUBE_MAP, _spaceTex);

    int nrChannels;
    unsigned char* data;
    for (unsigned int i = 0; i < textures_faces.size(); i++)
    {
        data = stbi_load(textures_faces[i].c_str(), &_spaceFaceWidth, &_spaceFaceHight, &nrChannels, 0);
        if (data)
        {
            GLenum format = 1;
            if (nrChannels == 1)
                format = GL_RED;
            else if (nrChannels == 3)
                format = GL_RGB;
            else if (nrChannels == 4)
                format = GL_RGBA;
            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, format, _spaceFaceWidth, _spaceFaceHight, 0, format, GL_UNSIGNED_BYTE, data // check image format
            );
        }
        else {
            std::cout << "Cubemap tex failed to load at path: " << textures_faces[i] << std::endl;
        }
            stbi_image_free(data);
        
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}



void Texture::Bind(unsigned textureSlot)
{
    glActiveTexture(GL_TEXTURE0 + textureSlot);
    glBindTexture(GL_TEXTURE_2D, _texture);
}

void Texture::SpaceBind(unsigned textureSlot)
{
    glActiveTexture(GL_TEXTURE0 + textureSlot);
    glBindTexture(GL_TEXTURE_CUBE_MAP, _spaceTex);
}
