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
    /** 1.声明着色器对象 */
    GLuint shader;
    FUN_BEGIN_TIME("GLUnit::loadShader")
        /** 2.创建着色器对象，需要指定是顶点着色器还是片段着色器 */
        shader = glCreateShader(shaderType);
        if (shader == 0) {
            LOGE("Could not create shader")
            return 0;
        }

        /** 3.加载着色器代码
        * 第一个参数指定编译的着色器对象句柄，GPU会把加载的代码存储在这个对象中
        * 第二个参数指定传递的源码字符串数量，这里只有一个
        * 第三个参数指定顶点着色器真正的源码
        * 第四个参数传入int数组，指定每段源码字符串的长度，这里只有一个GPU可以自己算长度，所以传入nullptr
        */
        glShaderSource(shader, 1, source, nullptr);
        /**4.编译着色器代码 */
        glCompileShader(shader);

        /**5.检查着色器编译状态，获取编译状态 */
        GLint status;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

        /**6.编译不成功，打印错误信息 */
        if (status != GL_TRUE) {
            GLint infoLen = 0;
            /**7.获取错误信息的长度 */  
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen > 1) {
                //获取错误信息
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
    /** 1.加载顶点着色器代码 */
    const char *vertexCode = openTextFile(vertexPath);

    /** 2.加载片段着色器代码 */
    const char *fragmentCode = openTextFile(fragmentPath);

    /** 3.加载几何着色器代码 */
    const char *geometryCode = nullptr;
    if (geometryPath) {
        geometryCode = openTextFile(geometryPath);
    } else {
        LOGE("geometry shader path is null")
    }


    /** 4.声明着色器程序id */
    GLuint programId;

    FUN_BEGIN_TIME("GLUtils::createProgram")
        /** 4.1.编译顶点着色器代码 */
        GLuint vertexShader = loadShader(GL_VERTEX_SHADER, &vertexCode);
        if (vertexShader == 0) {
            if (vertexCode != nullptr) {
                delete[] vertexCode;
                vertexCode = nullptr;
            }
            return 0;
        }

        /** 4.2.编译几何着色器代码 */
        GLuint geometryShader = 0;
        if (geometryCode) {
            geometryShader = loadShader(GL_GEOMETRY_SHADER, &geometryCode);
            if (geometryShader == 0) {
                if (geometryCode != nullptr) {
                    delete[] geometryCode;
                    geometryCode = nullptr;
                }
                return 0;
            }
        } else {
            LOGE("geometry shader code is null")
        }

        /** 4.3.编译片段着色器代码 */
        GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, &fragmentCode);
        if (fragmentShader == 0) {
            LOGE("fragment shader is null")
            if (fragmentCode != nullptr) {
                delete[] fragmentCode;
                fragmentCode = nullptr;
            }
            return 0;
        }

        /** 4.4.创建着色器程序对象 */
        programId = glCreateProgram();
        if (programId == 0) {
            LOGE("Could not create program")
            return 0;
        }

        /** 4.5.在program的attached_shaders列表中添加顶点着色器对象的句柄并增加对顶点着色器对象的引用计数，还要验证shader类型是否合法 */
        LOGI("attach vertex shader to program")
        glAttachShader(programId, vertexShader);
        checkGlError("glAttachShader");

        /** 4.6.在program的attached_shaders列表中添加几何着色器对象的句柄并增加对几何着色器对象的引用计数，还要验证shader类型是否合法 */
        if (geometryShader != 0) {
            LOGI("attach geometry shader to program")
            glAttachShader(programId, geometryShader);
            checkGlError("glAttachShader");
        } else {
            LOGE("geometry shader object is null")
        }

        /** 4.7.在program的attached_shaders列表中添加片段着色器对象的句柄并增加对片段着色器对象的引用计数，还要验证shader类型是否合法 */
        LOGI("attach fragment shader to program")
        glAttachShader(programId, fragmentShader);
        checkGlError("glAttachShader");

        /** 4.8.链接着色器程序
         * 1.检查所有着色器代码中的输入和输出变量是否匹配
         * 2.分配uniform/attribute变量的location
         * 3.验证并生成着色器程序的二进制代码
         */
        glLinkProgram(programId);

        /** 4.9.检查链接状态 */
        GLint linkStatus;
        glGetProgramiv(programId, GL_LINK_STATUS, &linkStatus);

        /** 4.10.释放顶点着色器对象和片段着色器对象 */
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

        /** 4.11.检查链接状态 */
        if (!linkStatus) {
            /** 4.11.1.获取着色器程序链接日志 */
            GLint infoLen = 0;
            glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen > 1) {
                char *infoLog = (char *) malloc(sizeof(char) * infoLen);
                glGetProgramInfoLog(programId, infoLen, nullptr, infoLog);
                LOGE("GLUtils::createProgram error linking program:\n%s\n", infoLog)
                free(infoLog);
            }

            /** 4.11.2.释放着色器程序对象 */
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

/**
 * 从 assets 读取文件到内存缓冲区（辅助函数，供 decodeAsset/decodeAssetHDR 复用）
 */
void *GLUtils::readAssetBuffer(const char *fileName, bool readCheck, off_t *outLength) {
    AAsset *asset = loadAsset(fileName);
    if (asset == nullptr) {
        LOGE("Couldn't load %s", fileName)
        return nullptr;
    }

    off_t assetLength = AAsset_getLength(asset);
    void *assetBuffer = malloc(assetLength);
    if (readCheck) {
        int num = AAsset_read(asset, assetBuffer, assetLength);
        AAsset_close(asset);
        if (num != assetLength) {
            LOGE("Couldn't fully read %s", fileName)
            free(assetBuffer);
            return nullptr;
        }
    } else {
        AAsset_read(asset, assetBuffer, assetLength);
        AAsset_close(asset);
    }

    if (outLength) {
        *outLength = assetLength;
    }
    return assetBuffer;
}

/**
 * 从 assets 读取文件并解码为 8bit LDR 图像数据（纯 CPU 侧，不创建纹理对象）
 */
unsigned char *GLUtils::decodeAsset(const char *fileName, bool flip,
                                    int *width, int *height, int *channels,
                                    int forceChannels, bool readCheck) {
    off_t assetLength;
    void *assetBuffer = readAssetBuffer(fileName, readCheck, &assetLength);
    if (assetBuffer == nullptr) {
        return nullptr;
    }

    stbi_set_flip_vertically_on_load(flip);

    unsigned char *imageData = stbi_load_from_memory(static_cast<stbi_uc *>(assetBuffer),
                                                     static_cast<int>(assetLength),
                                                     width, height, channels, forceChannels);
    free(assetBuffer);

    if (imageData == nullptr) {
        LOGD("Image decode failed for %s", fileName)
    }
    return imageData;
}

/**
 * 从 assets 读取文件并解码为 float HDR 图像数据（纯 CPU 侧，不创建纹理对象）
 */
float *GLUtils::decodeAssetHDR(const char *fileName, bool flip,
                               int *width, int *height, int *channels,
                               int forceChannels, bool readCheck) {
    off_t assetLength;
    void *assetBuffer = readAssetBuffer(fileName, readCheck, &assetLength);
    if (assetBuffer == nullptr) {
        return nullptr;
    }

    stbi_set_flip_vertically_on_load(flip);

    float *imageData = stbi_loadf_from_memory(static_cast<stbi_uc *>(assetBuffer),
                                              static_cast<int>(assetLength),
                                              width, height, channels, forceChannels);
    free(assetBuffer);

    if (imageData == nullptr) {
        LOGD("HDR image decode failed for %s", fileName)
    }
    return imageData;
}

/**
 * 通用 2D 纹理加载（核心入口） \n
 * internalFormat == 0：按解码通道数自动选格式（配合 srgb），解码保留原始通道 \n
 * internalFormat != 0：按显式格式上传，解码强制转 4 通道 RGBA
 */
GLuint GLUtils::loadTexture2D(const char *fileName, const TextureDesc &desc) {
    GLuint textureId;
    FUN_BEGIN_TIME("GLUtils::loadTexture2D")
        /** 1.生成纹理对象 */
        glGenTextures(1, &textureId);
        /** 2.绑定纹理对象，告诉 OpenGL 当前操作的是哪个纹理对象 */
        glBindTexture(GL_TEXTURE_2D, textureId);

        // 自动模式保留原始通道数（识别 GL_RED/GL_RGB/GL_RGBA），显式模式强制 4 通道
        int forceChannels = (desc.internalFormat == 0) ? 0 : 4;
        int width, height, channels;
        /** 3.解码图像数据 */
        unsigned char *imageData = decodeAsset(fileName, desc.flip, &width, &height,
                                               &channels, forceChannels);
        if (imageData == nullptr) {
            return 0;
        }

        // 确定上传格式（internalformat + format）
        GLenum internalFormat;
        GLenum dataFormat;
        if (desc.internalFormat == 0) {
            switch (channels) {
                case 1: internalFormat = dataFormat = GL_RED; break;
                case 3: internalFormat = desc.srgb ? GL_SRGB : GL_RGB;
                        dataFormat = GL_RGB; break;
                case 4: internalFormat = desc.srgb ? GL_SRGB8_ALPHA8 : GL_RGBA;
                        dataFormat = GL_RGBA; break;
                default: internalFormat = dataFormat = GL_RGBA; break;
            }
        } else {
            internalFormat = dataFormat = desc.internalFormat;
        }
        /**
         * 4.上传纹理数据，把解码后的图像数据上传纹理对象textureId \n
         * GL_TEXTURE_2D：2D 纹理目标 \n
         * 0：mipmap 层级（0 表示基础纹理） \n
         * internalFormat：纹理格式（GL_RGB、GL_RGBA 等） \n
         * width：纹理宽度 \n
         * height：纹理高度 \n
         * 0：边框宽度（0 表示无边框） \n
         * dataFormat：数据格式（GL_RGB、GL_RGBA 等） \n
         * GL_UNSIGNED_BYTE：数据类型（8 无符号字节） \n
         * imageData：图像数据指针
         */
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0,
                     dataFormat, GL_UNSIGNED_BYTE, imageData);
        checkGlError("glTexImage2D");
        /**
         * 5.生成 mipmap（如果配置了） \n
         * mipmap：多级纹理（mipmap）：用于在不同距离下渲染时保持细节和性能的纹理技术 \n
         * 生成 mipmap 后，纹理会自动根据距离和缩放级别，选择合适的 mipmap 层级进行渲染 \n
         * 一个关键约束：minFilter 必须配 MIPMAP 模式才生效，所以这里加了判断 \n
         */
        if (desc.mipmap && desc.minFilter != GL_NEAREST && desc.minFilter != GL_LINEAR) {
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        /**
         * 6.配置缩小过滤模式（命名规律：GL_[层内]_MIPMAP_[层间]，MIPMAP 前=层内筛选，后=层间筛选） \n
         * GL_NEAREST：最近邻过滤（无 mipmap），像素感，缩小时闪烁 \n
         * GL_LINEAR：线性过滤（无 mipmap），放大时软边但缩小时闪烁 \n
         * GL_LINEAR_MIPMAP_LINEAR：层内线性 + 层间线性（三线性过滤），质量最好，远处过渡自然 \n
         * GL_LINEAR_MIPMAP_NEAREST：层内线性 + 层间最近邻，远处会出现 mip 层硬切换 \n
         * GL_NEAREST_MIPMAP_LINEAR：层内最近邻 + 层间线性，像素感但远处过渡平滑 \n
         * GL_NEAREST_MIPMAP_NEAREST：层内最近邻 + 层间最近邻，像素感 + mip 硬切换 \n
         * */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, desc.minFilter);
        /**
         * 7.配置放大过滤模式
         * */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, desc.magFilter);
        /**
         * 8.配置水平方向环绕方式
         * GL_REPEAT：重复环绕（默认）-0.3->0.7 1.3->0.3 \n
         * GL_MIRRORED_REPEAT：镜像重复环绕 -0.3->0.3 1.3->0.7\n
         * GL_CLAMP_TO_EDGE：超出范围返回纹理边缘 -0.3->0.0 1.3->1.0\n
         * GL_CLAMP_TO_BORDER：超出范围返回指定颜色 -0.3->边框颜色 1.3->边框颜色 \n
         * GL_MIRROR_CLAMP_TO_EDGE：先镜像到最近整数，再 clamp -0.3->0.3 1.3->1.0 \n
         * */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, desc.wrapS);
        /**
         * 9.配置垂直方向环绕方式
         * GL_REPEAT：重复环绕（默认）-0.3->0.7 1.3->0.3 \n
         * GL_MIRRORED_REPEAT：镜像重复环绕 -0.3->0.3 1.3->0.7\n
         * GL_CLAMP_TO_EDGE：超出范围返回纹理边缘 -0.3->0.0 1.3->1.0\n
         * GL_CLAMP_TO_BORDER：超出范围返回指定颜色 -0.3->边框颜色 1.3->边框颜色 \n
         * GL_MIRROR_CLAMP_TO_EDGE：先镜像到最近整数，再 clamp -0.3->0.3 1.3->1.0 \n
         * */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, desc.wrapT);

        // 当配置环绕方式为GL_CLAMP_TO_BORDER时，需要设置边框颜色
        if (desc.wrapS == GL_CLAMP_TO_BORDER || desc.wrapT == GL_CLAMP_TO_BORDER) {
            GLfloat borderColor[] = {1.0f, 1.0f, 0.0f, 1.0f};
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        }

        // 释放图片数据
        stbi_image_free(imageData);

    FUN_END_TIME("GLUtils::loadTexture2D")
    return textureId;
}

//显式格式路线：未指定格式时默认 GL_RGBA
GLuint GLUtils::loadTgaTexture(const char *fileName, const TextureDesc &desc) {
    TextureDesc d = desc;
    if (d.internalFormat == 0) {
        d.internalFormat = GL_RGBA;
    }
    return loadTexture2D(fileName, d);
}

//自动格式路线：按通道数推导格式，REPEAT + 线性过滤
GLuint GLUtils::loadTexture(const char *fileName, bool gammaCorrection) {
    TextureDesc desc;
    desc.srgb = gammaCorrection;
    desc.flip = false;
    desc.wrapS = desc.wrapT = GL_REPEAT;
    if (gammaCorrection) {
        // sRGB 纹理不生成 mipmap，只用线性过滤
        desc.mipmap = false;
        desc.minFilter = desc.magFilter = GL_LINEAR;
    } else {
        desc.minFilter = GL_LINEAR_MIPMAP_LINEAR;
        desc.magFilter = GL_LINEAR;
    }
    return loadTexture2D(fileName, desc);
}

//从图片中加载纹理
GLuint GLUtils::loadHDRTexture(const char *fileName) {
    GLuint textureId;
    FUN_BEGIN_TIME("GLUtils::loadHDRTexture")
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);

        // 读取并解码 HDR 图片数据（独立函数，纯 CPU 侧操作；HDR 垂直翻转是必需的）
        int width, height, channels;
        float *imageData = decodeAssetHDR(fileName, /*flip=*/true, &width, &height, &channels);
        if (imageData == nullptr) {
            return 0;
        }

        //加载纹理数据
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0,
                     GL_RGB, GL_FLOAT, imageData);
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

            // 读取并解码图片数据（独立函数，纯 CPU 侧操作）
            int width, height, channels;
            unsigned char *imageData = decodeAsset(faces[i].c_str(), flip,
                                                   &width, &height, &channels,
                                                   /*forceChannels=*/0, /*readCheck=*/true);
            if (imageData == nullptr) {
                return textureId;
            }

            // 为每个面加载纹理数据
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
                         width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
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

bool GLUtils::isSupportedForTexture(GLenum internalformat) {
    GLint supported = 0;
    glGetInternalformativ(GL_TEXTURE_2D, internalformat, GL_TEXTURE_INTERNAL_FORMAT,
                          1, &supported);
    if (supported != GL_TRUE) {
        LOGE("Device does not support %d internal format for textures.",
             supported)
    }
    return supported == GL_TRUE;
}
