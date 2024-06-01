// 通过


#include "NativeTriangle6.h"


void NativeTriangle::Create() {
    GLUtils::printGLInfo();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vVertices), vVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) nullptr);
    glEnableVertexAttribArray(0);

    //创建着色器程序,并编译着色器代码
    m_ProgramObj = GLUtils::createProgram("shaders/vertex_shader_triangle.glsl",
                                          "shaders/fragment_shader_triangle.glsl");

    if (!m_ProgramObj) {
        LOGD("Could not create program")
        return;
    }
    //设置清屏颜色
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}

void NativeTriangle::Draw() {
    //清除屏幕
    glClear(GL_COLOR_BUFFER_BIT);

    //激活着色器程序
    glUseProgram(m_ProgramObj);

    //绑定VAO
    glBindVertexArray(VAO);

    //绘制形状
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void NativeTriangle::Shutdown() {
    //关闭顶点属性
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    GLBaseSample::Shutdown();
}