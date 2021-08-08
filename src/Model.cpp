#include "Model.hpp"

#include <iostream>

Model::Model(char *path)
{
    std::string fpath = std::string(path);
    loadModel(fpath);
    std::cout << "Total Textures Loaded : " << textures_loaded.size() << "\n";
}

Model::Model(std::string &path)
{
    loadModel(path);
    std::cout << "Total Textures Loaded : " << textures_loaded.size() << "\n";
}

void Model::Draw(Shader &shader)
{
    for (unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].Draw(shader);
}

void Model::loadModel(std::string &path)
{

    unsigned int ppsteps = aiProcess_CalcTangentSpace |         // calculate tangents and bitangents if possible
                           aiProcess_JoinIdenticalVertices |    // join identical vertices/ optimize indexing
                           aiProcess_ValidateDataStructure |    // perform a full validation of the loader's output
                           aiProcess_ImproveCacheLocality |     // improve the cache locality of the output vertices
                           aiProcess_RemoveRedundantMaterials | // remove redundant materials
                           aiProcess_FindDegenerates |          // remove degenerated polygons from the import
                           aiProcess_FindInvalidData |          // detect invalid model data, such as invalid normal vectors
                           aiProcess_GenUVCoords |              // convert spherical, cylindrical, box and planar mapping to proper UVs
                           aiProcess_TransformUVCoords |        // preprocess UV transformations (scaling, translation ...)
                           aiProcess_FindInstances |            // search for instanced meshes and remove them by references to one master
                           aiProcess_LimitBoneWeights |         // limit bone weights to 4 per vertex
                           aiProcess_OptimizeMeshes |           // join small meshes, if possible;
                           aiProcess_SplitByBoneCount |         // split meshes with too many bones. Necessary for our (limited) hardware skinning shader
                           aiProcess_Triangulate |
                           aiProcess_GenSmoothNormals | // generate smooth normal vectors if not existing
                           aiProcess_SplitLargeMeshes | // split large, unrenderable meshes into submeshes
                           aiProcess_Triangulate |      // triangulate polygons with more than 3 edges
                           //aiProcess_ConvertToLeftHanded    | // convert everything to D3D left handed space
                           aiProcess_SortByPType |
                           0;

    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, ppsteps);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "Error::ASSIMP:: " << importer.GetErrorString() << "\n";
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
    static int count = 0;
    // process the node's meshes
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Mesh::Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    static std::string diffStr = "texture_diffuse";
    static std::string specStr = "texture_specular";

    // extracting vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Mesh::Vertex vertex;
        glm::vec3 vector(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        vertex.position = vector;
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.normal = vector;

        if (mesh->mTextureCoords[0])
        {
            glm::vec2 vec(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
            vertex.texCoords = vec;
        }
        else
            vertex.texCoords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, diffStr);
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, specStr);
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string &typeName)
{
    std::vector<Texture> textures;
    for (int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for (int j = 0; j < textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if (!skip)
        {
            Texture texture;
            std::string wd = directory;
            wd.push_back('/');
            std::string filepath = wd + std::string(str.C_Str());
            texture.load(filepath.c_str());
            texture.setType(typeName);
            texture.path = std::string(str.C_Str());
            textures_loaded.push_back(texture);
        }
    }
    return textures;
}