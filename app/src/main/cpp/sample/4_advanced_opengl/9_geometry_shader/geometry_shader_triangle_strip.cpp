//
// Created by tzh on 2025/12/10.
//

#include "geometry_shader_triangle_strip.h"

REGISTER_SAMPLE(SAMPLE_TYPE_GEOMETRY_SHADER_TRIANGLE_STRIP, geometry_shader_triangle_strip)

static float points[] = {
        // 位置                  // 颜色
        -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, // 左上
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, // 右上
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // 右下
        -0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // 左下
};

void geometry_shader_triangle_strip::Create() {
    GLUtils::printGLInfo();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                          5 * sizeof(float), (void *) nullptr);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                          5 * sizeof(float), (void *) (2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //创建着色器程序,并编译着色器代码
    m_ProgramObj = GLUtils::createProgram("shaders/vs_geometry_shader_triangle_strip.glsl",
                                          "shaders/fs_geometry_shader_triangle_strip.glsl",
                                          "shaders/gs_geometry_shader_triangle_strip.glsl");

    if (!m_ProgramObj) {
        LOGD("Could not create program")
        return;
    }

    //设置清屏颜色
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}

void geometry_shader_triangle_strip::Draw() {
    //清除屏幕
    glClear(GL_COLOR_BUFFER_BIT);

    //激活着色器程序
    glUseProgram(m_ProgramObj);

    glBindVertexArray(VAO);
    //绘制点
    glDrawArrays(GL_POINTS, 0, 4);
}

void geometry_shader_triangle_strip::Shutdown() {
    if (VBO) {
        glDeleteBuffers(1, &VBO);
        VBO = 0;
    }

    if (VAO) {
        glDeleteVertexArrays(1, &VAO);
        VAO = 0;
    }

    if (m_ProgramObj) {
        glDeleteProgram(m_ProgramObj);
        m_ProgramObj = 0;
    }
    GLBaseSample::Shutdown();
}