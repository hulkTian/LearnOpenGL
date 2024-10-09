//
// Created by TS on 2024/2/27.
//

#ifndef LEARNOPENGL_GLBASESAMPLE_H
#define LEARNOPENGL_GLBASESAMPLE_H

// 这些定义和Java层的 IMyNativeRendererType 类对应起来
#define SAMPLE_TYPE                             100
#define SAMPLE_TYPE_KEY_TRIANGLE                                    SAMPLE_TYPE + 0
#define SAMPLE_TYPE_KEY_RECTANGLE                                   SAMPLE_TYPE + 1
#define SAMPLE_TYPE_TRIANGLE2                                       SAMPLE_TYPE + 2
#define SAMPLE_TYPE_TRIANGLE3                                       SAMPLE_TYPE + 3
#define SAMPLE_TYPE_TRIANGLE4                                       SAMPLE_TYPE + 4
#define SAMPLE_TYPE_TRIANGLE5                                       SAMPLE_TYPE + 5
#define SAMPLE_TYPE_TEXTURE                                         SAMPLE_TYPE + 6
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
#define SAMPLE_TYPE_STENCIL_TESTING                                 SAMPLE_TYPE + 20
#define SAMPLE_TYPE_BLENDING_DISCARD                                SAMPLE_TYPE + 21
#define SAMPLE_TYPE_CULL_FACE                                       SAMPLE_TYPE + 22
#define SAMPLE_TYPE_FRAME_BUFFERS                                   SAMPLE_TYPE + 23
#define SAMPLE_TYPE_FRAME_BUFFERS_EXERCISE                          SAMPLE_TYPE + 24

#define KEY_W 1
#define KEY_S 2
#define KEY_A 3
#define KEY_D 4

#define DEFAULT_OGL_ASSETS_DIR "/data/user/0/com.example.learnopengl/files"

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
        m_Width = width;
        m_Height = height;
        // Set the viewport
        // 通知OpenGL ES 用于绘制的2D渲染表面的原点、宽度和高度。
        // 在OpenGL ES 中，视口(Viewport) 定义所有OpenGL ES 渲染操作最终显示的2D矩形
        // 视口(Viewport) 由原点坐标(x,y)和宽度(width) 、高度(height)定义。
        glViewport(0, 0, m_Width, m_Height);
    }

    virtual void Draw() = 0;

    virtual void ProcessInput(int i) {
        if (i == KEY_W)
            cameraUtils.ProcessKeyboard(FORWARD, deltaTime);
        if (i == KEY_S)
            cameraUtils.ProcessKeyboard(BACKWARD, deltaTime);
        if (i == KEY_A)
            cameraUtils.ProcessKeyboard(LEFT, deltaTime);
        if (i == KEY_D)
            cameraUtils.ProcessKeyboard(RIGHT, deltaTime);
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



protected:
    /**
     * 程序对象
     */
    GLuint m_ProgramObj;
    /**
     * 屏幕宽高
     */
    float m_Width;
    float m_Height;
    CameraUtils cameraUtils = CameraUtils(glm::vec3(0.0f, 0.0f,  3.0f));
    float deltaTime = 0.0f; // 当前帧花费的时间
};

#endif //LEARNOPENGL_GLBASESAMPLE_H
