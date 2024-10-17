//
// Created by TS on 2024/2/28.
//
// 在着色器代码中定义两个属性，一个位置属性，一个颜色属性，并通过CPU代码把位置数据和颜色数据设置给两个属性

#include "NativeTriangle2.h"

GLfloat vertices[] = {
        //顶点数据                         //颜色数据
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f
};

void NativeTriangle2::Create() {
    GLUtils::printGLInfo();

    //创建并绑定顶点数据VBO
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //创建并绑定VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    //设置位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) nullptr);
    glEnableVertexAttribArray(0);
    //设置颜色属性，最后一个参数需要注意设置起始位置的偏移量
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    //创建着色器程序,并编译着色器代码
    m_ProgramObj = GLUtils::createProgram("shaders/vs_triangle2.glsl",
                                          "shaders/fs_triangle2.glsl");

    if (!m_ProgramObj) {
        LOGD("Could not create program")
        return;
    }
    //设置清屏颜色
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}

void NativeTriangle2::Draw() {
    //清除屏幕
    glClear(GL_COLOR_BUFFER_BIT);

    //激活着色器程序
    glUseProgram(m_ProgramObj);
    //绑定VAO
    glBindVertexArray(vao);
    //绘制三角形
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void NativeTriangle2::Shutdown() {
    //释放缓冲区资源
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    GLBaseSample::Shutdown();
}