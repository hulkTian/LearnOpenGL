//
// Created by ts on 2024/12/18.
//
//练习2：重复显示纹理，调大纹理坐标，超出的部分重复采样纹理
#include "texture_exercise_2.h"
REGISTER_SAMPLE(SAMPLE_TYPE_TEXTURE_EXERCISE_2, texture_exercise_2)

void texture_exercise_2::Create() {
    GLUtils::printGLInfo();

    float  vertices[] = {
            //      ---- 位置 ----          ---- 颜色 ----                   - 纹理坐标 -
            0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   2.0f, 2.0f,   // 右上
            0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   2.0f, 0.0f,   // 右下
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 2.0f    // 左上
    };

    unsigned int indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
    };

    int pointer[] = {3, 3, 2, 0};

    VAO = GLUtils::setUpVAOAndVBO(vertices, sizeof(vertices), indices, sizeof(indices) ,pointer);

    //超出部分拉伸纹理边缘像素
    texture1 = GLUtils::loadTgaTexture("textures/container.jpg");

    //超出部分重复采样纹理
    TextureDesc desc2;
    desc2.wrapS = desc2.wrapT = GL_REPEAT;
    texture2 = GLUtils::loadTgaTexture("textures/awesomeface.png", desc2);

    //激活纹理单元，给绑定的纹理设置一个位置标签，这时第一个纹理，纹理单元值是0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);

    //这时第二个纹理，纹理单元值是1
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

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

void texture_exercise_2::Draw() {
    //清除屏幕
    glClear(GL_COLOR_BUFFER_BIT);

    //激活着色器程序
    glUseProgram(m_ProgramObj);

    //指定纹理位置
    setInt(m_ProgramObj, "texture1", 0);
    setInt(m_ProgramObj, "texture2", 1);

    //绑定VAO
    glBindVertexArray(VAO);

    //绘制形状
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void texture_exercise_2::Shutdown() {
    //关闭顶点属性
    glDeleteVertexArrays(1, &VAO);
    glDeleteTextures(1, &texture1);
    glDeleteTextures(1, &texture2);
    GLBaseSample::Shutdown();
}