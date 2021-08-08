#include "Shader.hpp"
#include <string.h>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(std::string vertexShaderPath, std::string fragShaderPath)
{
    std::string vertexCode, fragCode;
    std::ifstream vertexFile, fragFile;
    std::stringstream vStream, fStream;

    vStream.exceptions(std::ifstream::badbit | std::ifstream::failbit);
    fStream.exceptions(std::ifstream::badbit | std::ifstream::failbit);

    try
    {
        vertexFile.open(vertexShaderPath);
        fragFile.open(fragShaderPath);
        vStream << vertexFile.rdbuf();
        fStream << fragFile.rdbuf();
        vertexFile.close();
        fragFile.close();
        vertexCode = vStream.str();
        fragCode = fStream.str();
    }
    catch (const std::exception &e)
    {
        std::cerr << "FILE ERROR : " << e.what() << '\n';
    }

    const char *vCode = vertexCode.c_str();

    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vCode, NULL);
    glCompileShader(vertexShader);

    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        char err[70] = "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n";
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::runtime_error(strcat(err, infoLog));
    }

    GLuint fragmentShader;
    const char *fCode = fragCode.c_str();
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fCode, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        char infoLog[512];
        char err[70] = "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n";
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::runtime_error(strcat(err, infoLog));
    }

    this->programID = glCreateProgram();
    glAttachShader(this->programID, vertexShader);
    glAttachShader(this->programID, fragmentShader);
    glLinkProgram(this->programID);

    glGetProgramiv(this->programID, GL_LINK_STATUS, &success);

    if (!success)
    {
        char infoLog[512];
        char err[50] = "ERROR::SHADER::PROGRAM::LINKING_FAILED\n";
        glGetProgramInfoLog(this->programID, 512, NULL, infoLog);
        std::runtime_error(strcat(err, infoLog));
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // std::cout << "\nVertex Shader Source : " << vertexCode;
    // std::cout << "\nFragment Shader Source : " << fragCode;
}

Shader::~Shader()
{
    glDeleteProgram(this->programID);
}

void Shader::use()
{
    glUseProgram(this->programID);
}

GLuint Shader::getID()
{
    return this->programID;
}

void Shader::setMat4(const char *name, glm::mat4 matrix)
{
    GLint location = glGetUniformLocation(this->programID, name);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setVec3(const char *name, float x, float y, float z)
{
    GLint location = glGetUniformLocation(this->programID, name);
    glUniform3fv(location, 1, glm::value_ptr(glm::vec3(x, y, z)));
}

void Shader::setVec3(const char *name, glm::vec3 vec)
{
    GLint location = glGetUniformLocation(this->programID, name);
    glUniform3fv(location, 1, glm::value_ptr(vec));
}
void Shader::setVec3(const char *name, float v)
{
    GLint location = glGetUniformLocation(this->programID, name);
    glUniform3fv(location, 1, glm::value_ptr(glm::vec3(v)));
}

void Shader::setFloat(const char *name, float v)
{
    GLint location = glGetUniformLocation(this->programID, name);
    glUniform1f(location, v);
}

void Shader::setMatPtr(const char *name, const float *ptr)
{
    GLint location = glGetUniformLocation(this->programID, name);
    glUniformMatrix4fv(location, 1, GL_FALSE, ptr);
}

void Shader::setInt(const char *name, int v)
{
    GLint location = glGetUniformLocation(this->programID, name);
    glUniform1i(location, v);
}