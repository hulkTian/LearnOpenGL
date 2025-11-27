// 这个例子使用了纹理相关知识。
// 包括纹理坐标、加载纹理图片、纹理对象生成、配置纹理的环绕和过滤、使用多级渐远纹理。
// 纹理坐标范围和分辨率无关，一般在00到11的范围内；
// 如果想调整图片方向可以变换纹理坐标
// 如果想取纹理的一部分，可以定义纹理坐标范围到想要的区域
// 提供调节纹理坐标和环绕方式和过滤方式可以实现很多效果

#include "texture_unit.h"
REGISTER_SAMPLE(SAMPLE_TYPE_TEXTURE_UNIT, texture_unit)

void texture_unit::Create() {
    GLUtils::printGLInfo();

    float  vertices[] = {
            //      ---- 位置 ----          ---- 颜色 ----                   - 纹理坐标 -
            0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
            0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
    };

    unsigned int indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
    };

    int pointer[] = {3, 3, 2, 0};

    VAO = GLUtils::setUpVAOAndVBO(vertices, sizeof(vertices), indices, sizeof(indices) ,pointer);

    //load texture1
    texture1 = GLUtils::loadTgaTexture("textures/container.jpg");

    //load texture1
    texture2 = GLUtils::loadTgaTexture("textures/awesomeface.png");

    //创建着色器程序,并编译着色器代码
    m_ProgramObj = GLUtils::createProgram("shaders/vs_texture_warp.glsl",
                                          "shaders/fs_triangle7.glsl");

    if (!m_ProgramObj) {
        LOGD("Could not create program")
        return;
    }
    //设置清屏颜色
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}

void texture_unit::Draw() {
    //清除屏幕
    glClear(GL_COLOR_BUFFER_BIT);

    //激活着色器程序
    glUseProgram(m_ProgramObj);

    //指定纹理位置
    glUniform1i(glGetUniformLocation(m_ProgramObj, "texture1"), 0);
    glUniform1i(glGetUniformLocation(m_ProgramObj, "texture2"), 1);

    //激活纹理单元，给绑定的纹理设置一个位置标签，这时第一个纹理，纹理单元值是0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);

    //这时第二个纹理，纹理单元值是1
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    //绑定VAO
    glBindVertexArray(VAO);

    //绘制形状
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void texture_unit::Shutdown() {
    //关闭顶点属性
    glDeleteVertexArrays(1, &VAO);
    GLBaseSample::Shutdown();
}