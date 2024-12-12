//
// Created by ts on 2024/12/6.
//

#ifndef LEARNOPENGL_RESOURCEMANAGER_H
#define LEARNOPENGL_RESOURCEMANAGER_H

#include <map>
#include <string>

#include "texture2D.h"
#include "shader.h"
#include "Audio.h"

// A static singleton ResourceManager class that hosts several
// functions to load Textures and Shaders. Each loaded texture
// and/or shader is also stored for future reference by string
// handles. All functions and resources are static and no
// public constructor is defined.
// 单例资源管理类，包含了加载纹理和着色器的方法。每个加载的纹理和/或着色器被存放在映射表中，可以通过字符串建来引用。
// 所有的资源和函数都是静态的，并且没有定义公开的构造方法
class ResourceManager {
public:
    // Resource storage
    static std::map<std::string, Shader> Shaders;
    static std::map<std::string, Texture2D> Textures;

    // Loads (and generates) a shader program from file loading vertex, fragment (and geometry)
    // shader's source code. If gShaderFile is not nullptr, it also loads a geometry shader
    // 加载顶点着色器代码文件、片段着色器代码文件和几何着色器代码文件，并创建着色器程序。
    // 如果片段着色器代码文件不是空的，也要加载几何着色器
    static Shader
    LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile,
               std::string name);

    // Retrieves a stored shader
    // 检索存储的着色器程序对象
    static Shader GetShader(std::string name);

    // Loads (and generates) a texture from file
    // 从文件加载（并生成）纹理
    static Texture2D LoadTexture(const GLchar *file, GLboolean alpha, std::string name);

    // Retrieves a stored texture
    // 检索存储的纹理对象
    static Texture2D& GetTexture(std::string name);

    // Properly de-allocates all loaded resources
    // 回收所有加载的资源
    static void Clear();

private:
    // Private constructor, that is we do not want any actual resource manager objects.
    // Its members and functions should be publicly available (static).
    // 私有的构造函数，即我们不想要任何实际的资源管理器对象。
    // 它的成员和功能应该是公开的（静态的）。
    ResourceManager() {}

    // Loads and generates a shader from file
    // 从文件加载并生成着色器
    static Shader loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile,
                                     const GLchar *gShaderFile = nullptr);

    // Loads a single texture from file
    // 从文件加载单个纹理
    static Texture2D loadTextureFromFile(const GLchar *file, GLboolean alpha);

};


#endif //LEARNOPENGL_RESOURCEMANAGER_H
