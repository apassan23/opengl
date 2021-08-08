#ifndef __SHADER_HPP__
#define __SHADER_HPP__

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

class Shader
{
private:
    GLuint programID;

public:
    Shader(std::string vertexShaderPath, std::string fragShaderPath);
    ~Shader();
    void use();
    GLuint getID();
    void setMat4(const char *name, glm::mat4 matrix);
    void setVec3(const char *name, float x, float y, float z);
    void setVec3(const char *name, glm::vec3 vec);
    void setVec3(const char *name, float v);
    void setFloat(const char *name, float v);
    void setInt(const char *name, int v);
    void setMatPtr(const char *name, const float *ptr);
};

#endif