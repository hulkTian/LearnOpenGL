//
// Created by ts on 2024/10/10.
// 折射

#include "CubeMapsRefraction.h"

REGISTER_SAMPLE(SAMPLE_TYPE_CUBE_MAPS_REFLECTION, CubeMapsRefraction)
static float cubeVertices[] = {
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

static float skyboxVertices[] = {
        // positions
        -1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        -1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f
};

static std::vector<std::string> faces
        {
                "textures/skybox/right.jpg",
                "textures/skybox/left.jpg",
                "textures/skybox/top.jpg",
                "textures/skybox/bottom.jpg",
                "textures/skybox/front.jpg",
                "textures/skybox/back.jpg"
        };

void CubeMapsRefraction::Create() {
    GLUtils::printGLInfo();

    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          6 * sizeof(float), (void *) nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                          6 * sizeof(float), (void *) (3 * sizeof(float)));

    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          3 * sizeof(float), (void *) nullptr);

    // load textures
    cubemapTexture = GLUtils::loadCubemap(faces, false);

    std::string path(DEFAULT_OGL_ASSETS_DIR);
    ourModel = Model(path + "/objects/nanosuit/nanosuit.obj");

    m_ProgramObj = GLUtils::createProgram("shaders/vs_cube_map.glsl",
                                          "shaders/fs_cube_map.glsl");
    m_ProgramObj_cube = GLUtils::createProgram("shaders/vs_refraction.glsl",
                                               "shaders/fs_refraction.glsl");
    m_ProgramObj_mode = GLUtils::createProgram("shaders/vs_refraction.glsl",
                                               "shaders/fs_refraction.glsl");

    if (!m_ProgramObj || !m_ProgramObj_cube || !m_ProgramObj_mode) {
        LOGD("Could not create program")
        return;
    }

    //指定纹理位置
    glUseProgram(m_ProgramObj);
    setInt(m_ProgramObj, "skybox", 0);
    glUseProgram(m_ProgramObj_cube);
    setInt(m_ProgramObj_cube, "skybox", 0);
    glUseProgram(m_ProgramObj_mode);
    setInt(m_ProgramObj_mode, "skybox", 0);

    glEnable(GL_DEPTH_TEST);
}

void CubeMapsRefraction::Draw() {
    //计算每一帧绘制的时间：先记录当前在开始时间
    float currentFrame = TimeUtils::currentTimeSeconds();

    //清除屏幕和深度缓冲，还有模板缓冲
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view = cameraUtils.GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(cameraUtils.Zoom),
                                            SCR_WIDTH / SCR_HEIGHT,0.1f, 100.0f);

    DrawMode(view, projection);
    DrawCube(view, projection);
    DrawSkybox(view, projection);

    // 计算每一帧绘制的时间，再计算当前帧结束时间
    deltaTime = TimeUtils::currentTimeSeconds() - currentFrame;
}

void CubeMapsRefraction::DrawCube(glm::mat4 view, glm::mat4 projection) {
    // draw scene as normal
    glUseProgram(m_ProgramObj_cube);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-0.4f, 0.5f, 0.0f));
    model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
    setMat4(m_ProgramObj_cube, "model", model);
    setMat4(m_ProgramObj_cube, "view", view);
    setMat4(m_ProgramObj_cube, "projection", projection);
    setNormalMatrix(m_ProgramObj_cube, "normalMatrix", model);
    // todo 摄像机位置，和法向量一起计算出折射方向，作为获取天空纹理的方向向量
    setVec3(m_ProgramObj_cube, "cameraPos", cameraUtils.Position);
    // cubes
    glBindVertexArray(cubeVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void CubeMapsRefraction::DrawMode(glm::mat4 view, glm::mat4 projection) {
    glUseProgram(m_ProgramObj_mode);
    setVec3(m_ProgramObj_mode, "cameraPos", cameraUtils.Position);
    setMat4(m_ProgramObj_mode, "projection", projection);
    setMat4(m_ProgramObj_mode, "view", view);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -0.7f, 0.0f));
    model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
    setMat4(m_ProgramObj_mode, "model", model);
    setNormalMatrix(m_ProgramObj_cube, "normalMatrix", model);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    ourModel.Draw(m_ProgramObj_mode);
}

void CubeMapsRefraction::DrawSkybox(glm::mat4 view, glm::mat4 projection) {
    glDepthFunc(GL_LEQUAL);
    glUseProgram(m_ProgramObj);
    view = glm::mat4(glm::mat3(cameraUtils.GetViewMatrix()));
    setMat4(m_ProgramObj, "view", view);
    setMat4(m_ProgramObj, "projection", projection);
    // skybox cube
    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
}

void CubeMapsRefraction::Shutdown() {
    glDeleteTextures(1, &cubemapTexture);
    glDeleteBuffers(1, &skyboxVAO);
    glDeleteBuffers(1, &skyboxVBO);
    glDeleteBuffers(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteProgram(m_ProgramObj_cube);
    glDeleteProgram(m_ProgramObj_mode);
    GLBaseSample::Shutdown();
}