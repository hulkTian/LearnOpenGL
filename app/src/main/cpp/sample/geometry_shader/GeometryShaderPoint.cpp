//
// Created by ts on 2024/10/14.
//

#include "GeometryShaderPoint.h"

static float points[] = {
        -0.5f,  0.5f, // 左上
        0.5f,  0.5f, // 右上
        0.5f, -0.5f, // 右下
        -0.5f, -0.5f  // 左下
};

void GeometryShaderPoint::Create() {
    GLUtils::printGLInfo();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    //创建着色器程序,并编译着色器代码
    m_ProgramObj = GLUtils::createProgram("shaders/vertex_shader_geometry_shader_point.glsl",
                                          "shaders/fragment_shader_geometry_shader_point.glsl",
                                          "shaders/geometry_shader_geometry_shader_point.glsl");

    if (!m_ProgramObj) {
        LOGD("Could not create program")
        return;
    }

    //设置清屏颜色
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}

void GeometryShaderPoint::Draw() {
    //清除屏幕
    glClear(GL_COLOR_BUFFER_BIT);

    //激活着色器程序
    glUseProgram(m_ProgramObj);

    glBindVertexArray(VAO);
    glDrawArrays(GL_POINTS, 0, 4);
}

void GeometryShaderPoint::Shutdown() {
    glDeleteBuffers(1, &VAO);
    glDeleteBuffers(1, &VBO);
    GLBaseSample::Shutdown();
}