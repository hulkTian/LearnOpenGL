//
// Created by TS on 2024/2/28.
//

#include "GLUtils.h"
#include <cstdlib>

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

static JNIEnv *sEnv = nullptr;
static jobject sAssetManager = nullptr;

/**
 * 打开文件.
 * @param path
 * @return
 */
static AAsset *loadAsset(const char *path) {
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
        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        //编译不成功，打印错误信息
        if (!success) {
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
GLuint GLUtils::createProgram(const char *vertexPath, const char *fragmentPath) {
    //加载顶点着色器代码
    const char *vertexCode = openTextFile(vertexPath);

    //加载片段着色器代码
    const char *fragmentCode = openTextFile(fragmentPath);

    //声明着色器程序id
    GLuint programId;

    FUN_BEGIN_TIME("GLUtils::createProgram")
        //加载顶点着色器代码并创建shader对象
        GLuint vertexShader = loadShader(GL_VERTEX_SHADER, &vertexCode);
        if (vertexShader == 0) {
            return 0;
        }

        //加载片段着色器代码并创建shader对象
        GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, &fragmentCode);
        if (fragmentShader == 0) {
            return 0;
        }

        //创建着色器程序对象
        programId = glCreateProgram();
        if (programId == 0) {
            return 0;
        }

        //绑定顶点着色器对象到着色器程序
        glAttachShader(programId, vertexShader);
        checkGlError("glAttachShader");

        //绑定片段着色器对象到着色器程序
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
    for (GLint error = glGetError(); error; error = glGetError()) {
        LOGE("GLUtils::CheckGLError after GL Operation %s() glError (0x%x)\n", pGLOperation, error)
    }
}

//从图片中加载纹理
GLuint GLUtils::loadTgaTexture(const char *fileName, const bool flip,
                               unsigned int texture_warp_s, unsigned int texture_warp_t,
                               unsigned int texture_min_filter, unsigned int texture_max_filter) {
    GLuint textureId;
    FUN_BEGIN_TIME("GLUtils::loadTgaTexture")
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);

        // 打开assets中的文件
        AAsset *asset = loadAsset(fileName);

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
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
                     0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
        // 生成多级渐远纹理
        glGenerateMipmap(GL_TEXTURE_2D);
        // 为当前绑定的纹理对象设置环绕、过滤方式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture_warp_s);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture_warp_t);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture_min_filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture_max_filter);

        // 释放图片数据
        stbi_image_free(imageData);

    FUN_END_TIME("GLUtils::loadTgaTexture")
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
                                                             &channels, 0);

            free(assetBuffer);

            if (imageData == nullptr) {
                // 图片解码失败的处理逻辑
                LOGD("Image data is null")
                return 0;
            }

            // 为每个面加载纹理数据
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB,
                         GL_UNSIGNED_BYTE, imageData);
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
