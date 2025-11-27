//
// Created by ts on 2024/10/15.
//

#include "Asteroids.h"

REGISTER_SAMPLE(SAMPLE_TYPE_INSTANCING_ASTEROIDS, Asteroids)
void Asteroids::Create() {
    GLUtils::printGLInfo();

    cameraUtils = CameraUtils(glm::vec3(0.0f, 0.0f, 155.0f));

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    m_ProgramObj = GLUtils::createProgram("shaders/vs_asteroids.glsl",
                                          "shaders/fs_asteroids.glsl");

    // load models
    // -----------
    std::string path(DEFAULT_OGL_ASSETS_DIR);
    rock = Model(path + "/objects/rock/rock.obj");
    planet = Model(path + "/objects/planet/planet.obj");

    // generate a large list of semi-random model transformation matrices
    // ------------------------------------------------------------------
    modelMatrices = new glm::mat4[amount];
    srand(static_cast<unsigned int>(TimeUtils::currentTimeSeconds())); // initialize random seed
    float radius = 150.0;
    float offset = 25.0f;
    for (unsigned int i = 0; i < amount; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        // 1. translation: displace along circle with 'radius' in range [-offset, offset]
        float angle = (float)i / (float)amount * 360.0f;
        float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float x = sin(angle) * radius + displacement;
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float y = displacement * 0.4f; // keep height of asteroid field smaller compared to width of x and z
        displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
        float z = cos(angle) * radius + displacement;
        model = glm::translate(model, glm::vec3(x, y, z));

        // 2. scale: Scale between 0.05 and 0.25f
        float scale = static_cast<float>((rand() % 20) / 100.0 + 0.05);
        model = glm::scale(model, glm::vec3(scale));

        // 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
        float rotAngle = static_cast<float>((rand() % 360));
        model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

        // 4. now add to list of matrices
        modelMatrices[i] = model;
    }
}

void Asteroids::Draw() {
    //计算每一帧绘制的时间：先记录当前在开始时间
    float currentFrame = TimeUtils::currentTimeSeconds();

    //清除屏幕和深度缓冲，还有模板缓冲
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // configure transformation matrices
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), SCR_WIDTH / SCR_HEIGHT, 0.1f, 1000.0f);
    glm::mat4 view = cameraUtils.GetViewMatrix();;
    glUseProgram(m_ProgramObj);
    setMat4(m_ProgramObj, "projection", projection);
    setMat4(m_ProgramObj, "view", view);

    // draw planet
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
    model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
    setMat4(m_ProgramObj, "model", model);
    planet.Draw(m_ProgramObj);

    // draw meteorites
    for (unsigned int i = 0; i < amount; i++)
    {
        setMat4(m_ProgramObj, "model", modelMatrices[i]);
        rock.Draw(m_ProgramObj);
    }

    // 计算每一帧绘制的时间，再计算当前帧结束时间
    deltaTime = TimeUtils::currentTimeSeconds() - currentFrame;
}

void Asteroids::Shutdown() {
    GLBaseSample::Shutdown();
}
