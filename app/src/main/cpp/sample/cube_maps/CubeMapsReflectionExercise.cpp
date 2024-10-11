//
// Created by ts on 2024/10/11.
//

#include "CubeMapsReflectionExercise.h"

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

void CubeMapsReflectionExercise::Create() {
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
    ourModel = Model(path + "/nanosuit/nanosuit.obj");

    m_ProgramObj = GLUtils::createProgram("shaders/vertex_shader_cube_map.glsl",
                                          "shaders/fragment_shader_cube_map.glsl");
    m_ProgramObj_mode = GLUtils::createProgram("shaders/vertex_shader_reflection_model_exercise.glsl",
                                               "shaders/fragment_shader_reflection_model_exercise.glsl");

    if (!m_ProgramObj || !m_ProgramObj_mode) {
        LOGD("Could not create program")
        return;
    }

    //指定纹理位置
    glUseProgram(m_ProgramObj);
    setInt(m_ProgramObj, "skybox", 0);
    glUseProgram(m_ProgramObj_mode);
    // todo 模型已经使用了前面三个位置，所以天空盒子贴图纹理放到第4个位置
    setInt(m_ProgramObj_mode, "skybox", 3);

    glEnable(GL_DEPTH_TEST);
}

void CubeMapsReflectionExercise::Draw() {
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
    // 添加平行光
    setVec3(m_ProgramObj_mode, "dirLight.direction", 0.0f, 0.0f, 1.0f);
    setVec3(m_ProgramObj_mode, "dirLight.ambient", 1.0f, 1.0f, 1.0f);
    setVec3(m_ProgramObj_mode, "dirLight.diffuse", 1.0f, 1.0f, 1.0f);
    setVec3(m_ProgramObj_mode, "dirLight.specular", 1.0f, 1.0f, 1.0f);
    // 设置镜面反射强度
    setFloat(m_ProgramObj_mode, "material.shininess", 32.0f);

    // 摄像机位置，和法向量一起计算出反射方向，作为获取天空纹理的方向向量
    setVec3(m_ProgramObj_mode, "cameraPos", cameraUtils.Position);

    glm::mat4 projection = glm::perspective(glm::radians(cameraUtils.Zoom), m_Width / m_Height, 0.1f, 100.0f);
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

void CubeMapsReflectionExercise::Shutdown() {
    glDeleteTextures(1, &cubemapTexture);
    glDeleteBuffers(1, &skyboxVAO);
    glDeleteBuffers(1, &skyboxVBO);
    glDeleteBuffers(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteProgram(m_ProgramObj_mode);
    GLBaseSample::Shutdown();
}
