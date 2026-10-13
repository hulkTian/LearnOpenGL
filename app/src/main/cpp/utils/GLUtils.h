//
// Created by TS on 2024/2/28.
//

#ifndef LEARNOPENGL_GLUTILS_H
#define LEARNOPENGL_GLUTILS_H

#define KEY_W 1
#define KEY_S 2
#define KEY_A 3
#define KEY_D 4
#define KEY_B 5

#include <jni.h>
#include <GLES3/gl32.h>
#include <GLES3/gl3ext.h>
#include <LogUtils.h>
#include <TimeUtils.h>
#include <string>
#include <vector>
#include <android/asset_manager_jni.h>
#include <glm/detail/type_mat4x4.hpp>

#define MATH_PI 3.1415926535897932384626433832802

static JNIEnv *sEnv;
static jobject sAssetManager;

/**
 * 纹理加载参数描述（配合 loadTexture2D / loadTgaTexture / loadTexture 使用）
 * 所有字段带默认值，调用方只设置需要覆盖的项
 */
struct TextureDesc {
    GLint internalFormat = 0;            // 0 = 按图片通道数自动选格式；非 0 = 显式指定（如 GL_RGBA）
    bool  srgb       = false;            // 自动模式下是否选 sRGB 变体（gamma 校正）
    bool  flip       = true;             // 是否垂直翻转
    bool  mipmap     = true;             // 是否生成 mipmap
    GLenum wrapS     = GL_CLAMP_TO_EDGE; // S 环绕方式
    GLenum wrapT     = GL_CLAMP_TO_EDGE; // T 环绕方式
    GLenum minFilter = GL_NEAREST;       // 缩小过滤
    GLenum magFilter = GL_LINEAR;        // 放大过滤
};

class GLUtils {

public:
    /**
	 * Set Environment parameter
	 */
    static void setEnvAndAssetManager(JNIEnv *env, jobject assetManager);

    static AAsset *loadAsset(const char *path);

    /**
     *  Loads a file from assets/path into a char array.
     */
    static char *openTextFile(const char *path);

    /**
     * 通用 2D 纹理加载（核心入口）
     * desc.internalFormat == 0：按图片实际通道数自动选格式（1→GL_RED，3→GL_RGB/GL_SRGB，4→GL_RGBA/GL_SRGB8_ALPHA8）
     * desc.internalFormat != 0：按显式格式上传（解码强制转 4 通道 RGBA）
     */
    static GLuint loadTexture2D(const char *fileName, const TextureDesc &desc = {});

    /**
     * 自动格式路线的便捷封装（原 loadTexture 行为）：
     * 不翻转、REPEAT 环绕、线性过滤；sRGB 不生成 mipmap，否则生成并使用 LINEAR_MIPMAP_LINEAR
     */
    static GLuint loadTexture(char const *path, bool gammaCorrection);

    static bool isSupportedForTexture(GLenum internalformat);

    /**
     * 从 assets 读取文件并解码为 8bit LDR 图像数据（纯 CPU 侧，不创建纹理对象）
     * @param fileName  assets 路径
     * @param flip      是否垂直翻转
     * @param width/height/channels 输出参数，返回解码后的图像信息
     * @param forceChannels 强制输出通道数：0=保留原始，1/3/4=强制指定（loadTgaTexture 需要 4）
     * @param readCheck 是否校验实际读取字节数（loadCubemap/loadTexture 需要，性能敏感场景可关）
     * @return 解码后的图像数据，失败返回 nullptr；调用方负责用 stbi_image_free 释放
     */
    static unsigned char *decodeAsset(const char *fileName, bool flip,
                                      int *width, int *height, int *channels,
                                      int forceChannels = 0, bool readCheck = true);

    /**
     * 从 assets 读取文件并解码为 float HDR 图像数据（纯 CPU 侧，不创建纹理对象）
     * 参数同 decodeAsset，解码器为 stbi_loadf_from_memory
     */
    static float *decodeAssetHDR(const char *fileName, bool flip,
                                 int *width, int *height, int *channels,
                                 int forceChannels = 0, bool readCheck = true);

    /**
     * 从 assets 读取文件到内存缓冲区（辅助函数，供 decodeAsset/decodeAssetHDR 复用）
     * @param outLength 输出参数，返回文件字节数
     * @return 分配好的缓冲区，调用方负责 free；失败返回 nullptr
     */
    static void *readAssetBuffer(const char *fileName, bool readCheck, off_t *outLength);

    /**
     * 显式格式路线的便捷封装（原 loadTgaTexture 行为）：
     * internalFormat 未指定时默认 GL_RGBA（解码强制转 4 通道），其余行为同 loadTexture2D
     */
    static GLuint loadTgaTexture(const char *fileName, const TextureDesc &desc = {});

    static GLuint loadHDRTexture(const char *fileName);

    static GLuint setUpVAOAndVBO(const float *vertices, const GLsizeiptr vSize,
                                 const GLuint *indices, const GLsizeiptr iSize, const int *pointer);

    /**
     * 加载立方体贴图
     * @param faces 纹理地址
     * @param flip 是否翻转
     * @return cube map 纹理id
     */
    static GLuint loadCubemap(const std::vector<std::string> faces, const bool flip);

    /**
	 * Create a program with the given vertex and framgent
	 * shader source code.
	 */
    static GLuint createProgram(const char *vertexPath, const char *fragmentPath, const char *geometryPath = nullptr);

    static void checkGlError(const char *pGLOperation);

    static void printGLString(const char *name, GLenum s) {
        const char *v = (const char *) glGetString(s);
        LOGI("GL %s = %s \n", name, v)
    }

    // Print some OpenGL info
    static void printGLInfo() {
        LOGI("===== Here are some information of your device =====")
        printGLString("Version", GL_VERSION);
        printGLString("GLSL Version", GL_SHADING_LANGUAGE_VERSION);
        printGLString("Vendor", GL_VENDOR);
        printGLString("Renderer", GL_RENDERER);
        printGLString("Extensions", GL_EXTENSIONS);

        GLint iParam[2];
        glGetIntegerv(GL_MAX_VERTEX_UNIFORM_VECTORS, iParam);
        LOGI("Max Vertex Uniform Vectors: %d", iParam[0])

        glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_VECTORS, iParam);
        LOGI("Max Fragment Uniform Vectors : %d", iParam[0])

        glGetIntegerv(GL_MAX_VARYING_VECTORS, iParam);
        LOGI("Max Varying Vectors: %d", iParam[0])

        glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, iParam);
        LOGI("Max Texture Image Units : %d", iParam[0])

        glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, iParam);
        LOGI("Max Combined Texture Image Units : %d", iParam[0])

        glGetIntegerv(GL_MAX_TEXTURE_SIZE, iParam);
        LOGI("Max Texture Size : %d", iParam[0])

        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, iParam);
        LOGI("Max Vertex Attribs: %d", iParam[0])

        glGetIntegerv(GL_MAX_VIEWPORT_DIMS, iParam);
        LOGI("Max Viewport Dims : [%d, %d]", iParam[0], iParam[1])

        glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE, iParam);
        LOGI("Max Render Buffer Size: %d", iParam[0])

        glGetIntegerv(GL_STENCIL_BITS, iParam);
        LOGI("Stencil Buffer Bits : %d", iParam[0])

        glGetIntegerv(GL_ALIASED_POINT_SIZE_RANGE, iParam);
        LOGI("Point Size Range: [%d, %d]", iParam[0], iParam[1])

        glGetIntegerv(GL_ALIASED_LINE_WIDTH_RANGE, iParam);
        LOGI("Line Width Range: [%d, %d]", iParam[0], iParam[1])

        glGetIntegerv(GL_DEPTH_BITS, iParam);
        LOGI("Depth Bits: %d", iParam[0])

        LOGI("===== Information end =====")
    }

};

static void setBool(GLuint programId, const std::string &name, bool value) {
    glUniform1i(glGetUniformLocation(programId, name.c_str()), (int) value);
}

static void setInt(GLuint programId, const std::string &name, int value) {
    glUniform1i(glGetUniformLocation(programId, name.c_str()), value);
}

static void setFloat(GLuint programId, const std::string &name, float value) {
    glUniform1f(glGetUniformLocation(programId, name.c_str()), value);
}

static void setVec2(GLuint programId, const std::string &name, const glm::vec2 &value) {
    glUniform2fv(glGetUniformLocation(programId, name.c_str()), 1, &value[0]);
}

static void setVec2(GLuint programId, const std::string &name, float x, float y) {
    glUniform2f(glGetUniformLocation(programId, name.c_str()), x, y);
}

static void setVec3(GLuint programId, const std::string &name, const glm::vec3 &value) {
    glUniform3fv(glGetUniformLocation(programId, name.c_str()), 1, &value[0]);
}

static void setVec3(GLuint programId, const std::string &name, float x, float y, float z) {
    glUniform3f(glGetUniformLocation(programId, name.c_str()), x, y, z);
}

static void setVec4(GLuint programId, const std::string &name, const glm::vec4 &value) {
    glUniform4fv(glGetUniformLocation(programId, name.c_str()), 1, &value[0]);
}

static void setVec4(GLuint programId, const std::string &name, float x, float y, float z, float w) {
    glUniform4f(glGetUniformLocation(programId, name.c_str()), x, y, z, w);
}

static void setMat2(GLuint programId, const std::string &name, const glm::mat2 &mat) {
    glUniformMatrix2fv(glGetUniformLocation(programId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

static void setMat3(GLuint programId, const std::string &name, const glm::mat3 &mat) {
    glUniformMatrix3fv(glGetUniformLocation(programId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

static void setNormalMatrix(GLuint programId, const std::string &name, const glm::mat4 &model) {
    glUniformMatrix3fv(glGetUniformLocation(programId, name.c_str()), 1,
                       GL_FALSE, &glm::transpose(glm::inverse(glm::mat3(model)))[0][0]);
}

static void setNormalMatrix(GLuint programId, const std::string &name, const glm::mat4 &model,
                            const glm::mat4 &view) {
    glUniformMatrix3fv(glGetUniformLocation(programId, name.c_str()), 1,
                       GL_FALSE, &glm::transpose(glm::inverse(glm::mat3(view * model)))[0][0]);
}

static void setMat4(GLuint programId, const std::string &name, const glm::mat4 &mat) {
    glUniformMatrix4fv(glGetUniformLocation(programId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

#endif //LEARNOPENGL_GLUTILS_H
