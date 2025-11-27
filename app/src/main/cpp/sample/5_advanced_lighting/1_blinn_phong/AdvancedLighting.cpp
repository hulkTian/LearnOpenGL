//
// Created by ts on 2024/10/17.
//
/**
 * Blinn-Phong 基于风式光照模型对镜面反射光优化
 *
 * 原有的风式光照模型在计算镜面反射时存在一个问题：
 * 视线与反射方向之间的夹角大于90度，镜面光分量会变为0.0，就会出现异常的高光区。
 * 这在大多数情况下都不是什么问题，因为观察方向离反射方向都非常远。
 * 然而，当物体的反光度非常小时，它产生的镜面高光半径足以让这些相反方向的光线对亮度产生足够大的影响。
 */
#include "AdvancedLighting.h"

REGISTER_SAMPLE(SAMPLE_TYPE_ADVANCED_LIGHTING, AdvancedLighting)
void AdvancedLighting::Create() {
    GLUtils::printGLInfo();

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // build and compile shaders
    // -------------------------
    m_ProgramObj = GLUtils::createProgram("shaders/vs_advanced_lighting.glsl", "shaders/fs_advanced_lighting.glsl");

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
    floorTexture = GLUtils::loadTgaTexture("textures/wood.png", GL_RGBA,
                                           GL_RGBA, GL_UNSIGNED_BYTE,false,
                                           GL_REPEAT, GL_REPEAT,
                                           GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    // shader configuration
    // --------------------
    glUseProgram(m_ProgramObj);
    setInt(m_ProgramObj, "floorTexture", 0);

}

void AdvancedLighting::Draw() {
    //计算每一帧绘制的时间：先记录当前在开始时间
    float currentFrame = TimeUtils::currentTimeSeconds();

    // render
    // ------
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // draw objects
    glUseProgram(m_ProgramObj);
    glm::mat4 projection = glm::perspective(glm::radians(cameraUtils.Zoom), SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = cameraUtils.GetViewMatrix();
    setMat4(m_ProgramObj, "projection", projection);
    setMat4(m_ProgramObj, "view", view);
    // set light uniforms
    setVec3(m_ProgramObj, "viewPos", cameraUtils.Position);
    setVec3(m_ProgramObj, "lightPos", lightPos);
    setInt(m_ProgramObj, "blinn", true);
    // floor
    glBindVertexArray(planeVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, floorTexture);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // 计算每一帧绘制的时间，再计算当前帧结束时间
    deltaTime = TimeUtils::currentTimeSeconds() - currentFrame;
}

void AdvancedLighting::Shutdown() {
    GLBaseSample::Shutdown();
}
