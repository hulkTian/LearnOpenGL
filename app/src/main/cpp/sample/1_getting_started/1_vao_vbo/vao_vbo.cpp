//
// Created by ts on 2024/12/17.
//
/**
 * 第一个渲染例子，展示了VAO和VBO的使用流程，着色器代码的编写、加载和编译流程，以及绘制一个三角形的简单流程。
 */

#include "vao_vbo.h"

void vao_vbo::Create() {
    // 打印当前设备信息
    GLUtils::printGLInfo();

    GLfloat vertices[] = {
            //顶点坐标数据       //颜色数据
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f
    };

    // 1.创建并绑定VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    // 2.创建并绑定VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 3.复制顶点数据到VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 4.设置顶点属性配置
    // 设置位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) nullptr);
    // 5.启动指定索引的顶点属性
    glEnableVertexAttribArray(0); // 启用第0个顶点属性
    // 设置颜色属性，最后一个参数需要注意设置起始位置的偏移量
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1); // 启用第1个顶点属性
    // 6.优化可选操作：解绑VAO，避免当前VAO在使用前被修改
    glBindVertexArray(0);

    //加载并编译着色器代码流程：
    //1.着色器代码文件通过AAssetManager读取到内存中;
    //2.编译着色器代码：
    //2.1调用glCreateShader创建着色器对象，需要指定是顶点着色器还是片段着色器;
    //2.2调用glShaderSource把内存中的着色器代码与着色器对象绑定;
    //2.3调用glCompileShader编译着色器代码;
    //3.调用glCreateProgram创建着色器程序对象;
    //4.调用glAttachShader绑定顶点着色器对象和片元着色器对象;
    //5.调用glLinkProgram链接着色器程序;
    m_ProgramObj = GLUtils::createProgram("shaders/vs_triangle.glsl", "shaders/fs_triangle.glsl");

    // 设置清屏颜色
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}

void vao_vbo::Draw() {
    //清除屏幕颜色缓冲
    glClear(GL_COLOR_BUFFER_BIT);

    //1.激活着色器程序
    glUseProgram(m_ProgramObj);

    //2.绑定VAO
    glBindVertexArray(VAO);

    //3.绘制三角形
    //第一个参数指定需要绘制的形状时三角形
    //第二个参数指定顶点起始索引
    //第三个参数指定需要绘制的顶点数
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void vao_vbo::Shutdown() {
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(m_ProgramObj);
}
