//
// Created by TS on 2024/2/28.
//

#include "GLUtils.h"
#include <cstdlib>

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

/**
     * 设置并配置VAO（顶点数组对象）和VBO（顶点缓冲对象），如果有索引数据（通过indices传入），也会配置EBO（元素缓冲对象）。
     * 函数接收顶点数据、索引数据（可选，若不需要传nullptr）以及用于描述顶点属性布局的指针数组（pointer）。
     * @param vertices 指向顶点数据的指针，数据按顺序排列，每个顶点包含多个属性，具体由pointer描述。
     * @param indices 指向索引数据的指针（可选，若不需要索引则传入nullptr），用于指定绘制顶点的顺序。
     * @param pointer 一个整数数组指针，用于指定每个属性在单个顶点数据中的组件数量（如位置属性可能是3个组件等），以确定顶点属性布局。
     * @return 创建并配置好的VAO的标识符（GLuint类型），供后续渲染操作使用。
     */
GLuint GLUtils::setUpVAOAndVBO(const float *vertices, const GLsizeiptr vSize, const GLuint *indices,
                               const GLsizeiptr iSize, const int *pointer) {
    GLuint VAO, VBO;

    // 创建并绑定VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // 创建并绑定VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // 正确计算顶点数据的字节大小并复制到VBO中
    // 通过将指针类型转换为size_t并相减来获取顶点数量，再乘以单个顶点占用字节数来计算总字节数
    glBufferData(GL_ARRAY_BUFFER, vSize, vertices, GL_STATIC_DRAW);

    if (indices != nullptr) {
        GLuint EBO;
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        // 正确计算索引数据的字节大小并复制到EBO中
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, iSize, indices, GL_STATIC_DRAW);
    }

    // 配置顶点属性指针，循环处理每个属性
    int numAttributes = 0;
    GLsizei stride = 0;
    while (pointer[numAttributes] != 0) {
        stride += pointer[numAttributes] * sizeof(float);
        numAttributes++;
    }

    int offset = 0;
    for (int i = 0; i < numAttributes; ++i) {
        if (i > 0) {
            offset += sizeof(float) * pointer[i - 1];
        }
        glVertexAttribPointer(i, pointer[i], GL_FLOAT, GL_FALSE, stride,
                              reinterpret_cast<const void *>(offset));
        glEnableVertexAttribArray(i);
    }

    // 解绑VAO，恢复默认状态
    glBindVertexArray(0);
    return VAO;
}

/**
 * 打开文件.
 * @param path
 * @return
 */
AAsset *GLUtils::loadAsset(const char *path) {
    //将Java层的AssetManager对象转换为native层的对象
    AAssetManager *aAssetManager = AAssetManager_fromJava(sEnv, sAssetManager);
    if (aAssetManager == nullptr) {
        return nullptr;
    }
    //打开path对应文件
    return AAssetManager_open(aAssetManager, path, AASSET_MODE_UNKNOWN);
}

/**
 * 编译着色器代码
 * @param shaderType 着色器类型
 * @param source 着色器代码字符串
 * @return 着色器代码id
 */
static GLuint loadShader(GLenum shaderType, const char **source) {
    //着色器对象id
    GLuint shader;
    FUN_BEGIN_TIME("GLUnit::loadShader")
        //创建着色器对象，需要指定是顶点着色器还是片段着色器
        shader = glCreateShader(shaderType);
        if (shader == 0) {
            LOGE("Could not create shader")
            return 0;
        }

        //加载着色器代码
        //glShaderSource函数把要编译的着色器对象作为第一个参数。
        //第二参数指定了传递的源码字符串数量，这里只有一个。
        //第三个参数是顶点着色器真正的源码，第四个参数我们先设置为NULL。
        glShaderSource(shader, 1, source, nullptr);

        //编译着色器代码
        glCompileShader(shader);

        //检查着色器编译状态，获取编译状态
        GLint status;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

        //编译不成功，打印错误信息
        if (status != GL_TRUE) {
            GLint infoLen = 0;
            //获取错误信息的长度
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen > 1) {
                char *infoLog = (char *) malloc(sizeof(char) * infoLen);
                glGetShaderInfoLog(shader, infoLen, nullptr, infoLog);
                LOGE("GLUtils::loadShader error compiling shader:\n%s\n", infoLog)
                free(infoLog);
            }
            //释放Shader对象
            glDeleteShader(shader);
            return 0;
        }
    FUN_END_TIME("GLUnit::loadShader")
    return shader;
}

void GLUtils::setEnvAndAssetManager(JNIEnv *env, jobject assetManager) {
    sEnv = env;
    sAssetManager = assetManager;
}

/**
 * 将字节流文件转为字符串.
 * @param path 文件地址
 * @return
 */
char *GLUtils::openTextFile(const char *path) {
    char *buffer;
    FUN_BEGIN_TIME("GLUtils::openTextFile")
        LOGD("GLUtils::openTextFile path [%s]", path)
        AAsset *asset = loadAsset(path);
        if (asset == nullptr) {
            LOGE("Couldn't load %s", path)
            return nullptr;
        }
        off_t length = AAsset_getLength(asset);
        buffer = new char[length + 1];
        int num = AAsset_read(asset, buffer, length);
        AAsset_close(asset);
        if (num != length) {
            LOGE("Couldn't load %s", path)
            delete[] buffer;
            return nullptr;
        }
        //拼接结束符
        buffer[length] = '\0';
    FUN_END_TIME("GLUtils::openTextFile")
    return buffer;
}

/**
 * 创建着色器程序
 * @param vertexSource 顶点着色器代码字符串
 * @param fragmentSource 片段着色器代码字符串
 * @return 着色器程序id
 */
GLuint
GLUtils::createProgram(const char *vertexPath, const char *fragmentPath, const char *geometryPath) {
    //加载顶点着色器代码
    const char *vertexCode = openTextFile(vertexPath);

    //加载片段着色器代码
    const char *fragmentCode = openTextFile(fragmentPath);

    //加载几何着色器代码
    const char *geometryCode = nullptr;
    if (geometryPath) {
        geometryCode = openTextFile(geometryPath);
    } else {
        LOGE("geometry shader path is null")
    }


    //声明着色器程序id
    GLuint programId;

    FUN_BEGIN_TIME("GLUtils::createProgram")
        //加载顶点着色器代码并创建shader对象
        GLuint vertexShader = loadShader(GL_VERTEX_SHADER, &vertexCode);
        if (vertexShader == 0) {
            return 0;
        }

        //加载几何着色器代码并创建shader对象
        GLuint geometryShader = 0;
        if (geometryCode) {
            geometryShader = loadShader(GL_GEOMETRY_SHADER, &geometryCode);
            if (geometryShader == 0) {
                return 0;
            }
        } else {
            LOGE("geometry shader code is null")
        }

        //加载片段着色器代码并创建shader对象
        GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, &fragmentCode);
        if (fragmentShader == 0) {
            LOGE("fragment shader is null")
            return 0;
        }

        //创建着色器程序对象
        programId = glCreateProgram();
        if (programId == 0) {
            LOGE("Could not create program")
            return 0;
        }

        //绑定顶点着色器对象到着色器程序
        LOGI("attach vertex shader to program")
        glAttachShader(programId, vertexShader);
        checkGlError("glAttachShader");

        //绑定几何着色器对象到着色器程序
        if (geometryShader != 0) {
            LOGI("attach geometry shader to program")
            glAttachShader(programId, geometryShader);
            checkGlError("glAttachShader");
        } else {
            LOGE("geometry shader object is null")
        }

        //绑定片元着色器对象到着色器程序
        LOGI("attach fragment shader to program")
        glAttachShader(programId, fragmentShader);
        checkGlError("glAttachShader");

        //链接着色器程序
        glLinkProgram(programId);

        //检查链接状态
        GLint linkStatus;
        glGetProgramiv(programId, GL_LINK_STATUS, &linkStatus);

        //释放顶点着色器对象和片段着色器对象
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        if (vertexCode != nullptr) {
            delete[] vertexCode;
            vertexCode = nullptr;
        }
        if (fragmentCode != nullptr) {
            delete[] fragmentCode;
            fragmentCode = nullptr;
        }

        if (!linkStatus) {
            GLint infoLen = 0;
            glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen > 1) {
                char *infoLog = (char *) malloc(sizeof(char) * infoLen);
                glGetProgramInfoLog(programId, infoLen, nullptr, infoLog);
                LOGE("GLUtils::createProgram error linking program:\n%s\n", infoLog)
                free(infoLog);
            }

            //释放着色器程序对象
            glDeleteProgram(programId);
            return 0;
        }

    FUN_END_TIME("GLUtils::createProgram")
    return programId;
}

void GLUtils::checkGlError(const char *pGLOperation) {
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR) {
        std::string error;
        switch (errorCode) {
            case GL_INVALID_ENUM:
                error = "GL_INVALID_ENUM";
                break;
            case GL_INVALID_VALUE:
                error = "GL_INVALID_VALUE";
                break;
            case GL_INVALID_OPERATION:
                error = "GL_INVALID_OPERATION";
                break;
            case GL_STACK_OVERFLOW:
                error = "GL_STACK_OVERFLOW";
                break;
            case GL_STACK_UNDERFLOW:
                error = "GL_STACK_UNDERFLOW";
                break;
            case GL_OUT_OF_MEMORY:
                error = "GL_OUT_OF_MEMORY";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                error = "GL_INVALID_FRAMEBUFFER_OPERATION";
                break;
        }
        LOGE("GLUtils::CheckGLError after GL Operation %s() : %s\n", pGLOperation, error.c_str())
    }
}

//从图片中加载纹理
GLuint GLUtils::loadTgaTexture(const char *fileName, GLint internalformat, GLenum format,
                               GLenum type, const bool flip,
                               unsigned int texture_warp_s, unsigned int texture_warp_t,
                               unsigned int texture_min_filter, unsigned int texture_max_filter) {
    GLuint textureId;
    FUN_BEGIN_TIME("GLUtils::loadTgaTexture")
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        // 打开assets中的文件
        AAsset *asset = loadAsset(fileName);
        if (asset == nullptr) {
            LOGE("Couldn't load %s", fileName)
            return textureId;
        }
        // 获取文件大小
        off_t assetLength = AAsset_getLength(asset);
        void *assetBuffer = malloc(assetLength);
        AAsset_read(asset, assetBuffer, assetLength);
        AAsset_close(asset);
        // 翻转y轴，使纹理坐标从底部开始
        stbi_set_flip_vertically_on_load(flip);

        // 使用stb_image解码图片数据
        int width, height, channels;
        unsigned char *imageData = stbi_load_from_memory(static_cast<stbi_uc *>(assetBuffer),
                                                         static_cast<int>(assetLength),
                                                         &width, &height,
                                                         &channels, 4);

        free(assetBuffer);

        if (imageData == nullptr) {
            // 图片解码失败的处理逻辑
            LOGD("Image data is null")
            return 0;
        }

        //加载纹理数据
        glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, format, type, imageData);
        // 生成多级渐远纹理
        glGenerateMipmap(GL_TEXTURE_2D);
        // 为当前绑定的纹理对象设置环绕、过滤方式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture_min_filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture_max_filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture_warp_s);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture_warp_t);

        // 当配置环绕方式为GL_CLAMP_TO_BORDER时，需要设置边框颜色
        if (texture_warp_s == GL_CLAMP_TO_BORDER || texture_warp_t == GL_CLAMP_TO_BORDER) {
            GLfloat borderColor[] = {1.0f, 1.0f, 0.0f, 1.0f};
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        }

        // 释放图片数据
        stbi_image_free(imageData);

    FUN_END_TIME("GLUtils::loadTgaTexture")
    return textureId;
}

//从图片中加载纹理
GLuint GLUtils::loadHDRTexture(const char *fileName) {
    GLuint textureId;
    FUN_BEGIN_TIME("GLUtils::loadHDRTexture")
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);

        // 打开assets中的文件
        AAsset *asset = loadAsset(fileName);
        if (asset == nullptr) {
            LOGE("Couldn't load %s", fileName)
            return textureId;
        }

        // 获取文件大小
        off_t assetLength = AAsset_getLength(asset);
        void *assetBuffer = malloc(assetLength);
        AAsset_read(asset, assetBuffer, assetLength);
        AAsset_close(asset);

        // 翻转y轴，使纹理坐标从底部开始
        stbi_set_flip_vertically_on_load(true);

        // 使用stb_image解码图片数据
        int width, height, channels;
        float *imageData = stbi_loadf_from_memory(static_cast<stbi_uc *>(assetBuffer),
                                                  static_cast<int>(assetLength),
                                                  &width, &height,
                                                  &channels, 0);
        free(assetBuffer);

        if (imageData == nullptr) {
            // 图片解码失败的处理逻辑
            LOGD("Image data is null")
            return 0;
        }

        //加载纹理数据
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, imageData);
        // 生成多级渐远纹理
        glGenerateMipmap(GL_TEXTURE_2D);
        // 为当前绑定的纹理对象设置环绕、过滤方式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // 释放图片数据
        stbi_image_free(imageData);

    FUN_END_TIME("GLUtils::loadHDRTexture")
    return textureId;
}

/**
 * 加载立方体盒子贴图纹理
 */
GLuint GLUtils::loadCubemap(const std::vector<std::string> faces, const bool flip) {
    GLuint textureId;
    FUN_BEGIN_TIME("GLUtils::loadCubemap")
        glGenTextures(1, &textureId);
        // 绑定立方体贴图纹理目标
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

        for (unsigned int i = 0; i < faces.size(); i++) {

            // 打开assets中的文件
            AAsset *asset = loadAsset(faces[i].c_str());
            if (asset == nullptr) {
                LOGE("Could not open %s", faces[i].c_str())
                return textureId;
            }

            // 获取文件大小
            off_t assetLength = AAsset_getLength(asset);
            void *assetBuffer = malloc(assetLength);
            int num = AAsset_read(asset, assetBuffer, assetLength);
            AAsset_close(asset);
            if (num != assetLength) {
                LOGE("Couldn't load %s", faces[i].c_str())
                free(assetBuffer);
                return textureId;
            }

            // 翻转y轴，使纹理坐标从底部开始
            stbi_set_flip_vertically_on_load(flip);

            // 使用stb_image解码图片数据
            int width, height, channels;
            unsigned char *imageData = stbi_load_from_memory(static_cast<stbi_uc *>(assetBuffer),
                                                             static_cast<int>(assetLength),
                                                             &width, &height,
                                                             &channels, 0);

            free(assetBuffer);

            if (imageData == nullptr) {
                // 图片解码失败的处理逻辑
                LOGD("Image data is null")
                return 0;
            }

            // 为每个面加载纹理数据
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
                         width, height, 0, GL_RGB,GL_UNSIGNED_BYTE, imageData);
            // 为当前绑定的纹理对象设置环绕、过滤方式
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            // 释放图片数据
            stbi_image_free(imageData);
        }
    FUN_END_TIME("GLUtils::loadCubemap")
    return textureId;
}

/**
 * 加载纹理图片
 * @param path 图片路径
 * @param gammaCorrection 纹理是否需要从非线性颜色空间转到线性颜色空间
 * @return
 */
GLuint GLUtils::loadTexture(const char *path, bool gammaCorrection) {
    GLuint textureID;
    FUN_BEGIN_TIME("GLUtils::loadTexture")
        glGenTextures(1, &textureID);

        // 打开assets中的文件
        AAsset *asset = loadAsset(path);
        if (asset == nullptr) {
            LOGE("Could not open %s", path)
            return textureID;
        }

        // 获取文件大小
        off_t assetLength = AAsset_getLength(asset);
        void *assetBuffer = malloc(assetLength);
        int num = AAsset_read(asset, assetBuffer, assetLength);
        AAsset_close(asset);
        if (num != assetLength) {
            LOGE("Couldn't load %s", path)
            free(assetBuffer);
            return textureID;
        }

        int width, height, nrComponents;
        unsigned char *data = stbi_load_from_memory(static_cast<stbi_uc *>(assetBuffer),
                                                    static_cast<int>(assetLength),
                                                    &width, &height,
                                                    &nrComponents, 0);
        if (data) {
            GLenum internalFormat;
            GLenum dataFormat;
            if (nrComponents == 1) {
                internalFormat = dataFormat = GL_RED;
            } else if (nrComponents == 3) {
                internalFormat = gammaCorrection ? GL_SRGB8 : GL_RGB;
                dataFormat = GL_RGB;
            } else if (nrComponents == 4) {
                internalFormat = gammaCorrection ? GL_SRGB8_ALPHA8 : GL_RGBA;
                dataFormat = GL_RGBA;
            }

            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat,
                         GL_UNSIGNED_BYTE, data);
            // todo 需要判断设备是否支持 GL_SRGB8纹理生成多级渐远纹理
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        } else {
            LOGE("Texture failed to load at path: %s", path)
            stbi_image_free(data);
        }
    FUN_END_TIME("GLUtils::loadTexture")
    return textureID;
}