//
// Created by ts on 2024/9/10.
//

#include "Material.h"

static float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

void Material::Create() {
    GLUtils::printGLInfo();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 顶点数据
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    // 法向量数据
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),(void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenVertexArrays(1, &VAO_light);
    glBindVertexArray(VAO_light);
    // 只需要绑定VBO不用再次设置VBO的数据，因为箱子的VBO数据中已经包含了正确的立方体顶点数据
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 设置灯立方体的顶点属性（对我们的灯来说仅仅只有位置数据）
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    //创建着色器程序,并编译着色器代码
    m_ProgramObj = GLUtils::createProgram("shaders/vertex_shader_material.glsl",
                                          "shaders/fragment_shader_material.glsl");

    //创建光源的着色器程序
    m_ProgramObj_Light = GLUtils::createProgram("shaders/vertex_shader_colors.glsl",
                                                "shaders/fragment_shader_colors_light_material.glsl");

    if (!m_ProgramObj || !m_ProgramObj_Light) {
        LOGD("Could not create program")
        return;
    }

    //开启深度测试
    glEnable(GL_DEPTH_TEST);

    //设置清屏颜色
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}

void Material::ProcessInput(int i) {
    switch (i) {
        case KEY_W:
            camera.ProcessKeyboard(FORWARD, deltaTime);
            break;
        case KEY_S:
            camera.ProcessKeyboard(BACKWARD, deltaTime);
            break;
        case KEY_A:
            camera.ProcessKeyboard(LEFT, deltaTime);
            break;
        case KEY_D:
            camera.ProcessKeyboard(RIGHT, deltaTime);
            break;
    }
}

void Material::MoveCallback(double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
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

void Material::Draw() {
    // per-frame time logic
    // --------------------
    float currentFrame = static_cast<float>(TimeUtils::currentTimeSeconds());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    //清除屏幕
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // be sure to activate shader when setting uniforms/drawing objects
    glUseProgram(m_ProgramObj);
    setVec3(m_ProgramObj, "light.position", lightPos);
    setVec3(m_ProgramObj, "viewPos", camera.Position);

    // light properties
    glm::vec3 lightColor;
    lightColor.x = static_cast<float>(sin(TimeUtils::currentTimeSeconds() * 2.0));
    lightColor.y = static_cast<float>(sin(TimeUtils::currentTimeSeconds() * 0.7));
    lightColor.z = static_cast<float>(sin(TimeUtils::currentTimeSeconds() * 1.3));
    glm::vec3 diffuseColor = lightColor   * glm::vec3(0.5f); // decrease the influence
    glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence
    setVec3(m_ProgramObj,"light.ambient", ambientColor);
    setVec3(m_ProgramObj,"light.diffuse", diffuseColor);
    setVec3(m_ProgramObj,"light.specular", 1.0f, 1.0f, 1.0f);

    // material properties
    setVec3(m_ProgramObj,"material.ambient", 1.0f, 0.5f, 0.31f);
    setVec3(m_ProgramObj,"material.diffuse", 1.0f, 0.5f, 0.31f);
    setVec3(m_ProgramObj,"material.specular", 0.5f, 0.5f, 0.5f); // specular lighting doesn't have full effect on this object's material
    setFloat(m_ProgramObj,"material.shininess", 32.0f);

    // view/projection transformations
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), m_Width / m_Height, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    setMat4(m_ProgramObj,"projection", projection);
    setMat4(m_ProgramObj,"view", view);

    // world transformation
    glm::mat4 model = glm::mat4(1.0f);
    setMat4(m_ProgramObj,"model", model);

    // render the cube
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);


    // also draw the lamp object
    glUseProgram(m_ProgramObj_Light);
    setMat4(m_ProgramObj_Light, "projection", projection);
    setMat4(m_ProgramObj_Light, "view", view);
    model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
    setMat4(m_ProgramObj_Light, "model", model);

    // 光源立方体颜色
    setVec3(m_ProgramObj_Light, "light.position", lightPos);
    setVec3(m_ProgramObj_Light,"light.ambient", ambientColor);
    setVec3(m_ProgramObj_Light,"light.diffuse", diffuseColor);
    setVec3(m_ProgramObj_Light,"light.specular", 1.0f, 1.0f, 1.0f);

    glBindVertexArray(VAO_light);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Material::Shutdown() {
    //关闭顶点属性
    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &VAO_light);
    glDeleteBuffers(1, &VBO);
    // 释放光源的着色器程序指针
    if (m_ProgramObj_Light) {
        glDeleteProgram(m_ProgramObj_Light);
        m_ProgramObj_Light = GL_NONE;
    }
    GLBaseSample::Shutdown();
}