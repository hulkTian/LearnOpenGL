//
// Created by TS on 2024/2/28.
//

#include "NativeTriangle2.h"

GLfloat points[] = {
        0.0f,  0.5f,  0.0f,
        0.5f, -0.5f,  0.0f,
        -0.5f, -0.5f,  0.0f
};

GLfloat colours[] = {
        1.0f, 0.0f,  0.0f,
        0.0f, 1.0f,  0.0f,
        0.0f, 0.0f,  1.0f
};

void NativeTriangle2::Create() {
    GLUtils::printGLInfo();

    //创建并绑定顶点数据VBO
    glGenBuffers(1, &points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    //创建并绑定颜色数据VBO
    glGenBuffers(1, &colours_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colours), colours, GL_STATIC_DRAW);

    //创建并绑定VAO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, colours_vbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)nullptr);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    //加载顶点着色器代码
    VERTEX_SHADER = GLUtils::openTextFile("shaders/vertex_shader_triangle2.glsl");

    //加载片段着色器代码
    FRAGMENT_SHADER = GLUtils::openTextFile("shaders/fragment_shader_triangle2.glsl");

    //创建着色器程序,并编译着色器代码
    m_ProgramObj = GLUtils::createProgram(&VERTEX_SHADER, &FRAGMENT_SHADER);

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

    //绘制
    glUseProgram(m_ProgramObj);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void NativeTriangle2::Shutdown() {
    //释放缓冲区资源
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &points_vbo);
    glDeleteBuffers(1, &colours_vbo);
    GLBaseSample::Shutdown();
}