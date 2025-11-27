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
     * Loads a texture from assets/texture/<name>
     */
    static GLuint loadTexture(char const * path, bool gammaCorrection);

    /**
     * Loads a texture from assets/texture/<name>
     */
    static GLuint loadTgaTexture(const char *fileName, GLint internalformat = GL_RGBA,
                                 GLenum format = GL_RGBA,
                                 GLenum type = GL_UNSIGNED_BYTE,
                                 const bool flip = true,
                                 unsigned int texture_warp_s = GL_CLAMP_TO_EDGE,
                                 unsigned int texture_warp_t = GL_CLAMP_TO_EDGE,
                                 unsigned int texture_min_filter = GL_NEAREST,
                                 unsigned int texture_max_filter = GL_NEAREST);

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

static void setMat4(GLuint programId, const std::string &name, const glm::mat4 &mat) {
    glUniformMatrix4fv(glGetUniformLocation(programId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

#endif //LEARNOPENGL_GLUTILS_H
