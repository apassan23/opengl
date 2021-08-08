#ifndef __TEXTURE_HPP__
#define __TEXTURE_HPP__

#include <glad/glad.h>
#include <string>

class Texture
{
private:
    GLuint texture;
    int height;
    int width;
    int nrChannels;
    GLenum format;
    std::string type;

public:
    std::string path;
    Texture();
    Texture(GLuint &id);
    void bind();
    void load(const char *filename);
    void setID(GLuint &id);
    GLuint getID();
    int getHeight();
    int getWidth();
    void Delete();
    std::string getType();
    void setType(std::string &type);
    static void flipVertically(bool val);
};

#endif