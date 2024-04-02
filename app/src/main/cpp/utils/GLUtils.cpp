//
// Created by TS on 2024/2/28.
//

#include "GLUtils.h"
#include <cstdlib>

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
            LOGE("Couldn't load %s", path);
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
GLuint GLUtils::createProgram(const char **vertexSource, const char **fragmentSource) {
    //声明着色器程序id
    GLuint programId;

    FUN_BEGIN_TIME("GLUtils::createProgram")
        //加载顶点着色器代码并创建shader对象
        GLuint vertexShader = loadShader(GL_VERTEX_SHADER, vertexSource);
        if (vertexShader == 0) {
            return 0;
        }

        //加载片段着色器代码并创建shader对象
        GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentSource);
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

        //释放顶点着色器对象和片段着色器对象
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

    FUN_END_TIME("GLUtils::createProgram")
    return programId;
}

void GLUtils::checkGlError(const char *pGLOperation) {
    for (GLint error = glGetError(); error; error = glGetError()) {
        LOGE("GLUtils::CheckGLError after GL Operation %s() glError (0x%x)\n", pGLOperation, error)
    }
}