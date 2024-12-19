//
// Created by ts on 2024/12/18.
//

#include "texture_warp.h"

void texture_warp::Create() {
    GLUtils::printGLInfo();

    float vertices[] = {
//        ---- 位置 ----      ---- 颜色 ----       - 纹理坐标 -
            0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 5.0f, 5.0f,   // 右上
            0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 5.0f, 0.0f,   // 右下
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,   // 左下
            -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 5.0f    // 左上
    };

    unsigned int indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
    };

    int pointer[] = {3, 3, 2};

    VAO = GLUtils::setUpVAOAndVBO(vertices, sizeof(vertices), indices, sizeof(indices) ,pointer);

    //load texture1
    // 设置环绕方式为：GL_REPEAT
    /*ourTexture = GLUtils::loadTgaTexture("textures/container.jpg", GL_RGB, GL_RGB,GL_UNSIGNED_BYTE,
                                         true, GL_REPEAT, GL_REPEAT,GL_NEAREST,GL_LINEAR);*/
    // 设置环绕方式为：GL_MIRRORED_REPEAT
    ourTexture = GLUtils::loadTgaTexture("textures/container.jpg", GL_RGB, GL_RGB,GL_UNSIGNED_BYTE,
                                         true, GL_MIRRORED_REPEAT, GL_MIRRORED_REPEAT, GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR);
    // 设置环绕方式为：GL_CLAMP_TO_EDGE
    /*ourTexture = GLUtils::loadTgaTexture("textures/container.jpg", GL_RGB, GL_RGB,GL_UNSIGNED_BYTE,
                                         true, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_LINEAR);*/
    // 设置环绕方式为：GL_CLAMP_TO_EDGE
    /*ourTexture = GLUtils::loadTgaTexture("textures/container.jpg", GL_RGB, GL_RGB,GL_UNSIGNED_BYTE,
                                         true, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER,GL_NEAREST, GL_LINEAR);*/

    //创建着色器程序,并编译着色器代码
    m_ProgramObj = GLUtils::createProgram("shaders/vs_texture_warp.glsl", "shaders/fs_texture_warp.glsl");

    if (!m_ProgramObj) {
        LOGD("Could not create program")
        return;
    }

    glUseProgram(m_ProgramObj);
    setInt(m_ProgramObj, "ourTexture", 0);

    //设置清屏颜色
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}

void texture_warp::Draw() {
    //清除屏幕
    glClear(GL_COLOR_BUFFER_BIT);

    //激活着色器程序
    glUseProgram(m_ProgramObj);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ourTexture);

    //绑定VAO
    glBindVertexArray(VAO);

    //绘制形状
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void texture_warp::Shutdown() {
    glDeleteVertexArrays(1, &VAO);
    GLBaseSample::Shutdown();
}