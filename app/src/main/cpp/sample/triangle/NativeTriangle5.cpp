//
// Created by TS on 2024/3/1.
//
//使用uniform定义一个颜色变量，再循环渲染过程中随着时间设置颜色变量的值

#include "NativeTriangle5.h"

static float vVertices[] = {
        0.0f, 0.5f, 0.0f,            // 上角
        -0.5f, -0.5f, 0.0f,          // 左下角
        0.5f, -0.5f, 0.0f            // 右下角
};

void NativeTriangle5::Create() {
    GLUtils::printGLInfo();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    //绑定三角形的VAO和VBO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vVertices), vVertices, GL_STATIC_DRAW);
    //设置顶点属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) nullptr);
    glEnableVertexAttribArray(0);

    //创建着色器程序,并编译着色器代码
    m_ProgramObj = GLUtils::createProgram("shaders/vs_triangle5.glsl",
                                          "shaders/fs_triangle5.glsl");

    if (!m_ProgramObj) {
        LOGD("Could not create program")
        return;
    }
    //设置清屏颜色
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}

void NativeTriangle5::Draw() {
    //清除屏幕
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(m_ProgramObj);

    glBindVertexArray(VAO);

    // 更新uniform颜色
    float time = TimeUtils::currentTimeSeconds();
    auto greenValue = sin(time) / 2.0f + 0.5f;
    int vertexColorLocation = glGetUniformLocation(m_ProgramObj, "ourColor");
    glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void NativeTriangle5::Shutdown() {

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    GLBaseSample::Shutdown();
}
