//
// Created by tzh on 2025/12/9.
//

/**
 * GLSL的内建变量：
 * 片段着色器变量 gl_FragCoord ：
 * 用于获取当前片段在窗口坐标系中的位置。它是一个包含片段的屏幕空间坐标的内建变量，
 * 通常用于实现屏幕空间效果，如屏幕空间反射(SSR)、屏幕空间环境光遮蔽(SSAO)等。
 * gl_FragCoord 是一个 vec4 类型的变量，x 和 y 分量表示片段的屏幕空间坐标，原点位于左下角；
 * z 分量表示深度值，w 分量通常为1.0。
 */

#include "advanced_glsl_fragcoord.h"

REGISTER_SAMPLE(SAMPLE_TYPE_ADVANCED_GLSL_FRAGCOORD, advanced_glsl_fragcoord)

static float cubeVertices[] = {
        // positions
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,

        -0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,

        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,

        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f,
};

void advanced_glsl_fragcoord::Create() {
    m_programObj = GLUtils::createProgram("shaders/vs_advanced_glsl_fragcoord.glsl",
                                          "shaders/fs_advanced_glsl_fragcoord.glsl");
    if (!m_programObj) {
        LOGD("Could not create program")
        return;
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);
}

void advanced_glsl_fragcoord::Draw() {
    //计算每一帧绘制的时间：先记录当前在开始时间
    float currentFrame = TimeUtils::currentTimeSeconds();

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(m_programObj);

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 projection = glm::perspective(cameraUtils.Zoom, SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
    setMat4(m_programObj, "model", model);
    setMat4(m_programObj, "view", cameraUtils.GetViewMatrix());
    setMat4(m_programObj, "projection", projection);

    setFloat(m_programObj, "width", SCR_WIDTH);
    setFloat(m_programObj, "height", SCR_HEIGHT);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    deltaTime = TimeUtils::currentTimeSeconds() - currentFrame;
}

void advanced_glsl_fragcoord::Shutdown() {
    if (m_programObj) {
        glDeleteProgram(m_programObj);
        m_programObj = GL_NONE;
    }
    if (VBO) {
        glDeleteBuffers(1, &VBO);
        VBO = GL_NONE;
    }
    if (VAO) {
        glDeleteVertexArrays(1, &VAO);
        VAO = GL_NONE;
    }
    GLBaseSample::Shutdown();
}
