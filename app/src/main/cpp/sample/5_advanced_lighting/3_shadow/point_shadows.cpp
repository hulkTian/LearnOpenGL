//
// Created by tzh on 2025/12/30.
//

/**
 * 点光源阴影的实现
 * 1. 使用立方体深度贴图存储点光源的深度信息;
 * 1) 创建立方体深度贴图和帧缓冲对象
 * 2) 渲染场景到立方体深度贴图，使用几何着色器一次生成六个视图
 *
 * 2. 渲染场景时，使用立方体深度贴图进行阴影计算
 * 1) 在片段着色器中，计算片段到点光源的距离
 * 2) 从立方体深度贴图中采样对应方向的深度值
 * 3) 比较片段到光源的距离和采样到的深度值，判断是否在阴影中
 *
 * 3. 优化阴影效果
 * 1) 使用偏移值减少阴影伪影
 * 2) 使用PCF（Percentage Closer Filtering）技术平滑阴影边缘
 */

#include "point_shadows.h"

REGISTER_SAMPLE(SAMPLE_TYPE_POINT_SHADOWS, point_shadows)

void point_shadows::Create() {
    GLUtils::printGLInfo();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    m_ProgramObj = GLUtils::createProgram("shaders/vs_shadow_mapping_point.glsl",
                                          "shaders/fs_shadow_mapping_point.glsl");
    // 深度着色器，使用几何着色器将世界空间顶点位置转换到六个方向
    simpleDepthShader = GLUtils::createProgram("shaders/vs_shadow_mapping_depth_point.glsl",
                                               "shaders/fs_shadow_mapping_depth_point.glsl",
                                               "shaders/gs_shadow_mapping_depth_point.glsl");

    if (!m_ProgramObj || !simpleDepthShader) {
        LOGD("Could not create program")
        return;
    }

    woodTexture = GLUtils::loadTexture("textures/wood.png", false);

    glGenFramebuffers(1, &depthMapFBO);
    // 创建立方体深度贴图
    glGenTextures(1, &depthCubeMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);
    for (unsigned int i = 0; i < 6; ++i) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
                     GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT,
                     0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, nullptr);
        GLUtils::checkGlError("glTexImage2D");
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // 附加深度纹理到帧缓冲对象
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubeMap, 0);
    GLUtils::checkGlError("glFramebufferTexture");
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // 检查帧缓冲是否完整
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        LOGE("ERROR::FRAMEBUFFER:: Framebuffer is not complete!")
        return;
    }

    glUseProgram(m_ProgramObj);
    setInt(m_ProgramObj, "diffuseTexture", 0);
    setInt(m_ProgramObj, "depthMap", 1);

    glUseProgram(debugDepthQuad);
    setInt(debugDepthQuad, "depthMap", 0);
}

void point_shadows::Draw() {
    //计算每一帧绘制的时间：先记录当前在开始时间
    float currentFrame = TimeUtils::currentTimeSeconds();

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 动态移动光源位置
    //lightPos.z = sin(currentFrame * 0.5) * 3.0f;

    // 0. 配置立方体深度贴图的视图和投影矩阵
    // ----------------------------------------------

    glm::mat4 shadowProj = glm::perspective(90.0f, aspect, near_plane, far_plane);
    // 计算六个视图矩阵并乘以投影矩阵，得到最终的变换矩阵
    std::vector<glm::mat4> shadowTransforms;
    // 修改 shadowTransforms 的生成逻辑
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
    // 1. 渲染场景到立方体深度贴图
    // ----------------------------------------------
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    glUseProgram(simpleDepthShader);
    for (GLuint i = 0; i < 6; ++i) {
        setMat4(simpleDepthShader, "shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
    }
    setFloat(simpleDepthShader, "far_plane", far_plane);
    setVec3(simpleDepthShader, "lightPos", lightPos);
    renderScene(simpleDepthShader);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    /*glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glUseProgram(simpleDepthShader);
    setFloat(simpleDepthShader, "far_plane", far_plane);
    setVec3(simpleDepthShader, "lightPos", lightPos);

    for (unsigned int i = 0; i < 6; ++i) {
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                               GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, depthCubeMap, 0);
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            LOGE("FBO incomplete for face %d!", i)
        }
        glClear(GL_DEPTH_BUFFER_BIT);
        setMat4(simpleDepthShader, "shadowMatrix", shadowTransforms[i]);
        renderScene(simpleDepthShader);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);*/

    // 2. 渲染场景
    // ----------------------------------------------
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(m_ProgramObj);
    glm::mat4 projection = glm::perspective(glm::radians(cameraUtils.Zoom),
                                            (float) SCR_WIDTH / (float) SCR_HEIGHT,
                                            0.1f, 100.0f);
    glm::mat4 view = cameraUtils.GetViewMatrix();
    setMat4(m_ProgramObj, "projection", projection);
    setMat4(m_ProgramObj, "view", view);
    // 设置光源和视点位置以及远裁剪面距离
    setVec3(m_ProgramObj, "lightPos", lightPos);
    setVec3(m_ProgramObj, "viewPos", cameraUtils.Position);
    setFloat(m_ProgramObj, "far_plane", far_plane);
    // 是否开启阴影
    setBool(m_ProgramObj, "shadows", cameraUtils.isEnable);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, woodTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);
    renderScene(m_ProgramObj);

    // 计算每一帧绘制的时间，再计算当前帧结束时间
    deltaTime = TimeUtils::currentTimeSeconds() - currentFrame;
}

void point_shadows::renderScene(GLuint programObj) {
    // Room cube
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(10.0));
    setMat4(programObj, "model", model);
    glDisable(GL_CULL_FACE);
    setInt(programObj, "reverse_normals", 1);
    renderCube();
    setInt(programObj, "reverse_normals", 0);
    glEnable(GL_CULL_FACE);
    // Cubes
    /*model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(4.0f, -3.5f, 0.0));
    model = glm::scale(model, glm::vec3(0.5f));
    setMat4(programObj, "model", model);
    setNormalMatrix(programObj, "normalMatrix", model);
    renderCube();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 3.0f, 1.0));
    model = glm::scale(model, glm::vec3(0.75f));
    setMat4(programObj, "model", model);
    setNormalMatrix(programObj, "normalMatrix", model);
    renderCube();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-3.0f, -1.0f, 0.0));
    model = glm::scale(model, glm::vec3(0.5f));
    setMat4(programObj, "model", model);
    setNormalMatrix(programObj, "normalMatrix", model);
    renderCube();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.5f, 1.0f, 1.5));
    model = glm::scale(model, glm::vec3(0.5f));
    setMat4(programObj, "model", model);
    setNormalMatrix(programObj, "normalMatrix", model);
    renderCube();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.5f, 2.0f, -3.0));
    model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
    model = glm::scale(model, glm::vec3(0.75f));
    setMat4(programObj, "model", model);
    setNormalMatrix(programObj, "normalMatrix", model);
    renderCube();*/
}

void point_shadows::renderCube() {
    // Initialize (if necessary)
    if (cubeVAO == 0) {
        GLfloat vertices[] = {
                // Back face
                -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Bottom-left
                0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top-right
                0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // bottom-right
                0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,  // top-right
                -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,  // bottom-left
                -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,// top-left
                // Front face
                -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
                0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom-right
                0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // top-right
                0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top-right
                -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // top-left
                -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom-left
                // Left face
                -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
                -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-left
                -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-left
                -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
                -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
                -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
                // Right face
                0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-left
                0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
                0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-right
                0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-right
                0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // top-left
                0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-left
                // Bottom face
                -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
                0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, // top-left
                0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,// bottom-left
                0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-left
                -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom-right
                -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
                // Top face
                -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top-left
                0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
                0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top-right
                0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
                -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top-left
                -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f // bottom-left
        };
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        // Fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // Link vertex attributes
        glBindVertexArray(cubeVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *) 0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                              (GLvoid *) (3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
                              (GLvoid *) (6 * sizeof(GLfloat)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    // Render Cube
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void point_shadows::Shutdown() {
    glDeleteBuffers(1, &depthMapFBO);
    glDeleteBuffers(1, &depthCubeMap);
    glDeleteBuffers(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteTextures(1, &woodTexture);
    glDeleteProgram(m_ProgramObj);
    glDeleteProgram(simpleDepthShader);
    GLBaseSample::Shutdown();
}