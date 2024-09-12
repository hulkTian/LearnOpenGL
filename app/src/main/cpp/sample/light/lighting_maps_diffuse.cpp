//
// Created by ts on 2024/9/11.
//
/**
 * 漫反射和镜面光贴图
 * 在着色器中使用材质贴图的方法和纹理教程中是完全一样的。区别只是把材质中漫反射和镜面反射的颜色向量改为纹理2D采样器。
 * 大多数情况下环境光颜色在几乎所有情况下都等于漫反射颜色，所以漫反射的贴图也可以当作环境光贴图使用。
 */


#include "lighting_maps_diffuse.h"

static float vertices[] = {
        // positions                      // normals                       // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};

void LightingMapsDiffuse::Create() {
    GLUtils::printGLInfo();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glGenVertexArrays(1, &VAO_light);
    glBindVertexArray(VAO_light);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    //创建着色器程序,并编译着色器代码
    m_ProgramObj = GLUtils::createProgram("shaders/vertex_shader_lighting_maps.glsl",
                                          "shaders/fragment_shader_lighting_maps.glsl");

    //创建光源的着色器程序
    m_ProgramObj_Light = GLUtils::createProgram("shaders/vertex_shader_colors.glsl",
                                                "shaders/fragment_shader_colors_light.glsl");

    if (!m_ProgramObj || !m_ProgramObj_Light) {
        LOGD("Could not create program")
        return;
    }

    //加载漫反射贴图
    diffuseMap = GLUtils::loadTgaTexture("textures/container2.png");
    //加载镜面反射贴图
    specularMap = GLUtils::loadTgaTexture("textures/container2_specular.png");
    //放射光贴图
    emissionMap = GLUtils::loadTgaTexture("textures/matrix.png");

    glUseProgram(m_ProgramObj);
    setInt(m_ProgramObj, "material.diffuse", 0);
    setInt(m_ProgramObj, "material.specular", 1);
    setInt(m_ProgramObj, "material.emission", 2);

    //开启深度测试
    glEnable(GL_DEPTH_TEST);

    //设置清屏颜色
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
}

void LightingMapsDiffuse::ProcessInput(int i) {
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

void LightingMapsDiffuse::MoveCallback(double xposIn, double yposIn) {
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

void LightingMapsDiffuse::Draw() {
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
    setVec3(m_ProgramObj,"light.ambient", 0.2f, 0.2f, 0.2f);
    setVec3(m_ProgramObj,"light.diffuse", 0.5f, 0.5f, 0.5f);
    setVec3(m_ProgramObj,"light.specular", 1.0f, 1.0f, 1.0f);

    /**
     * 练习题：
     * 调整光源的环境光、漫反射和镜面光向量，看看它们如何影响箱子的视觉输出。
     * setVec3(m_ProgramObj,"light.ambient", 1.2f, 1.2f, 1.2f);
     * setVec3(m_ProgramObj,"light.diffuse", 0.5f, 0.5f, 0.5f);
     * setVec3(m_ProgramObj,"light.specular", 2.0f, 2.0f, 2.0f);
     * 调节环境光和漫反射光的效果基本一样：数值越大，箱子会变得整体变得更亮
     * 调节镜面反射光效果：数值越大，箱子有镜面反射光的部分会变得更亮
     */

    // material properties
    setFloat(m_ProgramObj,"material.shininess", 64.0f);

    // view/projection transformations
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), m_Width / m_Height, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    setMat4(m_ProgramObj,"projection", projection);
    setMat4(m_ProgramObj,"view", view);

    // world transformation
    glm::mat4 model = glm::mat4(1.0f);
    setMat4(m_ProgramObj,"model", model);

    // bind diffuse map
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    // bind specular map
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularMap);
    // bind emission map
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, emissionMap);

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

    glBindVertexArray(VAO_light);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void LightingMapsDiffuse::Shutdown() {
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