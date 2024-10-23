//
// Created by ts on 2024/10/23.
//
/**
 * 本节要点：
 * 1. CRT（阴极射线管显示器）设备亮度 = 电压的Gamma次幂，所以输出的亮度是非线性的。
 * 2. 为了保证在线性空间设置的颜色在CRT显示时是线性的，需要进行 Gamma 校正。
 *    思路：先对线性颜色进行CRT设备Gamma倒数次幂运算，颜色值传递给CRT设备后颜色会进行Gamma次幂运算，就会还原成线性空间设置的颜色。
 *    例如：暗红色(0.5,0.0,0.0)，Gamma值是2.2，Gamma的倒数是1/2.2，(0.5,0.0,0.0)的1/2.2次幂是(0.73,0.0,0.0)；
 *    传递给CRT设备后再次进行次幂计算，(0.73,0.0,0.0)的2.2次幂是(0.5,0.0,0.0)。
 * 3. Gamma校正有两种方式：
 *    1）使用OpenGL内建的sRGB帧缓冲。OpenGL ES 没有这种方式
 *    优点：操作简单，调用 glEnable(GL_FRAMEBUFFER_SRGB)就行。
 *    缺点：没有控制权，如果使用多个缓冲帧按照线性方式实现某些特效，将出现错误。
 *    2）自己在片段着色器中进行gamma校正。
 * 4.sRGB纹理需要在加载时恢复到线性颜色空间，否则后面进行Gamma校正就重复校正了。
 *   glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
 * 5.光照衰减是根据线性的物理衰减模型计算的，但是经过 Gamma 校正后，由于颜色值的非线性变化，在视觉上可能会出现光照衰减不符合预期的情况。
 *   解决方法：当使用Gamma 校正时，使用双曲线函数计算衰减强度：float attenuation = 1.0 / (distance);
 *
 */

#include "gamma_correction.h"

// lighting info
// -------------
static glm::vec3 lightPositions[] = {
        glm::vec3(-3.0f, 0.0f, 0.0f),
        glm::vec3(-1.0f, 0.0f, 0.0f),
        glm::vec3 (1.0f, 0.0f, 0.0f),
        glm::vec3 (3.0f, 0.0f, 0.0f)
};
static glm::vec3 lightColors[] = {
        glm::vec3(0.25),
        glm::vec3(0.50),
        glm::vec3(0.75),
        glm::vec3(1.00)
};

void gamma_correction::Create() {
    GLUtils::printGLInfo();

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // build and compile shaders
    // -------------------------
    m_ProgramObj = GLUtils::createProgram("shaders/vs_gamma_correction.glsl", "shaders/fs_gamma_correction.glsl");

    if (!m_ProgramObj) {
        LOGD("Could not create program")
        return;
    }

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float planeVertices[] = {
            // positions            // normals         // texcoords
            10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
            -10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
            -10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,

            10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
            -10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
            10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f
    };
    // plane VAO
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glBindVertexArray(0);

    // load textures
    // -------------
    floorTexture = GLUtils::loadTexture("textures/wood.png", false);
    floorTextureGammaCorrected = GLUtils::loadTexture("textures/wood.png", true);

    // shader configuration
    // --------------------
    glUseProgram(m_ProgramObj);
    setInt(m_ProgramObj, "floorTexture", 0);
}

void gamma_correction::Draw() {
    //计算每一帧绘制的时间：先记录当前在开始时间
    float currentFrame = TimeUtils::currentTimeSeconds();

    // render
    // ------
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // draw objects
    glUseProgram(m_ProgramObj);
    glm::mat4 projection = glm::perspective(glm::radians(cameraUtils.Zoom), m_Width / m_Height, 0.1f, 100.0f);
    glm::mat4 view = cameraUtils.GetViewMatrix();
    setMat4(m_ProgramObj, "projection", projection);
    setMat4(m_ProgramObj, "view", view);
    // set light uniforms
    glUniform3fv(glGetUniformLocation(m_ProgramObj, "lightPositions"), 4, &lightPositions[0][0]);
    glUniform3fv(glGetUniformLocation(m_ProgramObj, "lightColors"), 4, &lightColors[0][0]);
    setVec3(m_ProgramObj, "viewPos", cameraUtils.Position);
    setInt(m_ProgramObj, "gamma", cameraUtils.isEnable);
    // floor
    glBindVertexArray(planeVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, cameraUtils.isEnable ? floorTexture : floorTexture);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // 计算每一帧绘制的时间，再计算当前帧结束时间
    deltaTime = TimeUtils::currentTimeSeconds() - currentFrame;
}

void gamma_correction::Shutdown() {
    glDeleteTextures(1, &floorTexture);
    glDeleteTextures(1, &floorTextureGammaCorrected);
    glDeleteBuffers(1, &planeVAO);
    glDeleteBuffers(1, &planeVBO);
    GLBaseSample::Shutdown();
}
