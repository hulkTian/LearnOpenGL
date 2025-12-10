//
// Created by tzh on 2025/12/9.
//

/**
 * GLSL的内建变量：
 * 顶点着色器变量 GL_PointSize ：
 * 用于设置点精灵的大小。点精灵(Point Sprite) 是一种特殊的图元，用于在3D空间中表示点状对象。
 * 通过设置 GL_PointSize 变量，可以控制点在屏幕上的显示大小。
 */

#include "advanced_glsl_point_size.h"

REGISTER_SAMPLE(SAMPLE_TYPE_ADVANCED_GLSL_POINT_SIZE, advanced_glsl_point_size)

static float points[] = {
        // positions
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
};
void advanced_glsl_point_size::Create() {
    m_programObj = GLUtils::createProgram("shaders/vs_advanced_glsl_point_size.glsl",
                                          "shaders/fs_advanced_glsl_point_size.glsl");
    if (!m_programObj) {
        LOGD("Could not create program")
        return;
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);
}

void advanced_glsl_point_size::Draw() {
    //计算每一帧绘制的时间：先记录当前在开始时间
    float currentFrame = TimeUtils::currentTimeSeconds();

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(m_programObj);

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 projection = glm::perspective(45.0f, SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
    setMat4(m_programObj, "model", model);
    setMat4(m_programObj, "view", cameraUtils.GetViewMatrix());
    setMat4(m_programObj, "projection", projection);

    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, 3);

    // 计算每一帧绘制的时间，再计算当前帧结束时间
    deltaTime = TimeUtils::currentTimeSeconds() - currentFrame;
}

void advanced_glsl_point_size::Shutdown() {
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
