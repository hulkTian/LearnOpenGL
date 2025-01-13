//
// Created by ts on 2024/9/30.
//
/**
 * 丢弃片段：
 * 有些图片并不需要半透明，只需要根据纹理颜色值，显示一部分，或者不显示一部分，没有中间情况。
 * GLSL给了我们discard命令，一旦被调用，它就会保证片段不会被进一步处理，所以就不会进入颜色缓冲。
 *
 * 混合:
 * 启用纹理：glEnable(GL_BLEND);
 * 混合公式：Vec_result= Vec_source ∗ F_source + Vec_destination ∗ F_destination
 * Vec_source: 源颜色向量。这是源自纹理的颜色向量。
 * Vec_destination: 目标颜色向量。这是当前储存在颜色缓冲中的颜色向量。
 * F_source: 源因子值。指定了alpha值对源颜色的影响。
 * F_destination: 目标因子值。指定了alpha值对目标颜色的影响。
 *
 * 混合函数：
 * glBlendFunc(GLenum sfactor, GLenum dfactor)
 * sfactor：                     源因子
 * dfactor：                    目标因子
 * 可选项：
 * GL_ZERO	                    因子等于0
 * GL_ONE	                    因子等于1
 * GL_SRC_COLOR	                因子等于源颜色向量
 * GL_ONE_MINUS_SRC_COLOR	    因子等于1−Vec_source
 * GL_DST_COLOR	                因子等于目标颜色向量
 * GL_ONE_MINUS_DST_COLOR	    因子等于1−Vec_destination
 * GL_SRC_ALPHA	                因子等于Vec_source的alpha分量
 * GL_ONE_MINUS_SRC_ALPHA	    因子等于1−Vec_source的alpha分量
 * GL_DST_ALPHA	                因子等于Vec_destination的alpha分量
 * GL_ONE_MINUS_DST_ALPHA	    因子等于1−Vec_destination的alpha分量
 * GL_CONSTANT_COLOR	        因子等于常数颜色向量
 * GL_ONE_MINUS_CONSTANT_COLOR	因子等于1−Vec_constant
 * GL_CONSTANT_ALPHA	        因子等于Vec_constant的alpha分量
 * GL_ONE_MINUS_CONSTANT_ALPHA	因子等于1−Vec_constant的alpha分量
 *
 * 也可以使用glBlendFuncSeparate为RGB和alpha通道分别设置不同的混合因子选项：
 * glBlendFuncSeparate(GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
 *
 *
 * glBlendEquation(GLenum mode)允许我们设置运算符，它提供了三个选项：
 * GL_FUNC_ADD：默认选项，将两个分量相加：Vec_result=Src+Dst
 * GL_FUNC_SUBTRACT：将两个分量相减： Vec_result=Src−Dst
 * GL_FUNC_REVERSE_SUBTRACT：将两个分量相减，但顺序相反：Vec_result=Dst−Src
 *
 * 当绘制一个有不透明和透明物体的场景的时候，大体的原则如下：
 * 1.先绘制所有不透明的物体。
 * 2.对所有透明的物体排序。
 * 3.按顺序绘制所有透明的物体。
 */

#include "BlendingDiscard.h"
#include <vector>
#include <map>

static float cubeVertices[] = {
        // positions          // texture Coords
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
static float planeVertices[] = {
        // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
        5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
        -5.0f, -0.5f, 5.0f, 0.0f, 0.0f,
        -5.0f, -0.5f, -5.0f, 0.0f, 2.0f,

        5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f, 0.0f, 2.0f,
        5.0f, -0.5f, -5.0f, 2.0f, 2.0f
};

static float transparentVertices[] = {
        // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f,
        0.0f, -0.5f, 0.0f, 0.0f, 1.0f,
        1.0f, -0.5f, 0.0f, 1.0f, 1.0f,

        0.0f, 0.5f, 0.0f, 0.0f, 0.0f,
        1.0f, -0.5f, 0.0f, 1.0f, 1.0f,
        1.0f, 0.5f, 0.0f, 1.0f, 0.0f
};

// transparent vegetation locations
// --------------------------------
static std::vector<glm::vec3> vegetation = {
        glm::vec3(-1.5f, 0.0f, -0.48f),
        glm::vec3(1.5f, 0.0f, 0.51f),
        glm::vec3(0.0f, 0.0f, 0.7f),
        glm::vec3(-0.3f, 0.0f, -2.3f),
        glm::vec3(0.5f, 0.0f, -0.6f)};


void BlendingDiscard::Create() {
    GLUtils::printGLInfo();

    // cube
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *) (3 * sizeof(float)));
    glBindVertexArray(0);
    // plane
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *) (3 * sizeof(float)));
    glBindVertexArray(0);
    // transparent VAO
    glGenVertexArrays(1, &transparentVAO);
    glGenBuffers(1, &transparentVBO);
    glBindVertexArray(transparentVAO);
    glBindBuffer(GL_ARRAY_BUFFER, transparentVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), transparentVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *) (3 * sizeof(float)));
    glBindVertexArray(0);

    // load textures
    // 纹理过滤和环绕参数需要单独设置，地面纹理要使用GL_REPEAT，草的纹理需要使用GL_CLAMP_TO_EDGE
    cubeTexture = GLUtils::loadTgaTexture("textures/marble.png", GL_RGBA,
                                          GL_RGBA, GL_UNSIGNED_BYTE,false,
                                          GL_REPEAT, GL_REPEAT,
                                          GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    floorTexture = GLUtils::loadTgaTexture("textures/metal.png", GL_RGBA,
                                           GL_RGBA, GL_UNSIGNED_BYTE,false,
                                           GL_REPEAT, GL_REPEAT,
                                           GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    /*transparentTexture = GLUtils::loadTgaTexture("textures/grass.png", false,
                                                 GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE,
                                                 GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);*/
    transparentTexture = GLUtils::loadTgaTexture("textures/blending_transparent_window.png",
                                                 GL_RGBA,GL_RGBA, GL_UNSIGNED_BYTE,false,
                                                 GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE,
                                                 GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    //创建着色器程序,并编译着色器代码
    m_ProgramObj = GLUtils::createProgram("shaders/vs_blending.glsl",
                                          "shaders/fs_blending.glsl");

    if (!m_ProgramObj) {
        LOGD("Could not create program")
        return;
    }

    //指定纹理位置
    glUseProgram(m_ProgramObj);
    setInt(m_ProgramObj, "texture1", 0);

    // 开启深度测试
    glEnable(GL_DEPTH_TEST);

    // 开启混合
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //设置清屏颜色
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}

void BlendingDiscard::Draw() {
    //计算每一帧绘制的时间：先记录当前在开始时间
    float currentFrame = TimeUtils::currentTimeSeconds();

    // sort the transparent windows before rendering
    // ---------------------------------------------
    std::map<float, glm::vec3> sorted;
    for (unsigned int i = 0; i < vegetation.size(); i++) {
        float distance = glm::length(cameraUtils.Position - vegetation[i]);
        sorted[distance] = vegetation[i];
    }

    //清除屏幕和深度缓冲
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //激活着色器程序
    glUseProgram(m_ProgramObj);

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = cameraUtils.GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(cameraUtils.Zoom),
                                            SCR_WIDTH / SCR_WIDTH, 0.1f, 100.0f);
    setMat4(m_ProgramObj, "view", view);
    setMat4(m_ProgramObj, "projection", projection);

    // cubes
    glBindVertexArray(cubeVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, cubeTexture);
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
    setMat4(m_ProgramObj, "model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
    setMat4(m_ProgramObj, "model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    // floor
    glBindVertexArray(planeVAO);
    glBindTexture(GL_TEXTURE_2D, floorTexture);
    setMat4(m_ProgramObj, "model", glm::mat4(1.0f));
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    // vegetation
    glBindVertexArray(transparentVAO);
    glBindTexture(GL_TEXTURE_2D, transparentTexture);
    // 根据窗户的深度顺序绘制半透明的窗户，避免由于先绘制深度值较小的纹理，而丢弃深度值较大的片段
    for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin();
         it != sorted.rend(); ++it) {
        model = glm::mat4(1.0f);
        model = glm::translate(model, it->second);
        setMat4(m_ProgramObj, "model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    // 计算每一帧绘制的时间，再计算当前帧结束时间
    deltaTime = TimeUtils::currentTimeSeconds() - currentFrame;
}

void BlendingDiscard::Shutdown() {
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &planeVAO);
    glDeleteBuffers(1, &transparentVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &planeVBO);
    glDeleteBuffers(1, &transparentVBO);
    glDeleteTextures(1, &cubeTexture);
    glDeleteTextures(1, &floorTexture);
    glDeleteTextures(1, &transparentTexture);
    GLBaseSample::Shutdown();
}