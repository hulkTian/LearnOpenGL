//
// Created by TS on 2024/3/1.
//

#include "NativeTriangle5.h"

static float vVertices[] = {
        0.0f, 0.5f, 0.0f,            // 上角
        -0.5f, -0.5f, 0.0f,          // 左下角
        0.5f, -0.5f, 0.0f            // 右下角
};

//声明VBO和VAO
GLuint VBO, VAO;

void NativeTriangle5::Create() {
    GLUtils::printGLInfo();

    //加载顶点着色器代码
    VERTEX_SHADER = GLUtils::openTextFile("shaders/vertex_shader_triangle5.glsl");

    //加载片段着色器代码
    FRAGMENT_SHADER = GLUtils::openTextFile("shaders/fragment_shader_triangle5.glsl");

    //创建着色器程序,并编译着色器代码
    m_ProgramObj = GLUtils::createProgram(&VERTEX_SHADER, &FRAGMENT_SHADER);

    if (!m_ProgramObj) {
        LOGD("Could not create program")
        return;
    }
    //设置清屏颜色
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    //绑定三角形的VAO和VBO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vVertices), vVertices, GL_STATIC_DRAW);
    //设置顶点属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(0);

    glBindVertexArray(VAO);
}


void NativeTriangle5::Draw() {
    //清除屏幕
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(m_ProgramObj);

    // 更新uniform颜色
    int time = TimeUtils::currentTimeSeconds();
    float greenValue = sin(time) / 2.0f + 0.5f;
    int vertexColorLocation = glGetUniformLocation(m_ProgramObj, "ourColor");
    glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void NativeTriangle5::Shutdown() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    GLBaseSample::Shutdown();
}
