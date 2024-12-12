//
// Created by ts on 2024/12/6.
//

#include "ResourceManager.h"
#include "GLUtils.h"

#include "stb_image.h"

// Instantiate static variables
std::map<std::string, Texture2D>    ResourceManager::Textures;
std::map<std::string, Shader>       ResourceManager::Shaders;

Shader ResourceManager::LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile,
                                   const GLchar *gShaderFile, std::string name) {
    Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
    return Shaders[name];
}

Shader ResourceManager::GetShader(std::string name) {
    return Shaders[name];
}

Texture2D ResourceManager::LoadTexture(const GLchar *file, GLboolean alpha, std::string name) {
    Textures[name] = loadTextureFromFile(file, alpha);
    return Textures[name];
}

Texture2D& ResourceManager::GetTexture(std::string name) {
    return Textures[name];
}

void ResourceManager::Clear() {
    // (Properly) delete all shaders
    for (auto iter: Shaders)
        glDeleteProgram(iter.second.ID);
    // (Properly) delete all textures
    for (auto iter: Textures)
        glDeleteTextures(1, &iter.second.ID);
}

Shader ResourceManager::loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile,
                                           const GLchar *gShaderFile) {
    // 1. Retrieve the vertex/fragment source code from filePath
    // 1. 从 filePath 检索顶点 / 片段源代码
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    try {
        //加载顶点着色器代码
        vertexCode = GLUtils::openTextFile(vShaderFile);

        //加载片段着色器代码
        fragmentCode = GLUtils::openTextFile(fShaderFile);

        // If geometry shader path is present, also load a geometry shader
        // 如果存在几何着色器路径，还加载几何着色器
        if (gShaderFile != nullptr) {
            geometryCode = GLUtils::openTextFile(gShaderFile);
        }
    }
    catch (std::exception e) {
        LOGE("ERROR::SHADER: Failed to read shader files")
    }
    const GLchar *vShaderCode = vertexCode.c_str();
    const GLchar *fShaderCode = fragmentCode.c_str();
    const GLchar *gShaderCode = geometryCode.c_str();
    // 2. Now create shader object from source code
    // 2. 从源代码创建着色器对象
    Shader shader;
    shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
    return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const GLchar *file, GLboolean alpha) {
    // Create Texture object
    Texture2D texture;
    if (alpha) {
        texture.Internal_Format = GL_RGBA;
        texture.Image_Format = GL_RGBA;
    }

    // Load image
    int width, height, nrComponents;
    unsigned char *image = stbi_load(file, &width, &height, &nrComponents, 0);
    if (image == nullptr) {
        // 图片解码失败的处理逻辑
        LOGD("Image data is null")
        return texture;
    }
    // Now generate texture
    texture.Generate(width, height, image);
    // And finally free image data
    stbi_image_free(image);
    return texture;
}