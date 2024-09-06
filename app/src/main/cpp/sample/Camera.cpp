//
// Created by ts on 2024/7/3.
// LookAt矩阵可以构建出想要的观察矩阵，即把世界坐标系中的顶点，以摄像机的位置作为原点构建的观察坐标系。
// lookAt函数需要三个参数：摄像机位置向量，摄像机方向向量，向上的轴的向量。

//
//
//
/**
 * 构建坐标系需要四要素：原点、Z轴、X轴和Y轴。
 * 对应的构建摄像机的观察坐标系，需要：
 * 1. 摄像机位置，即原点；
 * 2. 摄像机方向，即摄像机坐标的z轴的正方向；
 * 3. 摄像机右轴，即摄像机空间的x轴的正方向；
 * 4. 摄像机上轴，即摄像机空间的y轴的正方向；
 *
 * 小技巧：定义一个上向量并和摄像机方向进行叉乘就可得到右向量，同理，把右向量和方向向量进行叉乘得到上轴。
 *
 * LookAt矩阵：3个相互垂直的轴和一个定义摄像机空间的位置坐标。注意，位置向量是相反的，因为我们最终希望把世界平移到与我们自身移动的相反方向。
 *
 *    ⎢Rx Ry Rz 0⎢ ⎢1 0 0 -Px⎢
 *    ⎢Ux Uy Uz 0⎢ ⎢1 1 0 -Py⎢
 *    ⎢Dx Dy Dz 0⎢ ⎢1 0 1 -Pz⎢
 *    ⎢0  0  0  1⎢ ⎢1 0 0  1 ⎢
 *
 */

#include "Camera.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

// 3D盒子顶点坐标和纹理坐标
// 6个面 x 每个面有2个三角形组成 x 每个三角形有3个顶点
static float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
};

static glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)
};

void Camera::Create() {
    GLUtils::printGLInfo();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //load texture1
    texture1 = GLUtils::loadTgaTexture("textures/container.jpg");

    //load texture1
    texture2 = GLUtils::loadTgaTexture("textures/awesomeface.png");

    //创建着色器程序,并编译着色器代码
    m_ProgramObj = GLUtils::createProgram("shaders/vertex_shader_coordinate.glsl",
                                          "shaders/fragment_shader_coordinate.glsl");

    if (!m_ProgramObj) {
        LOGD("Could not create program")
        return;
    }

    //开启深度测试
    glEnable(GL_DEPTH_TEST);

    //设置清屏颜色
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}

void Camera::Draw() {
    //清除屏幕和深度缓冲
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

    // 定义圆的半径
    float radius = 20.0f;
    // 随着时间的变化，遍历圆上的X和Y坐标
    float camX = sin(TimeUtils::currentTimeSeconds()) * radius;
    float camZ = cos(TimeUtils::currentTimeSeconds()) * radius;
    // 使用gml提供的函数，构建一个移动的LookAt矩阵：
    // 第一个参数：摄像机坐标，随时间变化，在圆上移动；第二个参数：摄像机观察目标的位置；第三个参数：世界空间中的上向量；
    glm::mat4 view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
    setMat4(m_ProgramObj, "view", view);

    // 透视投影矩阵
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(60.0f), m_Width / m_Height, 0.01f, 100.0f);
    setMat4(m_ProgramObj, "projection", projection);

    for (unsigned int i = 0; i < 10; i++) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        setMat4(m_ProgramObj, "model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

void Camera::Shutdown() {
    //关闭顶点属性
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    GLBaseSample::Shutdown();
}