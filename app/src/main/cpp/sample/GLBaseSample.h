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

#define KEY_W 1
#define KEY_S 2
#define KEY_A 3
#define KEY_D 4

#include <GLUtils.h>

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

    }

    virtual void MoveCallback(double x, double y) {

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

};

#endif //LEARNOPENGL_GLBASESAMPLE_H
