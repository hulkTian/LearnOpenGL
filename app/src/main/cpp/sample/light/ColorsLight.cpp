//
// Created by ts on 2024/9/5.
//

#include "ColorsLight.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

// 3D盒子顶点坐标和纹理坐标
// 6个面 x 每个面有2个三角形组成 x 每个三角形有3个顶点
static float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f
};

//定义绘制每个面的顶点顺序
static unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
};

void ColorsLight::Create() {
    GLUtils::printGLInfo();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 顶点数据
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    // 法向量数据
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenVertexArrays(1, &VAO_light);
    glBindVertexArray(VAO_light);
    // 只需要绑定VBO不用再次设置VBO的数据，因为箱子的VBO数据中已经包含了正确的立方体顶点数据
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 设置灯立方体的顶点属性（对我们的灯来说仅仅只有位置数据）
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    //创建着色器程序,并编译着色器代码
    m_ProgramObj = GLUtils::createProgram("shaders/vertex_shader_colors.glsl",
                                          "shaders/fragment_shader_colors.glsl");
    /**
     * 光照练习题：尝试实现一个Gouraud着色（而不是风氏着色）。
     *
     * m_ProgramObj = GLUtils::createProgram("shaders/vertex_shader_colors_gouraud.glsl",
                                          "shaders/fragment_shader_colors_gouraud.glsl");
     */


    //创建光源的着色器程序
    m_ProgramObj_Light = GLUtils::createProgram("shaders/vertex_shader_colors.glsl",
                                                "shaders/fragment_shader_colors_light.glsl");

    if (!m_ProgramObj || !m_ProgramObj_Light) {
        LOGD("Could not create program")
        return;
    }

    //开启深度测试
    glEnable(GL_DEPTH_TEST);

    //设置清屏颜色
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}

void ColorsLight::Draw() {
    //清除屏幕
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //激活着色器程序
    glUseProgram(m_ProgramObj);

    //设置物体颜色
    setVec3(m_ProgramObj, "objectColor", 1.0f, 0.5f, 0.31f);
    setVec3(m_ProgramObj, "lightColor", 1.0f, 1.0f, 1.0f);
    //设置光源位置，用户计算漫反射光
    setVec3(m_ProgramObj, "lightPos", lightPos);
    //设置摄像机位置，用于计算镜面光
    setVec3(m_ProgramObj, "viewPos", camera.Position);

    glm::mat4 model = glm::mat4(1.0f);
    setMat4(m_ProgramObj, "model", model);

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
                                            m_Width / m_Height,
                                            0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    view = glm::rotate(view, glm::radians(20.0f), glm::vec3(1.5f, -0.5f, 0.0f));
    setMat4(m_ProgramObj, "projection", projection);
    setMat4(m_ProgramObj, "view", view);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    //激活光源着色器程序
    glUseProgram(m_ProgramObj_Light);

    // 模型矩阵，把局部空间坐标系中的顶点，变换到世界空间的坐标系中
    model = glm::mat4(1.0f);
    /**
     * 光照练习题：目前，我们的光源是静止的，你可以尝试使用sin或cos函数让光源在场景中来回移动。观察光照随时间的改变能让你更容易理解风氏光照模型。
     * // 定义圆的半径
     * float radius = 20.0f;
     * // 随着时间的变化，遍历圆上的X和Y坐标
     * float camX = sin(TimeUtils::currentTimeSeconds()) * radius;
     * float camZ = cos(TimeUtils::currentTimeSeconds()) * radius;
     * lightPos = glm::vec3 (camX,0.7f,camZ);
     */
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.3f));
    setMat4(m_ProgramObj_Light, "model", model);
    setMat4(m_ProgramObj_Light, "view", view);
    setMat4(m_ProgramObj_Light, "projection", projection);

    //绑定VAO
    glBindVertexArray(VAO_light);

    //绘制形状
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void ColorsLight::Shutdown() {
    //关闭顶点属性
    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &VAO_light);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    // 释放光源的着色器程序指针
    if (m_ProgramObj_Light) {
        glDeleteProgram(m_ProgramObj_Light);
        m_ProgramObj_Light = GL_NONE;
    }
    GLBaseSample::Shutdown();
}