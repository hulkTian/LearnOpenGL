//
// Created by ts on 2024/9/14.
//

#include "multiple_lights.h"

static float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
};
// positions all containers
static glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)
};
// positions of the point lights
static glm::vec3 pointLightPositions[] = {
        glm::vec3(0.7f, 0.2f, 2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f, 2.0f, -12.0f),
        glm::vec3(0.0f, 0.0f, -3.0f)
};

void MultipleLights::Create() {
    GLUtils::printGLInfo();

    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void *) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);
    // 只需要绑定VBO不用再次设置VBO的数据，因为箱子的VBO数据中已经包含了正确的立方体顶点数据
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 设置灯立方体的顶点属性（对我们的灯来说仅仅只有位置数据）
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    //创建着色器程序,并编译着色器代码
    m_ProgramObj = GLUtils::createProgram("shaders/vertex_shader_lighting_maps.glsl",
                                          "shaders/fragment_shader_light_merge.glsl");

    //创建光源的着色器程序
    m_ProgramObj_Light = GLUtils::createProgram("shaders/vertex_shader_colors.glsl",
                                                "shaders/fragment_shader_colors_light_material.glsl");

    if (!m_ProgramObj || !m_ProgramObj_Light) {
        LOGD("Could not create program")
        return;
    }

    //加载漫反射贴图
    diffuseMap = GLUtils::loadTgaTexture("textures/container2.png");
    //加载镜面反射贴图
    specularMap = GLUtils::loadTgaTexture("textures/container2_specular.png");

    glUseProgram(m_ProgramObj);
    setInt(m_ProgramObj, "material.diffuse", 0);
    setInt(m_ProgramObj, "material.specular", 1);

    //开启深度测试
    glEnable(GL_DEPTH_TEST);

    //设置清屏颜色
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}

void MultipleLights::ProcessInput(int i) {
    if (i == KEY_W)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (i == KEY_S)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (i == KEY_A)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (i == KEY_D)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void MultipleLights::MoveCallback(double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void MultipleLights::Draw() {
    //计算每一帧绘制的时间
    float currentFrame = TimeUtils::currentTimeSeconds();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    if (type == 0)
    {
        DrawDefault();
    }

    if (type == 1)
    {
        DrawDesert();
    }

    if (type == 2)
    {
        DrawFactory();
    }

    if (type == 3)
    {
        DrawHorror();
    }

    if (type == 4)
    {
        DrawBiochemicalLab();
    }
}

void MultipleLights::DrawDefault() {
    // render
    // ------
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // be sure to activate shader when setting uniforms/drawing objects
    glUseProgram(m_ProgramObj);

    // directional light
    setVec3(m_ProgramObj, "dirLight.direction", -0.2f, -1.0f, -0.3f);
    setVec3(m_ProgramObj, "dirLight.ambient", 0.05f, 0.05f, 0.05f);
    setVec3(m_ProgramObj, "dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    setVec3(m_ProgramObj, "dirLight.specular", 0.5f, 0.5f, 0.5f);
    // point light 1
    setVec3(m_ProgramObj, "pointLights[0].position", pointLightPositions[0]);
    setVec3(m_ProgramObj, "pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
    setVec3(m_ProgramObj, "pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
    setVec3(m_ProgramObj, "pointLights[0].specular", 1.0f, 1.0f, 1.0f);
    setFloat(m_ProgramObj, "pointLights[0].constant", 1.0f);
    setFloat(m_ProgramObj, "pointLights[0].linear", 0.09f);
    setFloat(m_ProgramObj, "pointLights[0].quadratic", 0.032f);
    // point light 2
    setVec3(m_ProgramObj, "pointLights[1].position", pointLightPositions[1]);
    setVec3(m_ProgramObj, "pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
    setVec3(m_ProgramObj, "pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
    setVec3(m_ProgramObj, "pointLights[1].specular", 1.0f, 1.0f, 1.0f);
    setFloat(m_ProgramObj, "pointLights[1].constant", 1.0f);
    setFloat(m_ProgramObj, "pointLights[1].linear", 0.09f);
    setFloat(m_ProgramObj, "pointLights[1].quadratic", 0.032f);
    // point light 3
    setVec3(m_ProgramObj, "pointLights[2].position", pointLightPositions[2]);
    setVec3(m_ProgramObj, "pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
    setVec3(m_ProgramObj, "pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
    setVec3(m_ProgramObj, "pointLights[2].specular", 1.0f, 1.0f, 1.0f);
    setFloat(m_ProgramObj, "pointLights[2].constant", 1.0f);
    setFloat(m_ProgramObj, "pointLights[2].linear", 0.09f);
    setFloat(m_ProgramObj, "pointLights[2].quadratic", 0.032f);
    // point light 4
    setVec3(m_ProgramObj, "pointLights[3].position", pointLightPositions[3]);
    setVec3(m_ProgramObj, "pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
    setVec3(m_ProgramObj, "pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
    setVec3(m_ProgramObj, "pointLights[3].specular", 1.0f, 1.0f, 1.0f);
    setFloat(m_ProgramObj, "pointLights[3].constant", 1.0f);
    setFloat(m_ProgramObj, "pointLights[3].linear", 0.09f);
    setFloat(m_ProgramObj, "pointLights[3].quadratic", 0.032f);
    // spotLight
    setVec3(m_ProgramObj, "spotLight.position", camera.Position);
    setVec3(m_ProgramObj, "spotLight.direction", camera.Front);
    setVec3(m_ProgramObj, "spotLight.ambient", 0.0f, 0.0f, 0.0f);
    setVec3(m_ProgramObj, "spotLight.diffuse", 1.0f, 1.0f, 1.0f);
    setVec3(m_ProgramObj, "spotLight.specular", 1.0f, 1.0f, 1.0f);
    setFloat(m_ProgramObj, "spotLight.constant", 1.0f);
    setFloat(m_ProgramObj, "spotLight.linear", 0.09f);
    setFloat(m_ProgramObj, "spotLight.quadratic", 0.032f);
    setFloat(m_ProgramObj, "spotLight.cutOff", glm::cos(glm::radians(12.5f)));
    setFloat(m_ProgramObj, "spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

    setVec3(m_ProgramObj, "viewPos", camera.Position);
    setFloat(m_ProgramObj, "material.shininess", 32.0f);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularMap);

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),m_Width / m_Height, 0.1f,100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    setMat4(m_ProgramObj, "projection", projection);
    setMat4(m_ProgramObj, "view", view);

    // world transformation
    glm::mat4 model = glm::mat4(1.0f);
    setMat4(m_ProgramObj, "model", model);

    // render containers
    glBindVertexArray(cubeVAO);
    for (unsigned int i = 0; i < 10; i++) {
        // calculate the model matrix for each object and pass it to shader before drawing
        model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        setMat4(m_ProgramObj, "model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // also draw the lamp object(s)
    glUseProgram(m_ProgramObj_Light);
    setMat4(m_ProgramObj_Light, "projection", projection);
    setMat4(m_ProgramObj_Light, "view", view);

    glBindVertexArray(lightCubeVAO);
    for (unsigned int i = 0; i < 4; i++) {
        model = glm::mat4(1.0f);
        model = glm::translate(model, pointLightPositions[i]);
        model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
        setMat4(m_ProgramObj_Light, "model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

// 沙漠效果
void MultipleLights::DrawDesert() {
    glClearColor(0.75f, 0.52f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // be sure to activate shader when setting uniforms/drawing objects
    glUseProgram(m_ProgramObj);

    glm::vec3 pointLightColors[] = {
            glm::vec3(1.0f, 0.6f, 0.0f),
            glm::vec3(1.0f, 0.9f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.2f, 0.2f, 1.0f)
    };
    // Directional light
    setVec3(m_ProgramObj,"dirLight.direction", -0.2f, -1.0f, -0.3f);
    setVec3(m_ProgramObj,"dirLight.ambient", 0.3f, 0.24f, 0.14f);
    setVec3(m_ProgramObj,"dirLight.diffuse", 0.7f, 0.42f, 0.26f);
    setVec3(m_ProgramObj,"dirLight.specular", 0.5f, 0.5f, 0.5f);
    // Point light 1
    setVec3(m_ProgramObj,"pointLights[0].position", pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
    setVec3(m_ProgramObj,"pointLights[0].ambient", pointLightColors[0].x * 0.1,  pointLightColors[0].y * 0.1,  pointLightColors[0].z * 0.1);
    setVec3(m_ProgramObj,"pointLights[0].diffuse", pointLightColors[0].x,  pointLightColors[0].y,  pointLightColors[0].z);
    setVec3(m_ProgramObj,"pointLights[0].specular", pointLightColors[0].x,  pointLightColors[0].y,  pointLightColors[0].z);
    setFloat(m_ProgramObj, "pointLights[0].constant", 1.0f);
    setFloat(m_ProgramObj, "pointLights[0].linear", 0.09);
    setFloat(m_ProgramObj, "pointLights[0].quadratic", 0.032);
    // Point light 2
    setVec3(m_ProgramObj,"pointLights[1].position", pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
    setVec3(m_ProgramObj,"pointLights[1].ambient", pointLightColors[1].x * 0.1,  pointLightColors[1].y * 0.1,  pointLightColors[1].z * 0.1);
    setVec3(m_ProgramObj,"pointLights[1].diffuse", pointLightColors[1].x,  pointLightColors[1].y,  pointLightColors[1].z);
    setVec3(m_ProgramObj,"pointLights[1].specular", pointLightColors[1].x,  pointLightColors[1].y,  pointLightColors[1].z);
    setFloat(m_ProgramObj, "pointLights[1].constant", 1.0f);
    setFloat(m_ProgramObj, "pointLights[1].linear", 0.09);
    setFloat(m_ProgramObj, "pointLights[1].quadratic", 0.032);
    // Point light 3
    setVec3(m_ProgramObj,"pointLights[2].position", pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
    setVec3(m_ProgramObj,"pointLights[2].ambient", pointLightColors[2].x * 0.1,  pointLightColors[2].y * 0.1,  pointLightColors[2].z * 0.1);
    setVec3(m_ProgramObj,"pointLights[2].diffuse", pointLightColors[2].x,  pointLightColors[2].y,  pointLightColors[2].z);
    setVec3(m_ProgramObj,"pointLights[2].specular", pointLightColors[2].x,  pointLightColors[2].y,  pointLightColors[2].z);
    setFloat(m_ProgramObj, "pointLights[2].constant", 1.0f);
    setFloat(m_ProgramObj, "pointLights[2].linear", 0.09);
    setFloat(m_ProgramObj, "pointLights[2].quadratic", 0.032);
    // Point light 4
    setVec3(m_ProgramObj,"pointLights[3].position", pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
    setVec3(m_ProgramObj,"pointLights[3].ambient", pointLightColors[3].x * 0.1,  pointLightColors[3].y * 0.1,  pointLightColors[3].z * 0.1);
    setVec3(m_ProgramObj,"pointLights[3].diffuse", pointLightColors[3].x,  pointLightColors[3].y,  pointLightColors[3].z);
    setVec3(m_ProgramObj,"pointLights[3].specular", pointLightColors[3].x,  pointLightColors[3].y,  pointLightColors[3].z);
    setFloat(m_ProgramObj, "pointLights[3].constant", 1.0f);
    setFloat(m_ProgramObj, "pointLights[3].linear", 0.09);
    setFloat(m_ProgramObj, "pointLights[3].quadratic", 0.032);
    // SpotLight
    setVec3(m_ProgramObj,"spotLight.position" ,camera.Position.x, camera.Position.y, camera.Position.z);
    setVec3(m_ProgramObj,"spotLight.direction", camera.Front.x, camera.Front.y, camera.Front.z);
    setVec3(m_ProgramObj,"spotLight.ambient", 0.0f, 0.0f, 0.0f);
    setVec3(m_ProgramObj,"spotLight.diffuse", 0.8f, 0.8f, 0.0f);
    setVec3(m_ProgramObj,"spotLight.specular", 0.8f, 0.8f, 0.0f);
    setFloat(m_ProgramObj, "spotLight.constant", 1.0f);
    setFloat(m_ProgramObj, "spotLight.linear", 0.09);
    setFloat(m_ProgramObj, "spotLight.quadratic", 0.032);
    setFloat(m_ProgramObj, "spotLight.cutOff", glm::cos(glm::radians(12.5f)));
    setFloat(m_ProgramObj, "spotLight.outerCutOff", glm::cos(glm::radians(13.0f)));

    setVec3(m_ProgramObj, "viewPos", camera.Position);
    setFloat(m_ProgramObj, "material.shininess", 32.0f);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularMap);

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),m_Width / m_Height, 0.1f,100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    setMat4(m_ProgramObj, "projection", projection);
    setMat4(m_ProgramObj, "view", view);

    // world transformation
    glm::mat4 model = glm::mat4(1.0f);
    setMat4(m_ProgramObj, "model", model);

    // render containers
    glBindVertexArray(cubeVAO);
    for (unsigned int i = 0; i < 10; i++) {
        // calculate the model matrix for each object and pass it to shader before drawing
        model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        setMat4(m_ProgramObj, "model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // also draw the lamp object(s)
    glUseProgram(m_ProgramObj_Light);
    setMat4(m_ProgramObj_Light, "projection", projection);
    setMat4(m_ProgramObj_Light, "view", view);

    glBindVertexArray(lightCubeVAO);
    for (unsigned int i = 0; i < 4; i++) {
        model = glm::mat4(1.0f);
        model = glm::translate(model, pointLightPositions[i]);
        model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
        setMat4(m_ProgramObj_Light, "model", model);
        // 为每个光源立方体设置颜色
        setVec3(m_ProgramObj_Light,"light.ambient", pointLightColors[i]);
        setVec3(m_ProgramObj_Light,"light.diffuse", pointLightColors[i]);
        setVec3(m_ProgramObj_Light,"light.specular", pointLightColors[i]);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

// 工厂效果
void MultipleLights::DrawFactory() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // be sure to activate shader when setting uniforms/drawing objects
    glUseProgram(m_ProgramObj);

    glm::vec3 pointLightColors[] = {
            glm::vec3(0.2f, 0.2f, 0.6f),
            glm::vec3(0.3f, 0.3f, 0.7f),
            glm::vec3(0.0f, 0.0f, 0.3f),
            glm::vec3(0.4f, 0.4f, 0.4f)
    };
    // Directional light
    setVec3(m_ProgramObj,"dirLight.direction", -0.2f, -1.0f, -0.3f);
    setVec3(m_ProgramObj,"dirLight.ambient", 0.05f, 0.05f, 0.1f);
    setVec3(m_ProgramObj,"dirLight.diffuse", 0.2f, 0.2f, 0.7f);
    setVec3(m_ProgramObj,"dirLight.specular", 0.7f, 0.7f, 0.7f);
    // Point light 1
    setVec3(m_ProgramObj,"pointLights[0].position", pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
    setVec3(m_ProgramObj,"pointLights[0].ambient", pointLightColors[0].x * 0.1,  pointLightColors[0].y * 0.1,  pointLightColors[0].z * 0.1);
    setVec3(m_ProgramObj,"pointLights[0].diffuse", pointLightColors[0].x,  pointLightColors[0].y,  pointLightColors[0].z);
    setVec3(m_ProgramObj,"pointLights[0].specular", pointLightColors[0].x,  pointLightColors[0].y,  pointLightColors[0].z);
    setFloat(m_ProgramObj, "pointLights[0].constant", 1.0f);
    setFloat(m_ProgramObj, "pointLights[0].linear", 0.09f);
    setFloat(m_ProgramObj, "pointLights[0].quadratic", 0.032f);
    // Point light 2
    setVec3(m_ProgramObj,"pointLights[1].position", pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
    setVec3(m_ProgramObj,"pointLights[1].ambient", pointLightColors[1].x * 0.1,  pointLightColors[1].y * 0.1,  pointLightColors[1].z * 0.1);
    setVec3(m_ProgramObj,"pointLights[1].diffuse", pointLightColors[1].x,  pointLightColors[1].y,  pointLightColors[1].z);
    setVec3(m_ProgramObj,"pointLights[1].specular", pointLightColors[1].x,  pointLightColors[1].y,  pointLightColors[1].z);
    setFloat(m_ProgramObj, "pointLights[1].constant", 1.0f);
    setFloat(m_ProgramObj, "pointLights[1].linear", 0.09f);
    setFloat(m_ProgramObj, "pointLights[1].quadratic", 0.032f);
    // Point light 3
    setVec3(m_ProgramObj,"pointLights[2].position", pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
    setVec3(m_ProgramObj,"pointLights[2].ambient", pointLightColors[2].x * 0.1,  pointLightColors[2].y * 0.1,  pointLightColors[2].z * 0.1);
    setVec3(m_ProgramObj,"pointLights[2].diffuse", pointLightColors[2].x,  pointLightColors[2].y,  pointLightColors[2].z);
    setVec3(m_ProgramObj,"pointLights[2].specular", pointLightColors[2].x,  pointLightColors[2].y,  pointLightColors[2].z);
    setFloat(m_ProgramObj, "pointLights[2].constant", 1.0f);
    setFloat(m_ProgramObj, "pointLights[2].linear", 0.09f);
    setFloat(m_ProgramObj, "pointLights[2].quadratic", 0.032f);
    // Point light 4
    setVec3(m_ProgramObj,"pointLights[3].position", pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
    setVec3(m_ProgramObj,"pointLights[3].ambient", pointLightColors[3].x * 0.1,  pointLightColors[3].y * 0.1,  pointLightColors[3].z * 0.1);
    setVec3(m_ProgramObj,"pointLights[3].diffuse", pointLightColors[3].x,  pointLightColors[3].y,  pointLightColors[3].z);
    setVec3(m_ProgramObj,"pointLights[3].specular", pointLightColors[3].x,  pointLightColors[3].y,  pointLightColors[3].z);
    setFloat(m_ProgramObj, "pointLights[3].constant", 1.0f);
    setFloat(m_ProgramObj, "pointLights[3].linear", 0.09f);
    setFloat(m_ProgramObj, "pointLights[3].quadratic", 0.032f);
    // SpotLight
    setVec3(m_ProgramObj,"spotLight.position" ,camera.Position.x, camera.Position.y, camera.Position.z);
    setVec3(m_ProgramObj,"spotLight.direction", camera.Front.x, camera.Front.y, camera.Front.z);
    setVec3(m_ProgramObj,"spotLight.ambient", 0.0f, 0.0f, 0.0f);
    setVec3(m_ProgramObj,"spotLight.diffuse", 1.0f, 1.0f, 1.0f);
    setVec3(m_ProgramObj,"spotLight.specular", 1.0f, 1.0f, 1.0f);
    setFloat(m_ProgramObj, "spotLight.constant", 1.0f);
    setFloat(m_ProgramObj, "spotLight.linear", 0.09f);
    setFloat(m_ProgramObj, "spotLight.quadratic", 0.032f);
    setFloat(m_ProgramObj, "spotLight.cutOff", glm::cos(glm::radians(10.0f)));
    setFloat(m_ProgramObj, "spotLight.outerCutOff", glm::cos(glm::radians(12.5f)));

    setVec3(m_ProgramObj, "viewPos", camera.Position);
    setFloat(m_ProgramObj, "material.shininess", 32.0f);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularMap);

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),m_Width / m_Height, 0.1f,100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    setMat4(m_ProgramObj, "projection", projection);
    setMat4(m_ProgramObj, "view", view);

    // world transformation
    glm::mat4 model = glm::mat4(1.0f);
    setMat4(m_ProgramObj, "model", model);

    // render containers
    glBindVertexArray(cubeVAO);
    for (unsigned int i = 0; i < 10; i++) {
        // calculate the model matrix for each object and pass it to shader before drawing
        model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        setMat4(m_ProgramObj, "model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // also draw the lamp object(s)
    glUseProgram(m_ProgramObj_Light);
    setMat4(m_ProgramObj_Light, "projection", projection);
    setMat4(m_ProgramObj_Light, "view", view);

    glBindVertexArray(lightCubeVAO);
    for (unsigned int i = 0; i < 4; i++) {
        model = glm::mat4(1.0f);
        model = glm::translate(model, pointLightPositions[i]);
        model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
        setMat4(m_ProgramObj_Light, "model", model);
        // 为每个光源立方体设置颜色
        setVec3(m_ProgramObj_Light,"light.ambient", pointLightColors[i]);
        setVec3(m_ProgramObj_Light,"light.diffuse", pointLightColors[i]);
        setVec3(m_ProgramObj_Light,"light.specular", pointLightColors[i]);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

// 恐怖效果
void MultipleLights::DrawHorror() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // be sure to activate shader when setting uniforms/drawing objects
    glUseProgram(m_ProgramObj);

    glm::vec3 pointLightColors[] = {
            glm::vec3(0.1f, 0.1f, 0.1f),
            glm::vec3(0.1f, 0.1f, 0.1f),
            glm::vec3(0.1f, 0.1f, 0.1f),
            glm::vec3(0.3f, 0.1f, 0.1f)
    };
    // Directional light
    setVec3(m_ProgramObj,"dirLight.direction", -0.2f, -1.0f, -0.3f);
    setVec3(m_ProgramObj,"dirLight.ambient", 0.0f, 0.0f, 0.0f);
    setVec3(m_ProgramObj,"dirLight.diffuse", 0.05f, 0.05f, 0.05f);
    setVec3(m_ProgramObj,"dirLight.specular", 0.2f, 0.2f, 0.2f);
    // Point light 1
    setVec3(m_ProgramObj,"pointLights[0].position", pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
    setVec3(m_ProgramObj,"pointLights[0].ambient", pointLightColors[0].x * 0.1,  pointLightColors[0].y * 0.1,  pointLightColors[0].z * 0.1);
    setVec3(m_ProgramObj,"pointLights[0].diffuse", pointLightColors[0].x,  pointLightColors[0].y,  pointLightColors[0].z);
    setVec3(m_ProgramObj,"pointLights[0].specular", pointLightColors[0].x,  pointLightColors[0].y,  pointLightColors[0].z);
    setFloat(m_ProgramObj, "pointLights[0].constant", 1.0f);
    setFloat(m_ProgramObj, "pointLights[0].linear", 0.14f);
    setFloat(m_ProgramObj, "pointLights[0].quadratic", 0.07f);
    // Point light 2
    setVec3(m_ProgramObj,"pointLights[1].position", pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
    setVec3(m_ProgramObj,"pointLights[1].ambient", pointLightColors[1].x * 0.1,  pointLightColors[1].y * 0.1,  pointLightColors[1].z * 0.1);
    setVec3(m_ProgramObj,"pointLights[1].diffuse", pointLightColors[1].x,  pointLightColors[1].y,  pointLightColors[1].z);
    setVec3(m_ProgramObj,"pointLights[1].specular", pointLightColors[1].x,  pointLightColors[1].y,  pointLightColors[1].z);
    setFloat(m_ProgramObj, "pointLights[1].constant", 1.0f);
    setFloat(m_ProgramObj, "pointLights[1].linear", 0.14f);
    setFloat(m_ProgramObj, "pointLights[1].quadratic", 0.07f);
    // Point light 3
    setVec3(m_ProgramObj,"pointLights[2].position", pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
    setVec3(m_ProgramObj,"pointLights[2].ambient", pointLightColors[2].x * 0.1,  pointLightColors[2].y * 0.1,  pointLightColors[2].z * 0.1);
    setVec3(m_ProgramObj,"pointLights[2].diffuse", pointLightColors[2].x,  pointLightColors[2].y,  pointLightColors[2].z);
    setVec3(m_ProgramObj,"pointLights[2].specular", pointLightColors[2].x,  pointLightColors[2].y,  pointLightColors[2].z);
    setFloat(m_ProgramObj, "pointLights[2].constant", 1.0f);
    setFloat(m_ProgramObj, "pointLights[2].linear", 0.14f);
    setFloat(m_ProgramObj, "pointLights[2].quadratic", 0.07f);
    // Point light 4
    setVec3(m_ProgramObj,"pointLights[3].position", pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
    setVec3(m_ProgramObj,"pointLights[3].ambient", pointLightColors[3].x * 0.1,  pointLightColors[3].y * 0.1,  pointLightColors[3].z * 0.1);
    setVec3(m_ProgramObj,"pointLights[3].diffuse", pointLightColors[3].x,  pointLightColors[3].y,  pointLightColors[3].z);
    setVec3(m_ProgramObj,"pointLights[3].specular", pointLightColors[3].x,  pointLightColors[3].y,  pointLightColors[3].z);
    setFloat(m_ProgramObj, "pointLights[3].constant", 1.0f);
    setFloat(m_ProgramObj, "pointLights[3].linear", 0.14f);
    setFloat(m_ProgramObj, "pointLights[3].quadratic", 0.07f);
    // SpotLight
    setVec3(m_ProgramObj,"spotLight.position" ,camera.Position.x, camera.Position.y, camera.Position.z);
    setVec3(m_ProgramObj,"spotLight.direction", camera.Front.x, camera.Front.y, camera.Front.z);
    setVec3(m_ProgramObj,"spotLight.ambient", 0.0f, 0.0f, 0.0f);
    setVec3(m_ProgramObj,"spotLight.diffuse", 1.0f, 1.0f, 1.0f);
    setVec3(m_ProgramObj,"spotLight.specular", 1.0f, 1.0f, 1.0f);
    setFloat(m_ProgramObj, "spotLight.constant", 1.0f);
    setFloat(m_ProgramObj, "spotLight.linear", 0.09f);
    setFloat(m_ProgramObj, "spotLight.quadratic", 0.032f);
    setFloat(m_ProgramObj, "spotLight.cutOff", glm::cos(glm::radians(10.0f)));
    setFloat(m_ProgramObj, "spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

    setVec3(m_ProgramObj, "viewPos", camera.Position);
    setFloat(m_ProgramObj, "material.shininess", 32.0f);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularMap);

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),m_Width / m_Height, 0.1f,100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    setMat4(m_ProgramObj, "projection", projection);
    setMat4(m_ProgramObj, "view", view);

    // world transformation
    glm::mat4 model = glm::mat4(1.0f);
    setMat4(m_ProgramObj, "model", model);

    // render containers
    glBindVertexArray(cubeVAO);
    for (unsigned int i = 0; i < 10; i++) {
        // calculate the model matrix for each object and pass it to shader before drawing
        model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        setMat4(m_ProgramObj, "model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // also draw the lamp object(s)
    glUseProgram(m_ProgramObj_Light);
    setMat4(m_ProgramObj_Light, "projection", projection);
    setMat4(m_ProgramObj_Light, "view", view);

    glBindVertexArray(lightCubeVAO);
    for (unsigned int i = 0; i < 4; i++) {
        model = glm::mat4(1.0f);
        model = glm::translate(model, pointLightPositions[i]);
        model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
        setMat4(m_ProgramObj_Light, "model", model);
        // 为每个光源立方体设置颜色
        setVec3(m_ProgramObj_Light,"light.ambient", pointLightColors[i]);
        setVec3(m_ProgramObj_Light,"light.diffuse", pointLightColors[i]);
        setVec3(m_ProgramObj_Light,"light.specular", pointLightColors[i]);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}
// 生物化学实验室效果
void MultipleLights::DrawBiochemicalLab() {
    glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // be sure to activate shader when setting uniforms/drawing objects
    glUseProgram(m_ProgramObj);

    glm::vec3 pointLightColors[] = {
            glm::vec3(0.4f, 0.7f, 0.1f),
            glm::vec3(0.4f, 0.7f, 0.1f),
            glm::vec3(0.4f, 0.7f, 0.1f),
            glm::vec3(0.4f, 0.7f, 0.1f)
    };
    // Directional light
    setVec3(m_ProgramObj,"dirLight.direction", -0.2f, -1.0f, -0.3f);
    setVec3(m_ProgramObj,"dirLight.ambient", 0.5f, 0.5f, 0.5f);
    setVec3(m_ProgramObj,"dirLight.diffuse", 1.0f, 1.0f, 1.0f);
    setVec3(m_ProgramObj,"dirLight.specular", 1.0f, 1.0f, 1.0f);
    // Point light 1
    setVec3(m_ProgramObj,"pointLights[0].position", pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
    setVec3(m_ProgramObj,"pointLights[0].ambient", pointLightColors[0].x * 0.1,  pointLightColors[0].y * 0.1,  pointLightColors[0].z * 0.1);
    setVec3(m_ProgramObj,"pointLights[0].diffuse", pointLightColors[0].x,  pointLightColors[0].y,  pointLightColors[0].z);
    setVec3(m_ProgramObj,"pointLights[0].specular", pointLightColors[0].x,  pointLightColors[0].y,  pointLightColors[0].z);
    setFloat(m_ProgramObj, "pointLights[0].constant", 1.0f);
    setFloat(m_ProgramObj, "pointLights[0].linear", 0.07f);
    setFloat(m_ProgramObj, "pointLights[0].quadratic", 0.017f);
    // Point light 2
    setVec3(m_ProgramObj,"pointLights[1].position", pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
    setVec3(m_ProgramObj,"pointLights[1].ambient", pointLightColors[1].x * 0.1,  pointLightColors[1].y * 0.1,  pointLightColors[1].z * 0.1);
    setVec3(m_ProgramObj,"pointLights[1].diffuse", pointLightColors[1].x,  pointLightColors[1].y,  pointLightColors[1].z);
    setVec3(m_ProgramObj,"pointLights[1].specular", pointLightColors[1].x,  pointLightColors[1].y,  pointLightColors[1].z);
    setFloat(m_ProgramObj, "pointLights[1].constant", 1.0f);
    setFloat(m_ProgramObj, "pointLights[1].linear", 0.07f);
    setFloat(m_ProgramObj, "pointLights[1].quadratic", 0.017f);
    // Point light 3
    setVec3(m_ProgramObj,"pointLights[2].position", pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
    setVec3(m_ProgramObj,"pointLights[2].ambient", pointLightColors[2].x * 0.1,  pointLightColors[2].y * 0.1,  pointLightColors[2].z * 0.1);
    setVec3(m_ProgramObj,"pointLights[2].diffuse", pointLightColors[2].x,  pointLightColors[2].y,  pointLightColors[2].z);
    setVec3(m_ProgramObj,"pointLights[2].specular", pointLightColors[2].x,  pointLightColors[2].y,  pointLightColors[2].z);
    setFloat(m_ProgramObj, "pointLights[2].constant", 1.0f);
    setFloat(m_ProgramObj, "pointLights[2].linear", 0.07f);
    setFloat(m_ProgramObj, "pointLights[2].quadratic", 0.017f);
    // Point light 4
    setVec3(m_ProgramObj,"pointLights[3].position", pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
    setVec3(m_ProgramObj,"pointLights[3].ambient", pointLightColors[3].x * 0.1,  pointLightColors[3].y * 0.1,  pointLightColors[3].z * 0.1);
    setVec3(m_ProgramObj,"pointLights[3].diffuse", pointLightColors[3].x,  pointLightColors[3].y,  pointLightColors[3].z);
    setVec3(m_ProgramObj,"pointLights[3].specular", pointLightColors[3].x,  pointLightColors[3].y,  pointLightColors[3].z);
    setFloat(m_ProgramObj, "pointLights[3].constant", 1.0f);
    setFloat(m_ProgramObj, "pointLights[3].linear", 0.07f);
    setFloat(m_ProgramObj, "pointLights[3].quadratic", 0.017f);
    // SpotLight
    setVec3(m_ProgramObj,"spotLight.position" ,camera.Position.x, camera.Position.y, camera.Position.z);
    setVec3(m_ProgramObj,"spotLight.direction", camera.Front.x, camera.Front.y, camera.Front.z);
    setVec3(m_ProgramObj,"spotLight.ambient", 0.0f, 0.0f, 0.0f);
    setVec3(m_ProgramObj,"spotLight.diffuse", 0.0f, 1.0f, 0.0f);
    setVec3(m_ProgramObj,"spotLight.specular", 0.0f, 1.0f, 0.0f);
    setFloat(m_ProgramObj, "spotLight.constant", 1.0f);
    setFloat(m_ProgramObj, "spotLight.linear", 0.07f);
    setFloat(m_ProgramObj, "spotLight.quadratic", 0.017f);
    setFloat(m_ProgramObj, "spotLight.cutOff", glm::cos(glm::radians(7.0f)));
    setFloat(m_ProgramObj, "spotLight.outerCutOff", glm::cos(glm::radians(10.0f)));

    setVec3(m_ProgramObj, "viewPos", camera.Position);
    setFloat(m_ProgramObj, "material.shininess", 32.0f);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularMap);

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),m_Width / m_Height, 0.1f,100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    setMat4(m_ProgramObj, "projection", projection);
    setMat4(m_ProgramObj, "view", view);

    // world transformation
    glm::mat4 model = glm::mat4(1.0f);
    setMat4(m_ProgramObj, "model", model);

    // render containers
    glBindVertexArray(cubeVAO);
    for (unsigned int i = 0; i < 10; i++) {
        // calculate the model matrix for each object and pass it to shader before drawing
        model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        setMat4(m_ProgramObj, "model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // also draw the lamp object(s)
    glUseProgram(m_ProgramObj_Light);
    setMat4(m_ProgramObj_Light, "projection", projection);
    setMat4(m_ProgramObj_Light, "view", view);

    glBindVertexArray(lightCubeVAO);
    for (unsigned int i = 0; i < 4; i++) {
        model = glm::mat4(1.0f);
        model = glm::translate(model, pointLightPositions[i]);
        model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
        setMat4(m_ProgramObj_Light, "model", model);
        // 为每个光源立方体设置颜色
        setVec3(m_ProgramObj_Light,"light.ambient", pointLightColors[i]);
        setVec3(m_ProgramObj_Light,"light.diffuse", pointLightColors[i]);
        setVec3(m_ProgramObj_Light,"light.specular", pointLightColors[i]);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

void MultipleLights::Shutdown() {
    //关闭顶点属性
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);
    // 释放光源的着色器程序指针
    if (m_ProgramObj_Light) {
        glDeleteProgram(m_ProgramObj_Light);
        m_ProgramObj_Light = GL_NONE;
    }
    GLBaseSample::Shutdown();
}
