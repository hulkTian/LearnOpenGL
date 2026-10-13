//
// Created by ts on 2024/12/17.
//
//本例用于展示EBO的基本使用流程

/** 
 * GL_ARRAY_BUFFER和GL_ELEMENT_ARRAY_BUFFER不同点:
 * 1. ARRAY_BUFFER对应的时顶点属性，ELEMENT_ARRAY_BUFFER对应的时索引属性
 * 2. ARRAY_BUFFER在调用glVertexAttribPointer后算记录完成并可以释放了，
 *    ELEMENT_ARRAY_BUFFER在调用glBindBuffer后算记录完成，但只有在VAO被解绑后才能释放
*/ 

#include "ebo.h"
REGISTER_SAMPLE(SAMPLE_TYPE_EBO, ebo)

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

    /**  1. 创建VAO，记录顶点属性配置 */
    glGenVertexArrays(1, &VAO);
    /**  2. 创建VBO，缓冲顶点数据 */
    glGenBuffers(1, &VBO);
    /**  3. 创建EBO，缓冲索引数据 */
    glGenBuffers(1, &EBO);
    /**  4. 绑定VAO，告诉OpenGL当前VAO正在使用 */
    glBindVertexArray(VAO);
    /**  5. 绑定VBO，告诉OpenGL这个VBO是顶点数据缓冲区 */
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    /**  6. 把顶点数组复制到缓冲中供OpenGL使用 */
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    /**  7. 绑定EBO，告诉OpenGL这个EBO是索引数据缓冲区 */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    /**  8. 把索引数组复制到缓冲中供OpenGL使用 */
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    /**  9. 配置第0个顶点属性 */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);
    /**  10. 启用第0个顶点属性 */
    glEnableVertexAttribArray(0);
    /** 11. 解除绑定VBO，ARRAY_BUFFER槽位为空闲 */
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    /** 13. 解除绑定VAO，绑定到默认VAO */
    glBindVertexArray(0);
    /** 14. 解除绑定EBO，ELEMENT_ARRAY_BUFFER槽位为空闲 */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    /** 创建着色器程序,并编译着色器代码 */
    m_ProgramObj = GLUtils::createProgram("shaders/vs_rectangle.glsl", "shaders/fs_rectangle.glsl");

    /** 设置清屏颜色 */
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}

void ebo::Draw() {
    /** 清除颜色缓冲区，设置为设置的清屏颜色 */
    glClear(GL_COLOR_BUFFER_BIT);

    /**  1. 激活着色器程序 */
    glUseProgram(m_ProgramObj);

    /**  2. 绑定VAO，其中也包含了VBO和EBO两个缓冲区的数据 */
    glBindVertexArray(VAO);

    /** 3.绘制矩形\n
     * 第一个参数指定需要绘制的图元是三角形\n
     * 第二个参数指定索引数组的长度\n
     * 第三个参数指定索引数据类型，这里是无符号整数\n
     * 第四个参数指定可以指定EBO中的偏移量或不使用EBO传递一个索引数组
    */
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void ebo::Shutdown() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    GLBaseSample::Shutdown();
}