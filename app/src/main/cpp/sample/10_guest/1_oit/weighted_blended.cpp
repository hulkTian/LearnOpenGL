//
// Created by ts on 2024/12/12.
//

/**
 * 研究混合技术的原因：没有提供直接的API来渲染透明的界面，在技术上没有提供有效的方式来存储屏幕每一个坐标上不同层的像素。
 * 目前混合技术有两类方法：有序透明和顺序无关透明。
 *
 * 本节是混合技术的拓展，目的是学习不依赖顺序的混合技术实现透明效果。
 * 实现步骤：
 *
 */

#include "weighted_blended.h"

glm::mat4 calculate_model_matrix(const glm::vec3& position, const glm::vec3& rotation = glm::vec3(0.0f), const glm::vec3& scale = glm::vec3(1.0f));

void weighted_blended::Create() {
    cameraUtils = CameraUtils(glm::vec3(0.0f, 0.0f, 5.0f));

    // 加载所有Shader
    m_ProgramObj_Solid = GLUtils::createProgram("shaders/vs_oit_solid.glsl", "shaders/fs_oit_solid.glsl");
    m_ProgramObj_Transparent = GLUtils::createProgram("shaders/vs_oit_transparent.glsl", "shaders/fs_oit_transparent.glsl");
    m_ProgramObj_Composite = GLUtils::createProgram("shaders/vs_oit_composite.glsl", "shaders/fs_oit_composite.glsl");
    m_ProgramObj = GLUtils::createProgram("shaders/vs_oit_screen.glsl", "shaders/fs_oit_screen.glsl");

    glGenFramebuffers(1, &opaqueFBO);
    glGenFramebuffers(1, &transparentFBO);

    glGenTextures(1, &opaqueTexture);
    glBindTexture(GL_TEXTURE_2D, opaqueTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_HALF_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenTextures(1, &depthTexture);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, opaqueFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, opaqueTexture, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        LOGE("ERROR::FRAMEBUFFER:: Opaque framebuffer is not complete!")

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glGenTextures(1, &accumTexture);
    glBindTexture(GL_TEXTURE_2D, accumTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_HALF_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenTextures(1, &revealTexture);
    glBindTexture(GL_TEXTURE_2D, revealTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RED, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, transparentFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, accumTexture, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, revealTexture, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0); // opaque framebuffer's depth texture

    const GLenum transparentDrawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, transparentDrawBuffers);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        LOGE("ERROR::FRAMEBUFFER:: Transparent framebuffer is not complete!")

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    redModelMat = calculate_model_matrix(glm::vec3(0.0f, 0.0f, 1.0f));
    greenModelMat = calculate_model_matrix(glm::vec3(0.0f, 0.0f, 0.0f));
    blueModelMat = calculate_model_matrix(glm::vec3(0.0f, 0.0f, 2.0f));

    zeroFillerVec = glm::vec4(0.0f);
    oneFillerVec = glm::vec4(1.0f);

    CreateFBODebugShader();
}

void weighted_blended::Draw() {
    //计算每一帧绘制的时间：先记录当前在开始时间
    float currentFrame = TimeUtils::currentTimeSeconds();

    glm::mat4 projection = glm::perspective(glm::radians(cameraUtils.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = cameraUtils.GetViewMatrix();
    glm::mat4 vp = projection * view;

    // 启用深度测试
    glEnable(GL_DEPTH_TEST);
    // 设置深度测试比较函数，当配置 GL_LESS时，离摄像机更近的片段会被保留
    glDepthFunc(GL_LESS);
    // 设置深度测试掩码，通过测试的深度值会覆盖缓冲区的深度值
    glDepthMask(GL_TRUE);
    // 关闭混合模式，应为要绘制的是不透明四边形，四边形显示区域内不会只需要保存不透明四边形的颜色
    glDisable(GL_BLEND);

    glBindFramebuffer(GL_FRAMEBUFFER, opaqueFBO);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 激活不透明四边形着色器，并渲染
    glUseProgram(m_ProgramObj_Solid);
    // 整合模型矩阵、视图矩阵和投影矩阵
    setMat4(m_ProgramObj_Solid, "mvp", vp * redModelMat);
    setVec3(m_ProgramObj_Solid, "color", glm::vec3(1.0f, 0.0f, 0.0f));
    renderQuad();

    // debug深度贴图
    //DisplayFramebufferTexture(depthTexture, true);
    // debug颜色贴图
    //DisplayFramebufferTexture(opaqueTexture);

    // 设置深度测试掩码，测试通过不会覆盖缓冲区，但是会丢弃被遮挡的片段
    glDepthMask(GL_FALSE);
    // 启用混合，半透明四边形显示区域和透出不透明四边形显示区域重叠的位置会显示混合颜色
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glBlendFunci(0, GL_ONE, GL_ONE);
    //glBlendFunci(1, GL_ZERO, GL_ONE_MINUS_SRC_COLOR);
    glBlendEquation(GL_FUNC_ADD);

    glBindFramebuffer(GL_FRAMEBUFFER, transparentFBO);
    glClearBufferfv(GL_COLOR, 0, &zeroFillerVec[0]);
    glClearBufferfv(GL_COLOR, 1, &oneFillerVec[0]);

    // 渲染半透明的绿色四边形和蓝色四边形
    glUseProgram(m_ProgramObj_Transparent);

    setMat4(m_ProgramObj_Transparent, "mvp", vp * greenModelMat);
    setVec4(m_ProgramObj_Transparent, "color", glm::vec4(0.0f, 1.0f, 0.0f, 0.5f));
    renderQuad();

    setMat4(m_ProgramObj_Transparent, "mvp", vp * blueModelMat);
    setVec4(m_ProgramObj_Transparent, "color", glm::vec4(0.0f, 0.0f, 1.0f, 0.5f));
    renderQuad();

    glDepthFunc(GL_ALWAYS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindFramebuffer(GL_FRAMEBUFFER, opaqueFBO);

    glUseProgram(m_ProgramObj_Composite);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, accumTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, revealTexture);
    renderQuad();

    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE); // enable depth writes so glClear won't ignore clearing the depth buffer
    glDisable(GL_BLEND);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glUseProgram(m_ProgramObj);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, opaqueTexture);
    renderQuad();

    // 计算每一帧绘制的时间，再计算当前帧结束时间
    deltaTime = TimeUtils::currentTimeSeconds() - currentFrame;
}

glm::mat4 calculate_model_matrix(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) {
    glm::mat4 trans = glm::mat4(1.0f);

    trans = glm::translate(trans, position);
    trans = glm::rotate(trans, glm::radians(rotation.x), glm::vec3(1.0, 0.0, 0.0));
    trans = glm::rotate(trans, glm::radians(rotation.y), glm::vec3(0.0, 1.0, 0.0));
    trans = glm::rotate(trans, glm::radians(rotation.z), glm::vec3(0.0, 0.0, 1.0));
    trans = glm::scale(trans, scale);

    return trans;
}