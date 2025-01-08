//
// Created by ts on 2024/9/25.
//

#include "Model.h"
#include "LogUtils.h"
#include "GLUtils.h"
#include "stb_image.h"
#include <assimp/port/AndroidJNI/AndroidJNIIOSystem.h>

void Model::Draw(GLuint programId) {
    for (unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].Draw(programId);
}

void Model::loadModel(string const &path) {
    Assimp::Importer importer;
    // 后期处理指令：aiProcess_Triangulate 将不是三角形图元的形状转为三角形；aiProcess_FlipUVs 反转纹理y轴；aiProcess_CalcTangentSpace 计算切线和副切线
    const aiScene *scene = importer.ReadFile(path,
                                             aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    // 判断场景和根节点是否为空，检查flag标记判断数据是否加载完整
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        LOGE("Model::loadModel path=%s, assimpError=%s", path.c_str(), importer.GetErrorString())
        return;
    }
    // 获取文件路径
    directory = path.substr(0, path.find_last_of('/'));
    // 将场景的根节点和场景对象传递给processNode遍历
    processNode(scene->mRootNode, scene);
}

//递归处理所有节点
void Model::processNode(aiNode *node, const aiScene *scene) {
    LOGI("Model::processNode start node->mNumMeshes = %d node->mNumChildren = %d", node->mNumMeshes, node->mNumChildren)
    // 处理节点所有的网格（如果有的话）
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    // 接下来对它的子节点重复这一过程
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

/**
 * 解析网格
 * @param mesh 为解析的网格对象
 * @param scene 场景对象，网格的材质需要从场景中获取
 * @return Mesh对象
 */
Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    LOGI("Model::processMesh start %d", mesh->mNumVertices)
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;

    // 处理顶点位置、法线和纹理坐标
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;

        // 获取单个顶点数据
        glm::vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;

        // 获取每个顶点对应的法线数据
        if (mesh->HasNormals()) {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }

        // Assimp允许一个模型在一个顶点上有最多8个不同的纹理坐标，我们不会用到那么多，我们只关心第一组纹理坐标。
        if (mesh->mTextureCoords[0]) {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        } else {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }
        if (mesh->mTangents) {
            // tangent
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.Tangent = vector;
            // bitangent
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.Bitangent = vector;
        }

        vertices.push_back(vertex);
    }

    // Assimp的接口定义了每个网格都有一个面(Face)数组，每个面代表了一个图元，一个面包含了多个索引，它们定义了在每个图元中，
    // 我们应该绘制哪个顶点，并以什么顺序绘制，所以如果我们遍历了所有的面，并储存了面的索引到indices这个vector中就可以了。
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    // 一个网格只包含了一个指向材质对象的索引，如果想要获取网格真正的材质，我们还需要索引场景的mMaterials数组。
    // 网格材质索引位于它的mMaterialIndex属性中，我们同样可以用它来检测一个网格是否包含有材质：
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        // 1. diffuse maps
        vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        // 2. specular maps
        vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        // 3. normal maps
        // Assimp的aiTextureType_NORMAL并不会加载它的法线贴图，而aiTextureType_HEIGHT却能，所以我们经常这样加载它们：
        std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        // 4. height maps，反射贴图放在这里
        std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
    }

    return Mesh(vertices, indices, textures);
}

// 一个材质对象的内部对每种纹理类型都存储了一个纹理位置数组。不同的纹理类型都以 aiTextureType_ 为前缀。
vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName) {
    LOGI("Model::loadMaterialTextures type = %s count = %d", typeName.c_str(), mat->GetTextureCount(type))
    vector<Texture> textures;
    // 优化：加载纹理时判断纹理路径是否与已经加载的纹理路径相同，如果相同，则跳过纹理的加载，直接使用定位到的纹理结构体为网格的纹理。
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++) {
            if (std::strcmp(textures_loaded[j].path.data, str.C_Str()) == 0) {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }

        // 如果纹理还没有被加载，则加载它
        if (!skip) {
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), this->directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture); // 添加到已加载的纹理中
        }
    }
    return textures;
}

// 加载纹理
unsigned int TextureFromFile(const string path, const string &directory) {

    string filename = string(path);
    filename = directory + '/' + filename;

    // 生成一个纹理对象
    unsigned int textureID;
    glGenTextures(1, &textureID);

    FUN_BEGIN_TIME("Model::TextureFromFile")

        // 根据文件地址，使用stb_img加载纹理贴图文件
        int width, height, nrComponents;
        unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
        if (data) {
            GLenum format;
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;

            // 加载纹理贴身数据并绑定到之前生成的纹理对象
            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D); // 创建多级渐远纹理

            // 配置纹理采样参数
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        } else {
            LOGV("Texture failed to load at path: %s", path.c_str())
            stbi_image_free(data);
        }

    FUN_END_TIME("Model::TextureFromFile")

    return textureID;
}




