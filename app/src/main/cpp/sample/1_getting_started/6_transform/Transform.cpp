//
// Created by 田浩 on 2024/6/16.
//
// 组合矩阵需要按照从右到左的方式组合矩阵，并且组合式要先缩放再旋转最后位移

#include "Transform.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"



void Transform::Create() {
    GLUtils::printGLInfo();

    float vertices[] = {
//               ---- 位置 ----         - 纹理坐标 -
            0.5f, 0.5f, 0.0f, 1.0f, 1.0f,   // 右上
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f,   // 右下
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,   // 左下
            -0.5f, 0.5f, 0.0f, 0.0f, 1.0f    // 左上
    };
    unsigned int indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
    };

    int pointer[] = {3, 2, 0};

    VAO = GLUtils::setUpVAOAndVBO(vertices, sizeof(vertices), indices, sizeof(indices) ,pointer);

    //load texture1
    texture1 = GLUtils::loadTgaTexture("textures/container.jpg");

    //load texture1
    texture2 = GLUtils::loadTgaTexture("textures/awesomeface.png");

    //创建着色器程序,并编译着色器代码
    m_ProgramObj = GLUtils::createProgram("shaders/vs_transform.glsl",
                                          "shaders/fs_transform.glsl");

    if (!m_ProgramObj) {
        LOGD("Could not create program")
        return;
    }

    //设置清屏颜色
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}

void Transform::Draw() {
    //清除屏幕
    glClear(GL_COLOR_BUFFER_BIT);

    //激活着色器程序
    glUseProgram(m_ProgramObj);

    //指定纹理位置
    setInt(m_ProgramObj, "texture1", 0);
    setInt(m_ProgramObj, "texture2", 1);

    //激活纹理单元，给绑定的纹理设置一个位置标签，这时第一个纹理，纹理单元值是0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);

    //这时第二个纹理，纹理单元值是1
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glm::mat4 trans = glm::mat4(1.0f);
    /*trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
    trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));*/
    float time = TimeUtils::currentTimeSeconds();
    trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
    trans = glm::rotate(trans, time, glm::vec3(0.0f, 0.0f, 1.0f));
    setMat4(m_ProgramObj, "transform", trans);

    //绑定VAO
    glBindVertexArray(VAO);

    //绘制形状
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    trans = glm::mat4(1.0f);
    auto scale = abs(static_cast<float>(sin(time)));
    trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
    trans = glm::scale(trans, glm::vec3(scale, scale, 1.0f));
    setMat4(m_ProgramObj, "transform", trans);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Transform::Shutdown() {
    //关闭顶点属性
    glDeleteVertexArrays(1, &VAO);
    GLBaseSample::Shutdown();
}
