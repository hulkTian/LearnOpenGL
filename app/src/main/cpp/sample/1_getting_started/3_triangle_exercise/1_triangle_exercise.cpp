//
// Created by TS on 2024/2/29.
//
// 使用glDrawArrays按照默认顺序绘制两个相连的三角形

#include "1_triangle_exercise.h"
REGISTER_SAMPLE(SAMPLE_TYPE_VAO_VBO_EXERCISE_1, triangle_exercise_1)

void triangle_exercise_1::Create() {
    GLUtils::printGLInfo();

    float vertices[] = {
            // first triangle
            -0.9f, -0.5f, 0.0f,  // left
            -0.0f, -0.5f, 0.0f,  // right
            -0.45f, 0.5f, 0.0f,  // top
            // second triangle
            0.0f, -0.5f, 0.0f,  // left
            0.9f, -0.5f, 0.0f,  // right
            0.45f, 0.5f, 0.0f   // top
    };

    /** 1. 创建VAO对象，用于记录顶点属性配置*/
    glGenVertexArrays(1, &VAO);
    /** 2. 创建VBO对象，用于缓冲顶点数据*/
    glGenBuffers(1, &VBO);
    /** 3. 绑定VAO对象，告诉OpenGL当前所有操作都记录在VAO对象中 */
    glBindVertexArray(VAO);
    /** 4. 绑定VBO对象到GL_ARRAY_BUFFER槽位，把顶点数据复制到VBO缓冲中 */
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    /** 配置第0个顶点数据的读取规则，并把VBO对象的引用也记录下来 */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);
    /** 启用第0个顶点属性，告诉OpenGL第0个顶点属性激活*/
    glEnableVertexAttribArray(0);

    /** 创建着色器程序,并编译着色器代码 */
    m_ProgramObj = GLUtils::createProgram("shaders/vs_rectangle.glsl", "shaders/fs_rectangle.glsl");

    if (!m_ProgramObj) {
        LOGD("Could not create program")
        return;
    }
    /** 设置清屏颜色 */
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}

void triangle_exercise_1::Draw() {
    /** 清除颜色缓冲区，设置为设置的清屏颜色 */
    glClear(GL_COLOR_BUFFER_BIT);
    /** 激活着色器程序 */
    glUseProgram(m_ProgramObj);
    /**  绑定需要绘制的VAO */
    glBindVertexArray(VAO);
    /** 绘制三角形，按照给定的数组的顶点顺序绘制，两个三角形有六个顶点 */
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void triangle_exercise_1::Shutdown() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    GLBaseSample::Shutdown();
}
