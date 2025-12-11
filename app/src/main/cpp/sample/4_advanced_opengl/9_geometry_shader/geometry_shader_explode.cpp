//
// Created by tzh on 2025/12/10.
//

#include "geometry_shader_explode.h"

REGISTER_SAMPLE(SAMPLE_TYPE_GEOMETRY_SHADER_EXPLODE, geometry_shader_explode)

void geometry_shader_explode::Create() {
    GLUtils::printGLInfo();

    std::string path(DEFAULT_OGL_ASSETS_DIR);
    stbi_set_flip_vertically_on_load(true);
    ourModel = Model(path + "/objects/backpack/backpack.obj");
    m_ProgramObj = GLUtils::createProgram("shaders/vs_geometry_shader_explode.glsl",
                                               "shaders/fs_geometry_shader_explode.glsl",
                                               "shaders/gs_geometry_shader_explode.glsl");

    if (!m_ProgramObj) {
        LOGD("Could not create program")
        return;
    }

    glEnable(GL_DEPTH_TEST);
}

void geometry_shader_explode::Draw() {
    //计算每一帧绘制的时间：先记录当前在开始时间
    float currentFrame = TimeUtils::currentTimeSeconds();

    //清除屏幕和深度缓冲，还有模板缓冲
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //激活着色器程序
    glUseProgram(m_ProgramObj);

    setFloat(m_ProgramObj, "time", seek);

    // view/projection transformations
    glm::mat4 projection = glm::perspective(glm::radians(45.0f),
                                            SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = cameraUtils.GetViewMatrix();
    setMat4(m_ProgramObj, "projection", projection);
    setMat4(m_ProgramObj, "view", view);

    // render the loaded model
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
    setMat4(m_ProgramObj, "model", model);
    ourModel.Draw(m_ProgramObj);

    // 计算每一帧绘制的时间，再计算当前帧结束时间
    deltaTime = TimeUtils::currentTimeSeconds() - currentFrame;
}

void geometry_shader_explode::Shutdown() {
    ourModel.Delete();
    GLBaseSample::Shutdown();
}