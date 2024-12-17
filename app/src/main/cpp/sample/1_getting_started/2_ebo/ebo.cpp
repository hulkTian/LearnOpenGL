//
// Created by ts on 2024/12/17.
//
///本例用于展示EBO的基本使用流程

#include "ebo.h"

void ebo::Create() {
    // 打印当前设备信息
    GLUtils::printGLInfo();

    float vertices[] = {
            0.5f, 0.5f, 0.0f,   // 右上角
            0.5f, -0.5f, 0.0f,  // 右下角
            -0.5f, -0.5f, 0.0f, // 左下角
            -0.5f, 0.5f, 0.0f   // 左上角
    };

    int indices[] = {
            // 注意索引从0开始!
            // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
            // 这样可以由下标代表顶点组合成矩形
            0, 1, 3, // 第一个三角形
            1, 2, 3  // 第二个三角形
    };

    GLUtils::printGLInfo();

    //顶点数组对象和元素缓冲对象
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // 1. 绑定VAO，后面设置的顶点属性就会存放在这个VAO中了
    glBindVertexArray(VAO);

    // 2.1 把顶点数组复制到缓冲中供OpenGL使用
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 2.2 绑定EBO，缓冲目标类型：GL_ELEMENT_ARRAY_BUFFER
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 3. 配置第0个顶点属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(0);

    //解除绑定VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //创建着色器程序,并编译着色器代码
    m_ProgramObj = GLUtils::createProgram("shaders/vs_rectangle.glsl", "shaders/fs_rectangle.glsl");

    //设置清屏颜色
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}

void ebo::Draw() {
    //清除屏幕颜色缓冲
    glClear(GL_COLOR_BUFFER_BIT);

    //1.激活着色器程序
    glUseProgram(m_ProgramObj);

    //2.绑定VAO，其中也包含了VBO和EBO两个缓冲区的数据
    glBindVertexArray(VAO);

    //3.绘制矩形
    //第一个参数指定需要绘制的图元是三角形
    //第二个参数指定需要绘制的顶点数
    //第三个参数指定索引数据类型
    //第四个参数指定可以指定EBO中的偏移量或不使用EBO传递一个索引数组
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void ebo::Shutdown() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    GLBaseSample::Shutdown();
}