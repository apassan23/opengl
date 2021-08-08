#ifndef __MESH_HPP__
#define __MESH_HPP__

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <Texture.hpp>
#include <Shader.hpp>
#include <vector>

class Mesh
{
public:
    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoords;
    };
    std::vector<Vertex> vertices;
    std::vector<Texture> textures;
    std::vector<GLuint> indices;
    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);
    void Draw(Shader &shader);

private:
    GLuint VAO, VBO, EBO;
    void setupMesh();
};
#endif