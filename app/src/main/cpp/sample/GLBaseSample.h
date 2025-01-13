//
// Created by TS on 2024/2/27.
//

#ifndef LEARNOPENGL_GLBASESAMPLE_H
#define LEARNOPENGL_GLBASESAMPLE_H

// 这些定义和Java层的 IMyNativeRendererType 类对应起来
#define SAMPLE_TYPE                                                 100
#define SAMPLE_TYPE_VAO_VBO                                         SAMPLE_TYPE + 0
#define SAMPLE_TYPE_EBO                                             SAMPLE_TYPE + 1
#define SAMPLE_TYPE_VAO_VBO_EXERCISE_1                              SAMPLE_TYPE + 2
#define SAMPLE_TYPE_VAO_VBO_EXERCISE_2                              SAMPLE_TYPE + 3
#define SAMPLE_TYPE_UNIFORM                                         SAMPLE_TYPE + 4
#define SAMPLE_TYPE_TEXTURE_WARP                                    SAMPLE_TYPE + 5
#define SAMPLE_TYPE_TEXTURE2                                        SAMPLE_TYPE + 7
#define SAMPLE_TYPE_MAT                                             SAMPLE_TYPE + 8
#define SAMPLE_TYPE_COORDINATE                                      SAMPLE_TYPE + 9
#define SAMPLE_TYPE_CAMERA                                          SAMPLE_TYPE + 10
#define SAMPLE_TYPE_CAMERA_AUTO_MOVE                                SAMPLE_TYPE + 11
#define SAMPLE_TYPE_COLORS                                          SAMPLE_TYPE + 12
#define SAMPLE_TYPE_COLORS_VIEW                                     SAMPLE_TYPE + 13
#define SAMPLE_TYPE_COLORS_MATERIAL                                 SAMPLE_TYPE + 14
#define SAMPLE_TYPE_LIGHTING_MAPS_DIFFUSE                           SAMPLE_TYPE + 15
#define SAMPLE_TYPE_LIGHTING_CASTERS_DIRECTIONAL                    SAMPLE_TYPE + 16
#define SAMPLE_TYPE_LIGHTING_MERGE                                  SAMPLE_TYPE + 17
#define SAMPLE_TYPE_MODEL_LOADING                                   SAMPLE_TYPE + 18
#define SAMPLE_TYPE_DEPTH_TESTING                                   SAMPLE_TYPE + 19
#define SAMPLE_TYPE_DEPTH_DISPLAY                                   SAMPLE_TYPE + 191
#define SAMPLE_TYPE_STENCIL_TESTING                                 SAMPLE_TYPE + 20
#define SAMPLE_TYPE_BLENDING_DISCARD                                SAMPLE_TYPE + 21
#define SAMPLE_TYPE_CULL_FACE                                       SAMPLE_TYPE + 22
#define SAMPLE_TYPE_FRAME_BUFFERS                                   SAMPLE_TYPE + 23
#define SAMPLE_TYPE_FRAME_BUFFERS_EXERCISE                          SAMPLE_TYPE + 24
#define SAMPLE_TYPE_CUBE_MAPS                                       SAMPLE_TYPE + 25
#define SAMPLE_TYPE_CUBE_MAPS_REFLECTION1                           SAMPLE_TYPE + 26
#define SAMPLE_TYPE_CUBE_MAPS_REFLECTION2                           SAMPLE_TYPE + 27
#define SAMPLE_TYPE_CUBE_MAPS_REFRACTION                            SAMPLE_TYPE + 28
#define SAMPLE_TYPE_CUBE_MAPS_EXERCISE                              SAMPLE_TYPE + 29
#define SAMPLE_TYPE_CUBE_UNIFORM_BUFFER                             SAMPLE_TYPE + 30
#define SAMPLE_TYPE_GEOMETRY_SHADER                                 SAMPLE_TYPE + 31
#define SAMPLE_TYPE_INSTANCING                                      SAMPLE_TYPE + 32
#define SAMPLE_TYPE_INSTANCING_ASTEROIDS                            SAMPLE_TYPE + 33
#define SAMPLE_TYPE_ANIT_ALIASING                                   SAMPLE_TYPE + 34
#define SAMPLE_TYPE_ADVANCED_LIGHTING                               SAMPLE_TYPE + 35
#define SAMPLE_TYPE_ADVANCED_LIGHTING_GAMMA_CORRECTED               SAMPLE_TYPE + 36
#define SAMPLE_TYPE_SHADOW_MAPPING_DEPTH                            SAMPLE_TYPE + 37
#define SAMPLE_TYPE_SHADOW_MAPPING_BASE                             SAMPLE_TYPE + 38
#define SAMPLE_TYPE_NORMAL_MAPPING                                  SAMPLE_TYPE + 39
#define SAMPLE_TYPE_PARALLAX_MAPPING                                SAMPLE_TYPE + 40
#define SAMPLE_TYPE_STEEP_PARALLAX_MAPPING                          SAMPLE_TYPE + 41
#define SAMPLE_TYPE_PARALLAX_OCCLUSION_MAPPING                      SAMPLE_TYPE + 42
#define SAMPLE_TYPE_HDR                                             SAMPLE_TYPE + 43
#define SAMPLE_TYPE_BLOOM                                           SAMPLE_TYPE + 44
#define SAMPLE_TYPE_DEFERRED_SHADING                                SAMPLE_TYPE + 45
#define SAMPLE_TYPE_SSAO                                            SAMPLE_TYPE + 46
#define SAMPLE_TYPE_PBR_LIGHTING                                    SAMPLE_TYPE + 47
#define SAMPLE_TYPE_PBR_LIGHTING_TEXTURED                           SAMPLE_TYPE + 48
#define SAMPLE_TYPE_PBR_IBL_IRRADIANCE_CONVERSION                   SAMPLE_TYPE + 49
#define SAMPLE_TYPE_PBR_IBL_IRRADIANCE                              SAMPLE_TYPE + 50
#define SAMPLE_TYPE_PBR_IBL_SPECULAR                                SAMPLE_TYPE + 51
#define SAMPLE_TYPE_PBR_IBL_SPECULAR_TEXTURED                       SAMPLE_TYPE + 52
#define SAMPLE_TYPE_TEXT_RENDERING                                  SAMPLE_TYPE + 53
#define SAMPLE_TYPE_DEBUGGING                                       SAMPLE_TYPE + 54
#define SAMPLE_TYPE_BREAK_OUT                                       SAMPLE_TYPE + 55
#define SAMPLE_TYPE_WEIGHTED_BLENDED_OIT                            SAMPLE_TYPE + 56

#define DEFAULT_OGL_ASSETS_DIR "/data/data/com.example.learnopengl"

#include <GLUtils.h>

#include "CameraUtils.h"

class GLBaseSample {
public:
    GLBaseSample() {

    }

    virtual ~GLBaseSample() {}

    virtual void Create() = 0;

    virtual void Change(float width, float height) {
        LOGD("Change() width = %f , height = %f\n", width, height)
        SCR_WIDTH = width;
        SCR_HEIGHT = height;
        // Set the viewport
        // 通知OpenGL ES 用于绘制的2D渲染表面的原点、宽度和高度。
        // 在OpenGL ES 中，视口(Viewport) 定义所有OpenGL ES 渲染操作最终显示的2D矩形
        // 视口(Viewport) 由原点坐标(x,y)和宽度(width) 、高度(height)定义。
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    }

    virtual void Draw() = 0;

    virtual void ProcessInput(int i) {
        if (i == KEY_W)
            cameraUtils.ProcessKeyboard(FORWARD, deltaTime);
        if (i == KEY_S)
            cameraUtils.ProcessKeyboard(BACKWARD, deltaTime);
        if (i == KEY_A)
            cameraUtils.ProcessKeyboard(MOVE_LEFT, deltaTime);
        if (i == KEY_D)
            cameraUtils.ProcessKeyboard(MOVE_RIGHT, deltaTime);
        if (i == KEY_B)
            cameraUtils.ProcessKeyboard(BOOLEAN, deltaTime);
    }

    virtual void ProgressChanged(int i) {
        seek = i;
    }

    virtual void MoveCallback(float x, float y) {
        float xpos = static_cast<float>(x);
        float ypos = static_cast<float>(y);

        cameraUtils.ProcessMouseMovement(xpos, ypos);
    }

    virtual void Shutdown() {
        if (m_ProgramObj) {
            glDeleteProgram(m_ProgramObj);
            m_ProgramObj = GL_NONE;
        }
    }

    void CreateFBODebugShader() {
        m_ProgramObj_FBO_Debug = GLUtils::createProgram("shaders/vs_debugging_fbo_texture.glsl", "shaders/fs_debugging_fbo_texture.glsl");
        glUseProgram(m_ProgramObj_FBO_Debug);
        setInt(m_ProgramObj_FBO_Debug, "fboAttachment", 0);
    }

    /**
     * 调试帧缓冲的纹理附件
     * @param textureID
     */
    void DisplayFramebufferTexture(GLuint textureID, GLboolean isDepth = false) {
        if (debuggingVAO == 0) {
            float quadVertices[] = {
                    // positions      // texture Coords
                    0.5f,  1.0f, 0.0f, 0.0f, 1.0f,
                    0.5f,  0.5f, 0.0f, 0.0f, 0.0f,
                    1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
                    1.0f,  0.5f, 0.0f, 1.0f, 0.0f,
            };
            // setup plane VAO
            glGenVertexArrays(1, &debuggingVAO);
            glGenBuffers(1, &debuggingVBO);
            glBindVertexArray(debuggingVAO);
            glBindBuffer(GL_ARRAY_BUFFER, debuggingVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glUseProgram(m_ProgramObj_FBO_Debug);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureID);
        setBool(m_ProgramObj_FBO_Debug, "isDepth", isDepth);
        glBindVertexArray(debuggingVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);
        glUseProgram(0);
    }

    // 渲染一个屏幕大小的四边形
    void renderQuad() {
        if (quadVAO == 0) {
            float quadVertices[] = {
                    // 位置             // 纹理坐标
                    -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                    1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
                    1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            };
            // 配置VAO
            glGenVertexArrays(1, &quadVAO);
            glGenBuffers(1, &quadVBO);
            glBindVertexArray(quadVAO);
            glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
            // 创建缓冲区内存，并填充数据，当缓冲区数据是动态的时，可以使用GL_DYNAMIC_DRAW，并且可以不加载数据只申请内存
            // 缓冲区存放的顶点数据是不会变化的，所以可以使用GL_STATIC_DRAW
            glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
            // 启用两个顶点数据，并告知OpenGL顶点属性数据的加载方式，包括：数据大小，数据类型，是否是标准数据，读取补偿，内存地址偏移量
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        }
        /// 绑定VAO，高数OpenGL从哪个缓冲区读取数据
        glBindVertexArray(quadVAO);
        /// 使用GL_TRIANGLE_STRIP只需要4个顶点，节省了两个顶点数据
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        /// 渲染完成后，接触VAO的绑定
        glBindVertexArray(0);
    }

protected:
    /**
     * 程序对象
     */
    GLuint m_ProgramObj;
    GLuint m_ProgramObj_FBO_Debug;
    /**
     * 屏幕宽高
     */
    float SCR_WIDTH = 2208.0f;
    float SCR_HEIGHT = 1756.0f;
    CameraUtils cameraUtils = CameraUtils(glm::vec3(0.0f, 0.0f,  3.0f));
    float deltaTime = 0.0f; // 当前帧花费的时间
    // 进度条滑动值范围：0-100
    unsigned int seek = 0;

    GLuint quadVAO, quadVBO;
    GLuint debuggingVAO, debuggingVBO;
};

#endif //LEARNOPENGL_GLBASESAMPLE_H
