//
// Created by ts on 2024/10/10.
//
/**
 * 模型反射天空盒子颜色
 */

#include "ReflectionMode.h"
REGISTER_SAMPLE(SAMPLE_TYPE_CUBE_MAPS_REFLECTION2, ReflectionMode)
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

void ReflectionMode::Create() {
    GLUtils::printGLInfo();

    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);

    // load textures
    cubemapTexture = GLUtils::loadCubemap(faces, false);

    std::string path(DEFAULT_OGL_ASSETS_DIR);
    ourModel = Model(path + "/objects/nanosuit/nanosuit.obj");

    m_ProgramObj = GLUtils::createProgram("shaders/vs_cube_map.glsl",
                                          "shaders/fs_cube_map.glsl");
    m_ProgramObj_mode = GLUtils::createProgram("shaders/vs_reflection_model.glsl",
                                          "shaders/fs_reflection_model.glsl");

    if (!m_ProgramObj || !m_ProgramObj_mode) {
        LOGD("Could not create program")
        return;
    }

    //指定纹理位置
    glUseProgram(m_ProgramObj);
    setInt(m_ProgramObj, "skybox", 0);
    glUseProgram(m_ProgramObj_mode);
    setInt(m_ProgramObj_mode, "skybox", 3);

    glEnable(GL_DEPTH_TEST);
}

void ReflectionMode::Draw() {
    //计算每一帧绘制的时间：先记录当前在开始时间
    float currentFrame = TimeUtils::currentTimeSeconds();

    //清除屏幕和深度缓冲，还有模板缓冲
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // draw model
    glUseProgram(m_ProgramObj_mode);
    // 天空盒子纹理绑定给模型
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

    // 摄像机位置，和法向量一起计算出反射方向，作为获取天空纹理的方向向量
    setVec3(m_ProgramObj_mode, "cameraPos", cameraUtils.Position);

    glm::mat4 projection = glm::perspective(glm::radians(cameraUtils.Zoom),
                                            SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = cameraUtils.GetViewMatrix();
    setMat4(m_ProgramObj_mode, "projection", projection);
    setMat4(m_ProgramObj_mode, "view", view);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -0.7f, 0.0f));
    model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
    setMat4(m_ProgramObj_mode, "model", model);
    ourModel.Draw(m_ProgramObj_mode);

    // draw skybox as last
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
    glDepthFunc(GL_LESS); // set depth function back to default

    // 计算每一帧绘制的时间，再计算当前帧结束时间
    deltaTime = TimeUtils::currentTimeSeconds() - currentFrame;
}

void ReflectionMode::Shutdown() {
    glDeleteTextures(1, &cubemapTexture);
    glDeleteBuffers(1, &skyboxVAO);
    glDeleteBuffers(1, &skyboxVBO);
    glDeleteBuffers(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteProgram(m_ProgramObj_mode);
    GLBaseSample::Shutdown();
}