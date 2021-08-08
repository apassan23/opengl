#include "Texture.hpp"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image.h"

Texture::Texture()
{
}

Texture::Texture(GLuint &id)
{
    this->texture = id;
}

void Texture::bind()
{
    glBindTexture(GL_TEXTURE_2D, this->texture);
}

void Texture::load(const char *filename)
{
    glGenTextures(1, &texture);
    this->bind();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);

    if (data)
    {
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    else
    {
        std::cout << "Warning : Failed to load the texture !\n";
    }
    stbi_image_free(data);
}

int Texture::getHeight()
{
    return this->height;
}

int Texture::getWidth()
{
    return this->width;
}

GLuint Texture::getID()
{
    return this->texture;
}

void Texture::Delete()
{
    glDeleteTextures(1, &texture);
}

std::string Texture::getType()
{
    return type;
}

void Texture::setType(std::string &type)
{
    this->type = type;
}

void Texture::setID(GLuint &id)
{
    this->texture = id;
}

void Texture::flipVertically(bool val)
{
    stbi_set_flip_vertically_on_load(val);
}