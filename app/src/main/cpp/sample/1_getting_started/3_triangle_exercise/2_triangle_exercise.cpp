//
// Created by TS on 2024/2/29.
//
//通过绘制两个不同的三角形，了解多个VAO和VBO的使用方式

#include "2_triangle_exercise.h"
REGISTER_SAMPLE(SAMPLE_TYPE_VAO_VBO_EXERCISE_2, triangle_exercise_2)

static float first_vertices[] = {
        -0.9f, -0.5f, 0.0f,  // left
        -0.0f, -0.5f, 0.0f,  // right
        -0.45f, 0.5f, 0.0f,  // top
};
static float second_vertices[] = {
        0.0f, -0.5f, 0.0f,  // left
        0.9f, -0.5f, 0.0f,  // right
        0.45f, 0.5f, 0.0f   // top
};

void triangle_exercise_2::Create() {
    GLUtils::printGLInfo();

    //声明VBO和VAO
    glGenVertexArrays(2, VAOs);
    glGenBuffers(2, VBOs);

    //绑定第一个三角形的VAO和VBO
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(first_vertices), first_vertices, GL_STATIC_DRAW);
    //设置顶点属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(0);

    //绑定第二个三角形的VAO和VBO
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(second_vertices), second_vertices, GL_STATIC_DRAW);
    //给第二个三角形设置顶点属性，由于我们使用的相同的着色器所以设置的顶点属性也相同
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(0);

    //创建着色器程序,并编译着色器代码
    m_ProgramObj = GLUtils::createProgram("shaders/vs_rectangle.glsl", "shaders/fs_rectangle.glsl");

    if (!m_ProgramObj) {
        LOGD("Could not create program")
        return;
    }
    //设置清屏颜色
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}

void triangle_exercise_2::Draw() {
    //清除屏幕
    glClear(GL_COLOR_BUFFER_BIT);

    //激活着色器程序
    glUseProgram(m_ProgramObj);

    //绘制形状
    glBindVertexArray(VAOs[0]);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(VAOs[1]);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void triangle_exercise_2::Shutdown() {
    //关闭顶点属性
    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    GLBaseSample::Shutdown();
}
