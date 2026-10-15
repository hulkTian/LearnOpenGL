//
// Created by ts on 2024/7/1.
//

#include "CoordinateSystems.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>
REGISTER_SAMPLE(SAMPLE_TYPE_COORDINATE, CoordinateSystems)
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
static unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
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

void CoordinateSystems::Create() {
    GLUtils::printGLInfo();

    int pointer[] = {3, 2, 0};

    VAO = GLUtils::setUpVAOAndVBO(vertices, sizeof(vertices), indices, sizeof(indices) ,pointer);

    //load texture1
    texture1 = GLUtils::loadTgaTexture("textures/container.jpg");

    //load texture1
    texture2 = GLUtils::loadTgaTexture("textures/awesomeface.png");

    //创建着色器程序,并编译着色器代码
    m_ProgramObj = GLUtils::createProgram("shaders/vs_coordinate.glsl",
                                          "shaders/fs_coordinate.glsl");

    if (!m_ProgramObj) {
        LOGD("Could not create program")
        return;
    }

    //开启深度测试
    glEnable(GL_DEPTH_TEST);

    //设置清屏颜色
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
}

void CoordinateSystems::Draw() {
    //清除屏幕和深度缓冲
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

    /**
    * 局部空间 → 世界空间 → 观察空间 → 裁剪空间 → NDC → 屏幕空间 \n
    *   (aPos)    (×M)      (×V)      (×P)   (÷w)  (视口变换)
     */

    /** 
    * 观察矩阵定义了相机的位置和方向，以及相机的视野范围。
    */
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -9.0f));
    setMat4(m_ProgramObj, "view", view);

    /**
     *
     * 投影矩阵定义了相机的视野范围，以及相机的透视效果。
     * 透视投影：相机的视野范围是平截头体，近的物体会放大，远的物体会缩小。
     * fovy：视野角度，单位是弧度。
     * aspect：宽高比，相机的视野范围的宽度与高度的比例。
     * zNear：近平面距离，相机的视野范围的近平面距离。
     * zFar：远平面距离，相机的视野范围的远平面距离。
    */
    glm::mat4 projection = glm::perspective(glm::radians(60.0f), SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
    setMat4(m_ProgramObj, "projection", projection);
    
    /** 
    * 正射投影：相机的视野范围是长方体，远近物体的大小是固定的。
    * 左：左边界，相机的视野范围的左边界。
    * 右：右边界，相机的视野范围的右边界。
    * 下：下边界，相机的视野范围的下边界。
    * 上：上边界，相机的视野范围的上边界。
    * zNear：近平面距离，相机的视野范围的近平面距离。
    * zFar：远平面距离，相机的视野范围的远平面距离。
    */
    glm::mat4 ortho = glm::ortho( -10.0f , 10.0f , -10.0f , 10.0f , 0.1f , 100.0f );
    //setMat4(m_ProgramObj, "projection", ortho);

    for (unsigned int i = 0; i < sizeof(cubePositions) / sizeof(cubePositions[0]); i++) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        model = glm::scale(model, glm::vec3(1.5, 1.5, 1.5));
        /** 
        * 模型矩阵定义了物体的位置和方向，以及物体的大小。
        */
        setMat4(m_ProgramObj, "model", model);

        /** a：物体在世界空间的坐标 */
        glm::vec4 a = model*glm::vec4(-0.5f, -0.5f, -0.5f, 1.0f);
        /** b：物体在观察空间的坐标 */
        glm::vec4 b = view*a;
        /** c：物体在透视投影矩阵裁剪空间的坐标 */
        glm::vec4 c = projection*b;
        /** d：物体在正射投影矩阵裁剪空间的坐标 */
        glm::vec4 d = ortho*b;
        float v1 = a[0];
        float v2 = a[1];
        float v3 = a[2];
        float v4 = a[3];
        LOGD("%f,%f,%f,%f", v1, v2, v3,v4)
        v1 = b[0];
        v2 = b[1];
        v3 = b[2];
        v4 = b[3];
        LOGD("%f,%f,%f,%f", v1, v2, v3,v4)
        v1 = c[0];
        v2 = c[1];
        v3 = c[2];
        v4 = c[3];
        LOGD("%f,%f,%f,%f", v1, v2, v3, v4)
        v1 = c[0]/c[3];
        v2 = c[1]/c[3];
        v3 = c[2]/c[3];
        LOGD("%f,%f,%f", v1, v2, v3)
        v1 = d[0];
        v2 = d[1];
        v3 = d[2];
        v4 = d[3];
        LOGD("%f,%f,%f,%f", v1, v2, v3, v4)

        //绑定VAO
        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

void CoordinateSystems::Shutdown() {
    //关闭顶点属性
    glDeleteVertexArrays(1, &VAO);
    glDeleteTextures(1, &texture1);
    glDeleteTextures(1, &texture2);
    GLBaseSample::Shutdown();
}