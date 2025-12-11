//
// Created by ts on 2024/10/14.
//

/**
 * 几何着色器(Geometry Shader) 示例-点扩展为实线
 * 在这个示例中，我们将使用几何着色器将输入的点扩展为实线。
 * 具体来说，几何着色器会接收顶点着色器输出的点，并为每个点生成一条水平线段。
 *
 * 几何着色器是OpenGL渲染管线中的一个可选阶段，位于顶点着色器和片段着色器之间。
 * 它允许开发者在图形渲染过程中动态地生成、修改或删除图元（如点、线段和三角形）。
 *
 * gl_in内建(Built-in)变量，它是一个数组，包含了传递给几何着色器的所有顶点信息。
 */

#include "GeometryShaderPoint.h"
REGISTER_SAMPLE(SAMPLE_TYPE_GEOMETRY_SHADER, GeometryShaderPoint)
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
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                          2 * sizeof(float), (void *) nullptr);
    glEnableVertexAttribArray(0);

    //创建着色器程序,并编译着色器代码
    m_ProgramObj = GLUtils::createProgram("shaders/vs_geometry_shader_point.glsl",
                                          "shaders/fs_geometry_shader_point.glsl",
                                          "shaders/gs_geometry_shader_point.glsl");

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