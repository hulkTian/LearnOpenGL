//
// Created by ts on 2024/9/25.
//
/**
 * 网格类
 * 使用Assimp，可以加载不同的模型到程序中，但是载入后它们都被储存为Assimp的数据结构。
 * 网格(Mesh)代表的是单个的可绘制实体，是OpenGL能够理解的格式。
 * 一个网格应该至少需要一系列的顶点，每个顶点包含一个位置向量、一个法向量和一个纹理坐标向量。
 * 一个网格还应该包含用于索引绘制的索引以及纹理形式的材质数据（漫反射/镜面光贴图）。
 */

#ifndef LEARNOPENGL_MESH_H
#define LEARNOPENGL_MESH_H

#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "assimp/texture.h"

#include <string>
#include <vector>
#include <GLES3/gl3.h>

using namespace std;

#define MAX_BONE_INFLUENCE 4

//顶点结构体，包含：顶点数据、法线数据、纹理坐标数据
struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
    //bone indexes which will influence this vertex
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    //weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];
};

struct Texture {
    unsigned int id;
    string type;
    aiString path;// 优化：存储纹理路径用于与其它纹理进行比较
};

class Mesh {
public:
    //网格数据
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
    void Draw(GLuint programId);
    void Delete();

private:
    unsigned int VAO,VBO,EBO;
    void setupMesh();
};


#endif //LEARNOPENGL_MESH_H
