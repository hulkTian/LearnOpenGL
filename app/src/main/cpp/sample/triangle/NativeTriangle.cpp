//
// Created by TS on 2024/2/28.
//
//通过绘制一个三角形，理解VAO、VBO、顶点属性指针、着色器代码编译过程、着色器程序对象和图形的渲染过程。

#include "NativeTriangle.h"

static float vVertices[] = {
        0.0f, 0.5f, 0.0f,            // 上角
        -0.5f, -0.5f, 0.0f,          // 左下角
        0.5f, -0.5f, 0.0f            // 右下角
};

void NativeTriangle::Create() {
    GLUtils::printGLInfo();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // 1. 绑定VAO
    glBindVertexArray(VAO);

    // 2. 把顶点数组复制到缓冲中供OpenGL使用ts
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vVertices), vVertices, GL_STATIC_DRAW);

    // 3. 设置顶点属性指针
    //第一个参数指定顶点属性，在顶点着色器代码中声明顶点属性时，我们指定了属性对应的位置值
    //第二个参数指定每个顶点包含的分量数
    //第三个参数指定顶点分量的数据类型
    //第四个参数指定是否需要把顶点数据进行标准化。如果设置GL_TRUE，所有数据都会被映射到0（对于signed数据是-1）到1之间
    //第五个参数指定步长，它告诉我们在连续的顶点属性组之间的间隔。设置0，让OpenGL 自己决定步长，只有当数值是紧密排列时才能使用
    //第六个参数指定顶点数据
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vVertices);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) nullptr);
    //启用顶点属性，这里也要传递对应的属性位置值
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
    //第一个参数指定需要绘制的形状时三角形
    //第二个参数指定顶点起始索引
    //第三个参数指定需要绘制的顶点数
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void NativeTriangle::Shutdown() {
    //关闭顶点属性
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    GLBaseSample::Shutdown();
}