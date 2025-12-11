//
// Created by tzh on 2025/12/11.
//

#include "geometry_shader_normal.h"

REGISTER_SAMPLE(SAMPLE_TYPE_GEOMETRY_SHADER_NORMAL, geometry_shader_normal)

void geometry_shader_normal::Create() {
    GLUtils::printGLInfo();

    m_ProgramObj = GLUtils::createProgram("shaders/vs_geometry_shader_model.glsl",
                                          "shaders/fs_geometry_shader_model.glsl");
    m_ProgramObj_Normal = GLUtils::createProgram("shaders/vs_geometry_shader_normal.glsl",
                                               "shaders/fs_geometry_shader_normal.glsl",
                                               "shaders/gs_geometry_shader_normal.glsl");

    if (!m_ProgramObj || !m_ProgramObj_Normal) {
        LOGD("Could not create program")
        return;
    }

    std::string path(DEFAULT_OGL_ASSETS_DIR);
    stbi_set_flip_vertically_on_load(true);
    ourModel = Model(path + "/objects/backpack/backpack.obj");

    glEnable(GL_DEPTH_TEST);
}

void geometry_shader_normal::Draw() {
    //计算每一帧绘制的时间：先记录当前在开始时间
    float currentFrame = TimeUtils::currentTimeSeconds();

    //清除屏幕和深度缓冲，还有模板缓冲
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //激活着色器程序
    glUseProgram(m_ProgramObj);

    // view/projection transformations
    glm::mat4 projection = glm::perspective(glm::radians(cameraUtils.Zoom),
                                            SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = cameraUtils.GetViewMatrix();
    setMat4(m_ProgramObj, "projection", projection);
    setMat4(m_ProgramObj, "view", view);

    // render the loaded model
    glm::mat4 model = glm::mat4(1.0f);
    setMat4(m_ProgramObj, "model", model);
    ourModel.Draw(m_ProgramObj);

    // 绘制法线
    glUseProgram(m_ProgramObj_Normal);

    setMat4(m_ProgramObj_Normal, "projection", projection);
    setMat4(m_ProgramObj_Normal, "view", view);
    setMat4(m_ProgramObj_Normal, "model", model);
    ourModel.Draw(m_ProgramObj_Normal);

    // 计算每一帧绘制的时间，再计算当前帧结束时间
    deltaTime = TimeUtils::currentTimeSeconds() - currentFrame;
}

void geometry_shader_normal::Shutdown() {
    ourModel.Delete();
    GLBaseSample::Shutdown();
}
