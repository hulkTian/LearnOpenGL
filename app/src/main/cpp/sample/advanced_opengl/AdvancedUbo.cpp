//
// Created by ts on 2024/10/12.
//

/**
 * Uniform缓冲对象(Uniform Buffer Object)：
 * OpenGL提供的缓冲工具，可以使用glGenBuffers来创建它，将它绑定到GL_UNIFORM_BUFFER缓冲目标，并将所有相关的uniform数据存入缓冲。
 *
 * Uniform块布局：
 * layout (std140) uniform ExampleBlock
 * {
                     // 基准对齐量       // 对齐偏移量
    float value;     // 4               // 0
    vec3 vector;     // 16              // 16  (必须是16的倍数，所以 4->16)
    mat4 matrix;     // 16              // 32  (列 0)
                     // 16              // 48  (列 1)
                     // 16              // 64  (列 2)
                     // 16              // 80  (列 3)
    float values[3]; // 16              // 96  (values[0])
                     // 16              // 112 (values[1])
                     // 16              // 128 (values[2])
    bool boolean;    // 4               // 144
    int integer;     // 4               // 148
    };
 * std140布局定义了对齐偏移量标准，方便我们手动填充缓冲数据
 *
 * 绑定点(Binding Point)：
 * 在OpenGL上下文中，定义了一些绑定点，我们可以将一个Uniform缓冲链接至它。
 *
 * Uniform块索引(Uniform Block Index)：
 * 是着色器中已定义Uniform块的位置值索引。可以通过调用 glGetUniformBlockIndex 来获取。
 *
 * 如何才能让OpenGL知道哪个Uniform缓冲对应的是哪个Uniform块呢？
 * 1. 在创建Uniform缓冲之后，我们将它绑定到其中一个绑定点上。可以通过调用 glBindBufferBase 或 glBindBufferRange 来绑定。
 * 2. 将着色器中的Uniform块绑定到相同的绑定点，可以通过调用 glUniformBlockBinding 来绑定。
 *
 */

#include "AdvancedUbo.h"
#include "glm/gtc/type_ptr.hpp"

static float cubeVertices[] = {
        // positions
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,

        -0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,

        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,

        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f,
};

void AdvancedUbo::Create() {
    GLUtils::printGLInfo();

    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);

    // build and compile shaders
    // -------------------------
    m_program_red = GLUtils::createProgram("shaders/vs_advanced_glsl_ubo.glsl",
                                           "shaders/fs_advanced_glsl_red.glsl");
    m_program_green = GLUtils::createProgram("shaders/vs_advanced_glsl_ubo.glsl",
                                             "shaders/fs_advanced_glsl_green.glsl");
    m_program_blue = GLUtils::createProgram("shaders/vs_advanced_glsl_ubo.glsl",
                                            "shaders/fs_advanced_glsl_blue.glsl");
    m_program_yellow = GLUtils::createProgram("shaders/vs_advanced_glsl_ubo.glsl",
                                              "shaders/fs_advanced_glsl_yellow.glsl");

    if (!m_program_red) {
        LOGD("Could not create program")
        return;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // configure a uniform buffer object
    // ---------------------------------
    // first. We get the relevant block indices
    unsigned int uniformBlockIndexRed = glGetUniformBlockIndex(m_program_red, "Matrices");
    unsigned int uniformBlockIndexGreen = glGetUniformBlockIndex(m_program_green, "Matrices");
    unsigned int uniformBlockIndexBlue = glGetUniformBlockIndex(m_program_blue, "Matrices");
    unsigned int uniformBlockIndexYellow = glGetUniformBlockIndex(m_program_yellow, "Matrices");
    // then we link each shader's uniform block to this uniform binding point
    glUniformBlockBinding(m_program_red, uniformBlockIndexRed, 1);
    glUniformBlockBinding(m_program_green, uniformBlockIndexGreen, 1);
    glUniformBlockBinding(m_program_blue, uniformBlockIndexBlue, 1);
    glUniformBlockBinding(m_program_yellow, uniformBlockIndexYellow, 1);
    // Now actually create the buffer
    glGenBuffers(1, &uboMatrices);
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    // define the range of the buffer that links to a uniform binding point
    // glBindBufferBase(GL_UNIFORM_BUFFER, 1, uboMatrices);
    glBindBufferRange(GL_UNIFORM_BUFFER, 1, uboMatrices, 0, 2 * sizeof(glm::mat4));

    // store the projection matrix (we only do this once now) (note: we're not using zoom anymore by changing the FoV)
    glm::mat4 projection = glm::perspective(45.0f, 2208.0f / 1682.0f, 0.1f, 100.0f);
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void AdvancedUbo::Draw() {
    //计算每一帧绘制的时间：先记录当前在开始时间
    float currentFrame = TimeUtils::currentTimeSeconds();

    //清除屏幕和深度缓冲，还有模板缓冲
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set the view and projection matrix in the uniform block - we only have to do this once per loop iteration.
    glm::mat4 view = cameraUtils.GetViewMatrix();
    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // draw 4 cubes
    // RED
    glBindVertexArray(cubeVAO);
    glUseProgram(m_program_red);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-0.75f, 0.75f, 0.0f)); // move top-left
    setMat4(m_program_red, "model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    // GREEN
    glUseProgram(m_program_green);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.75f, 0.75f, 0.0f)); // move top-right
    setMat4(m_program_green, "model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    // YELLOW
    glUseProgram(m_program_yellow);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-0.75f, -0.75f, 0.0f)); // move bottom-left
    setMat4(m_program_yellow, "model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    // BLUE
    glUseProgram(m_program_blue);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.75f, -0.75f, 0.0f)); // move bottom-right
    setMat4(m_program_blue, "model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // 计算每一帧绘制的时间，再计算当前帧结束时间
    deltaTime = TimeUtils::currentTimeSeconds() - currentFrame;
}

void AdvancedUbo::Shutdown() {
    glDeleteBuffers(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &uboMatrices);
    glDeleteProgram(m_program_red);
    glDeleteProgram(m_program_green);
    glDeleteProgram(m_program_blue);
    glDeleteProgram(m_program_yellow);
    GLBaseSample::Shutdown();
}
