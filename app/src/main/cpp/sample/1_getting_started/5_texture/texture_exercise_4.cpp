//
// Created by ts on 2024/12/18.
//

#include "texture_exercise_4.h"
REGISTER_SAMPLE(SAMPLE_TYPE_TEXTURE_EXERCISE_4, texture_exercise_4)

void texture_exercise_4::Create() {
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
    texture1 = GLUtils::loadTgaTexture("textures/container.jpg", GL_RGBA, GL_RGBA,GL_UNSIGNED_BYTE,
                                       true, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_LINEAR);

    //load texture1
    texture2 = GLUtils::loadTgaTexture("textures/awesomeface.png", GL_RGBA, GL_RGBA,GL_UNSIGNED_BYTE,
                                       true, GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR);

    //创建着色器程序,并编译着色器代码
    m_ProgramObj = GLUtils::createProgram("shaders/vs_texture_warp.glsl",
                                          "shaders/fs_texture_exercise_4.glsl");

    if (!m_ProgramObj) {
        LOGD("Could not create program")
        return;
    }
    //设置清屏颜色
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}

void texture_exercise_4::Draw() {
    //清除屏幕
    glClear(GL_COLOR_BUFFER_BIT);

    //激活着色器程序
    glUseProgram(m_ProgramObj);

    //指定纹理位置
    glUniform1i(glGetUniformLocation(m_ProgramObj, "texture1"), 0);
    glUniform1i(glGetUniformLocation(m_ProgramObj, "texture2"), 1);

    //将进度值传入到片段着色器，作为纹理混合的参数
    setFloat(m_ProgramObj, "mixValue", 0.01f * static_cast<float>(seek));

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

void texture_exercise_4::Shutdown() {
    //关闭顶点属性
    glDeleteVertexArrays(1, &VAO);
    glDeleteTextures(1, &texture1);
    glDeleteTextures(1, &texture2);
    GLBaseSample::Shutdown();
}