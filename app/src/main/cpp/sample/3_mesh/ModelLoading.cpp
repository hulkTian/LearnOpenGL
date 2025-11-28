//
// Created by ts on 2024/9/25.
//

#include "ModelLoading.h"
#include "Model.h"
#include "stb_image.h"

REGISTER_SAMPLE(SAMPLE_TYPE_MODEL_LOADING, ModelLoading)
// positions of the point lights
static glm::vec3 pointLightPositions[] = {
        glm::vec3(0.7f, 0.2f, 2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f, 2.0f, -12.0f),
        glm::vec3(0.0f, 0.0f, -3.0f)
};

void ModelLoading::Create() {
    GLUtils::printGLInfo();

    //TODO 先把 model 文件夹拷贝到 /data/user/0/com.example.learnopengl/objects 路径下，然后可以选择你要加载的模型
    std::string path(DEFAULT_OGL_ASSETS_DIR);
    //ourModel = Model(path + "/objects/nanosuit/nanosuit.obj");
    stbi_set_flip_vertically_on_load(true);
    ourModel = Model(path + "/objects/backpack/backpack.obj");

    //创建着色器程序,并编译着色器代码
    m_ProgramObj = GLUtils::createProgram("shaders/vs_model_loading.glsl",
                                          "shaders/fs_model_loading.glsl");

    //加载边框着色器，绘制箱子的边框部分,其实就是按照箱子的轮廓绘制一个纯色的区域
    m_ProgramObj_border = GLUtils::createProgram("shaders/vs_depth_testing.glsl",
                                                 "shaders/fs_stencil_testing_color.glsl");

    if (!m_ProgramObj) {
        LOGD("Could not create program")
        return;
    }

    //开启深度测试
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    //设置清屏颜色
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}

void ModelLoading::Draw() {
    //计算每一帧绘制的时间：先记录当前在开始时间
    float currentFrame = TimeUtils::currentTimeSeconds();

    //清除屏幕和深度缓冲
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    //激活着色器程序
    glUseProgram(m_ProgramObj);

    setVec3(m_ProgramObj, "viewPos", cameraUtils.Position);
    setFloat(m_ProgramObj, "material.shininess", 32.0f);

    // point light 1
    setVec3(m_ProgramObj, "pointLights[0].position", pointLightPositions[0]);
    setVec3(m_ProgramObj, "pointLights[0].ambient", 1.0f, 1.0f, 1.0f);
    setVec3(m_ProgramObj, "pointLights[0].diffuse", 1.0f, 1.0f, 1.0f);
    setVec3(m_ProgramObj, "pointLights[0].specular", 1.0f, 1.0f, 1.0f);
    setFloat(m_ProgramObj, "pointLights[0].constant", 1.0f);
    setFloat(m_ProgramObj, "pointLights[0].linear", 0.09f);
    setFloat(m_ProgramObj, "pointLights[0].quadratic", 0.032f);

    // point light 2
    setVec3(m_ProgramObj, "pointLights[1].position", pointLightPositions[1]);
    setVec3(m_ProgramObj, "pointLights[1].ambient", 1.0f, 1.0f, 1.0f);
    setVec3(m_ProgramObj, "pointLights[1].diffuse", 1.0f, 1.0f, 1.0f);
    setVec3(m_ProgramObj, "pointLights[1].specular", 1.0f, 1.0f, 1.0f);
    setFloat(m_ProgramObj, "pointLights[1].constant", 1.0f);
    setFloat(m_ProgramObj, "pointLights[1].linear", 0.09f);
    setFloat(m_ProgramObj, "pointLights[1].quadratic", 0.032f);

    // view/projection transformations
    glm::mat4 projection = glm::perspective(glm::radians(cameraUtils.Zoom),
                                            SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = cameraUtils.GetViewMatrix();
    setMat4(m_ProgramObj, "projection", projection);
    setMat4(m_ProgramObj, "view", view);

    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);

    // render the loaded model
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
    model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));	// it's a bit too big for our scene, so scale it down
    setMat4(m_ProgramObj, "model", model);
    setNormalMatrix(m_ProgramObj, "normalMatrix", model);
    ourModel.Draw(m_ProgramObj);

    // 按照模型的轮廓，渲染比模型稍微大一点的区域，这个区域启动模板测试，只保留不等于1的片段，并且禁止更新模板缓冲
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);
    glUseProgram(m_ProgramObj_border);
    setMat4(m_ProgramObj_border, "view", view);
    setMat4(m_ProgramObj_border, "projection", projection);
    float scale = 0.102f;

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(scale, scale, scale));
    setMat4(m_ProgramObj_border, "model", model);
    ourModel.Draw(m_ProgramObj_border);

    //恢复配置
    glBindVertexArray(0);
    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glEnable(GL_DEPTH_TEST);

    // 计算每一帧绘制的时间，再计算当前帧结束时间
    deltaTime = TimeUtils::currentTimeSeconds() - currentFrame;
}

void ModelLoading::Shutdown() {
    GLBaseSample::Shutdown();
}
