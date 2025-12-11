//
// Created by ts on 2024/9/25.
//
/**
 * 模型类
 * 包含多个网格，多个纹理的模型。
 */
#ifndef LEARNOPENGL_MODEL_H
#define LEARNOPENGL_MODEL_H


#include <GLES3/gl3.h>
#include "Mesh.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

unsigned int TextureFromFile(const std::string& path, const string &directory);

class Model {
public:
    Model()=default;

    Model(string const &path) {
        loadModel(path);
    }

    void Draw(GLuint programId);

    void Delete() {
        for (auto & mesh : meshes)
            mesh.Delete();
    }

private:
    // 优化：将加载过的纹理储存在另一个vector中，在模型类的顶部声明为一个私有变量：
    vector<Texture> textures_loaded;
    vector<Mesh> meshes;
    string directory;

    void loadModel(string const &path);

    void processNode(aiNode *node, const aiScene *scene);

    Mesh processMesh(aiMesh *mesh, const aiScene *scene);

    vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, const string& typeName);
};

#endif //LEARNOPENGL_MODEL_H
