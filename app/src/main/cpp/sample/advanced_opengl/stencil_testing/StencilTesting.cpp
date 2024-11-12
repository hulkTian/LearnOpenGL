//
// Created by ts on 2024/9/29.
//
/**
 * 模板测试：
 * 当片段着色器处理完一个片段之后模板测试(Stencil Test)会开始执行，和深度测试一样，它也可能会丢弃片段。
 *
 * 模板缓冲：
 * 一个模板缓冲中，（通常）每个模板值(Stencil Value)是8位的。所以每个像素/片段一共能有256种不同的模板值。
 *
 * 模板函数
 * glStencilFunc(GLenum func, GLint ref, GLuint mask)：模板缓冲存值内容配置。
 * func：设置模板测试函数。可用的选项有：GL_NEVER、GL_LESS、GL_LEQUAL、GL_GREATER、GL_GEQUAL、GL_EQUAL、GL_NOTEQUAL和GL_ALWAYS。它们的语义和深度缓冲的函数类似。
 * ref：设置了模板测试的参考值(Reference Value)。模板缓冲的内容将会与这个值进行比较。
 * mask：设置一个掩码，它将会与参考值和储存的模板值在测试比较它们之前进行与(AND)运算。初始情况下所有位都为1。
 *
 * glStencilOp(GLenum sfail, GLenum dpfail, GLenum dppass)：模板缓冲更新规则配置。
 * sfail：模板测试失败时采取的行为。
 * dpfail：模板测试通过，但深度测试失败时采取的行为。
 * dppass：模板测试和深度测试都通过时采取的行为。
 *
 * 每个选项都可以选用以下的其中一种行为：
 * GL_KEEP	    保持当前储存的模板值
 * GL_ZERO	    将模板值设置为0
 * GL_REPLACE	将模板值设置为glStencilFunc函数设置的ref值
 * GL_INCR	    如果模板值小于最大值则将模板值加1
 * GL_INCR_WRAP	与GL_INCR一样，但如果模板值超过了最大值则归零
 * GL_DECR	     如果模板值大于最小值则将模板值减1
 * GL_DECR_WRAP	与GL_DECR一样，但如果模板值小于0则将其设置为最大值
 * GL_INVERT	按位翻转当前的模板缓冲值
 */
#include "StencilTesting.h"

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

void StencilTesting::Create() {
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

    // load textures
    // 纹理过滤和环绕参数需要单独设置，地面纹理要使用GL_REPEAT
    cubeTexture = GLUtils::loadTgaTexture("textures/marble.png", false,
                                          GL_REPEAT, GL_REPEAT,
                                          GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    floorTexture = GLUtils::loadTgaTexture("textures/metal.png", false,
                                           GL_REPEAT, GL_REPEAT,
                                           GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    //加载深度测试中的着色器，绘制正常的地面和箱子
    m_ProgramObj = GLUtils::createProgram("shaders/vs_depth_testing.glsl",
                                          "shaders/fs_depth_testing.glsl");
    //加载边框着色器，绘制箱子的边框部分,其实就是按照箱子的轮廓绘制一个纯色的区域
    m_ProgramObj_border = GLUtils::createProgram("shaders/vs_depth_testing.glsl",
                                                "shaders/fs_stencil_testing_color.glsl");

    if (!m_ProgramObj || !m_ProgramObj_border) {
        LOGD("Could not create program")
        return;
    }

    //指定纹理位置
    glUseProgram(m_ProgramObj);
    setInt(m_ProgramObj, "texture1", 0);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

}

void StencilTesting::Draw() {
    //计算每一帧绘制的时间：先记录当前在开始时间
    float currentFrame = TimeUtils::currentTimeSeconds();

    //清除屏幕和深度缓冲，还有模板缓冲
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    //激活着色器程序
    glUseProgram(m_ProgramObj);

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = cameraUtils.GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(cameraUtils.Zoom),
                                            SCR_WIDTH / SCR_WIDTH, 0.1f, 100.0f);
    setMat4(m_ProgramObj, "view", view);
    setMat4(m_ProgramObj, "projection", projection);

    // 绘制地面不保存模板测试缓冲
    glStencilMask(0x00);
    // floor
    glBindVertexArray(planeVAO);
    glBindTexture(GL_TEXTURE_2D, floorTexture);
    setMat4(m_ProgramObj, "model", glm::mat4(1.0f));
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    // 第一步：绘制箱子，保存模板测试缓冲
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);
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

    // 第二步：绘制纯色区域。GL_NOTEQUAL,只保留模板缓冲不相等的片段，即比箱子大的区域缓冲值为0的片段就保留。
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00); // 每一位在写入模板缓冲时都会变成0（禁用写入）
    glDisable(GL_DEPTH_TEST);
    glUseProgram(m_ProgramObj_border);
    setMat4(m_ProgramObj_border, "view", view);
    setMat4(m_ProgramObj_border, "projection", projection);
    float scale = 1.01f;//放大一点纯色区域
    // cubes
    glBindVertexArray(cubeVAO);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
    model = glm::scale(model, glm::vec3(scale, scale, scale));
    setMat4(m_ProgramObj_border, "model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(scale, scale, scale));
    setMat4(m_ProgramObj_border, "model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    //恢复配置
    glBindVertexArray(0);
    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glEnable(GL_DEPTH_TEST);

    // 计算每一帧绘制的时间，再计算当前帧结束时间
    deltaTime = TimeUtils::currentTimeSeconds() - currentFrame;
}

void StencilTesting::Shutdown() {
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &planeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &planeVBO);
    glDeleteTextures(1, &cubeTexture);
    glDeleteTextures(1, &floorTexture);
    glDeleteProgram(m_ProgramObj_border);
    GLBaseSample::Shutdown();
}