//
// Created by ts on 2024/12/17.
//
/**
 * 第一个渲染例子，展示了VAO和VBO的使用流程，着色器代码的编写、加载和编译流程，以及绘制一个三角形的简单流程。
 */

#include "vao_vbo.h"

REGISTER_SAMPLE(SAMPLE_TYPE_VAO_VBO, vao_vbo)

void vao_vbo::Create() {
    // 打印当前设备信息
    GLUtils::printGLInfo();

    GLfloat vertices[] = {
            //顶点坐标数据       //颜色数据
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f
    };

    /** 1.创建并绑定VAO，告诉OpenGL怎么读取缓冲在VBO中的顶点数据 */
    glGenVertexArrays(1, &VAO);
    /** 1.1 绑定VAO，告诉OpenGL当前VAO正在使用 */
    glBindVertexArray(VAO);
    /** 2.创建一个VBO，存储顶点数据 */
    glGenBuffers(1, &VBO);
    /** 2.1 绑定VBO，告诉OpenGL这个VBO是顶点数据缓冲区 */
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    /** 2.2 将顶点数据复制到VBO中，数据大小为sizeof(vertices)，数据来源为vertices数组。GL_STATIC_DRAW：静态绘制，数据不会经常改变时使用 */
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    /** 3.第0个顶点属性数据读取规则：由3个float数据组成，每个属性间隔6个float数据，起始位置偏移量为0 */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          6 * sizeof(float), (void *) nullptr);
    /** 3.1 启动第0个顶点属性，告诉OpenGL需要读取第0个顶点属性数据 */
    glEnableVertexAttribArray(0);
    /** 4.第1个顶点属性数据读取规则：由3个float数据组成，每个属性间隔6个float数据，起始位置偏移量为3个float数据 */
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                          6 * sizeof(float), (void *) (3 * sizeof(float)));
    /** 4.1 启动第1个顶点属性，告诉OpenGL需要读取第1个顶点属性数据 */
    glEnableVertexAttribArray(1);
    /** 5.优化可选操作：解绑VAO，避免当前VAO在使用前被修改 */
    glBindVertexArray(0);

    /** 6.加载并编译着色器代码流程：
     * 6.1.着色器代码文件通过AAssetManager读取到内存中;
     * 6.2.编译着色器代码：
     * 6.2.1调用glCreateShader创建着色器对象，需要指定是顶点着色器还是片段着色器;
     * 6.2.2调用glShaderSource把内存中的着色器代码与着色器对象绑定;
     * 6.2.3调用glCompileShader编译着色器代码;
     * 6.3.调用glCreateProgram创建着色器程序对象;
     * 6.4.调用glAttachShader绑定顶点着色器对象和片元着色器对象;
     * 6.5.调用glLinkProgram链接着色器程序; */
    m_ProgramObj = GLUtils::createProgram(
            "shaders/vs_triangle.glsl", "shaders/fs_triangle.glsl");

    /** 设置清屏颜色 */
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}

void vao_vbo::Draw() {
    /** 清除屏幕颜色缓冲区，将屏幕颜色缓冲区的内容设置为设置的清屏颜色，用于绘制新的帧 */
    glClear(GL_COLOR_BUFFER_BIT);

    /** 激活着色器程序，告诉OpenGL当前着色器程序正在使用 */
    glUseProgram(m_ProgramObj);

    /** 绑定VAO，告诉OpenGL当前VAO正在使用，需要读取VAO中的顶点属性数据 */
    glBindVertexArray(VAO);

    /** 绘制三角形
    * 第一个参数指定需要绘制的形状时三角形
    * 第二个参数指定顶点起始索引
    * 第三个参数指定需要绘制的顶点数 */
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void vao_vbo::Shutdown() {
    /** 告诉OpenGL*/
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    GLBaseSample::Shutdown();
}