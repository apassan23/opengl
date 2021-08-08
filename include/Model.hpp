#ifndef __MODEL_HPP__
#define __MODEL_HPP__

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <Mesh.hpp>
#include <Shader.hpp>
#include <vector>
#include <string>

class Model
{
public:
    Model(char *path);
    Model(std::string &path);
    void Draw(Shader &shader);

private:
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<Texture> textures_loaded;
    void loadModel(std::string &path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string &typeName);
};

#endif