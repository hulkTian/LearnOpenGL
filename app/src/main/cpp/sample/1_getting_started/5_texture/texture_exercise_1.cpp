//
// Created by ts on 2024/12/18.
//

#include "texture_exercise_1.h"
REGISTER_SAMPLE(SAMPLE_TYPE_TEXTURE_EXERCISE_1, texture_exercise_1)

void texture_exercise_1::Create() {
    GLUtils::printGLInfo();

    float  vertices[] = {
            //      ---- 位置 ----          ---- 颜色 ----                   - 纹理坐标 -
            0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
            0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   1.0f, 1.0f
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
                                          "shaders/fs_triangle7.glsl");

    if (!m_ProgramObj) {
        LOGD("Could not create program")
        return;
    }
    //设置清屏颜色
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}

void texture_exercise_1::Draw() {
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

void texture_exercise_1::Shutdown() {
    //关闭顶点属性
    glDeleteVertexArrays(1, &VAO);
    GLBaseSample::Shutdown();
}