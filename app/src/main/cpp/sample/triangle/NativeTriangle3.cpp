//
// Created by TS on 2024/2/29.
//
// 绘制两个相连的三角形

#include "NativeTriangle3.h"

static float vertices[] = {
        // first triangle
        -0.9f, -0.5f, 0.0f,  // left
        -0.0f, -0.5f, 0.0f,  // right
        -0.45f, 0.5f, 0.0f,  // top
        // second triangle
        0.0f, -0.5f, 0.0f,  // left
        0.9f, -0.5f, 0.0f,  // right
        0.45f, 0.5f, 0.0f   // top
};

void NativeTriangle3::Create() {
    GLUtils::printGLInfo();

    //加载顶点着色器代码
    VERTEX_SHADER = GLUtils::openTextFile("shaders/vertex_shader_triangle.glsl");

    //加载片段着色器代码
    FRAGMENT_SHADER = GLUtils::openTextFile("shaders/fragment_shader_triangle.glsl");

    //创建着色器程序,并编译着色器代码
    m_ProgramObj = GLUtils::createProgram(&VERTEX_SHADER, &FRAGMENT_SHADER);

    if (!m_ProgramObj) {
        LOGD("Could not create program")
        return;
    }
    //设置清屏颜色
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}

void NativeTriangle3::Draw() {
    //清除屏幕
    glClear(GL_COLOR_BUFFER_BIT);

    //声明顶点缓存对象和顶点数组对象
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // 1. 绑定VAO
    glBindVertexArray(VAO);

    // 2. 把顶点数组复制到缓冲中供OpenGL使用
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 3. 设置顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);
    //启用顶点属性，这里也要传递对应的属性位置值
    glEnableVertexAttribArray(0);

    //激活着色器程序
    glUseProgram(m_ProgramObj);

    //绘制三角形，按照给定的数组的顶点顺序绘制，两个三角形有六个顶点
    glDrawArrays(GL_TRIANGLES, 0, 6);

    //关闭顶点属性
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(m_ProgramObj);
}

void NativeTriangle3::Shutdown() {
    GLBaseSample::Shutdown();
}
