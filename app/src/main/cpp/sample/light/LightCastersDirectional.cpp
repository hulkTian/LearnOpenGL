//
// Created by ts on 2024/9/13.
//
/**
 * 将光投射(Cast)到物体的光源叫做投光物(Light Caster)。
 *
 *
 * 平行光：
 * 当一个光源处于很远的地方时，来自光源的每条光线就会近似于互相平行。
 * 当我们使用一个假设光源处于无限远处的模型时，它就被称为【定向光】，因为它的所有光线都有着相同的方向，它与光源的位置是没有关系的。
 *
 * 注意：目前使用的光照计算需求一个从片段至光源的光线方向，但人们更习惯定义定向光为一个从光源出发的全局方向。
 * 所以我们需要对全局光照方向向量取反来改变它的方向，它现在是一个指向光源的方向向量了。
 * vec3 lightDir = normalize(-light.direction);
 *
 * 点光源：
 * 点光源是处于世界中某一个位置的光源，它会朝着所有方向发光，但光线会随着距离逐渐衰减。
 * 随着光线传播距离的增长逐渐削减光的强度通常叫做衰减(Attenuation)。
 *
 * 衰减工时：Fatt=1.0/(Kc+Kl∗d+Kq∗d2)
 * 常数项通常保持为1.0，它的主要作用是保证分母永远不会比1小，否则的话在某些距离上它反而会增加强度，这肯定不是我们想要的效果。
 * 一次项会与距离值相乘，以线性的方式减少强度。
 * 二次项会与距离的平方相乘，让光源以二次递减的方式减少强度。二次项在距离比较小的时候影响会比一次项小很多，但当距离值比较大的时候它就会比一次项更大了。
 * 正确地设定它们的值取决于很多因素：环境、希望光覆盖的距离、光的类型等。
 *
 * 聚光：
 * 聚光是位于环境中某个位置的光源，这样的结果就是只有在聚光方向的特定半径内的物体才会被照亮，其它的物体都会保持黑暗。
 *
 * OpenGL中聚光是用一个世界空间位置、一个方向和一个切光角(Cutoff Angle)来表示的，切光角指定了聚光的半径。
 *
 * 平滑/软化边缘
 * 为了创建一种看起来边缘平滑的聚光，我们需要模拟聚光有一个内圆锥(Inner Cone)和一个外圆锥(Outer Cone)。
 * 一个片段处于内外圆锥之间，将会给它计算出一个0.0到1.0之间的强度值。
 */

#include "LightCastersDirectional.h"

static float vertices[] = {
        // positions                      // normals                       // texture coords
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

void LightCastersDirectional::Create() {
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
    /*m_ProgramObj = GLUtils::createProgram("shaders/vs_light_casters.glsl",
                                          "shaders/fs_light_casters.glsl");*/
    //聚光着色器
    m_ProgramObj = GLUtils::createProgram("shaders/vs_light_casters.glsl",
                                          "shaders/fs_light_flashlight.glsl");

    //创建光源的着色器程序
    m_ProgramObj_Light = GLUtils::createProgram("shaders/vs_colors.glsl",
                                                "shaders/fs_colors_light.glsl");

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

void LightCastersDirectional::Draw() {
    //计算每一帧绘制的时间：先记录当前在开始时间
    float currentFrame = TimeUtils::currentTimeSeconds();

    //清除屏幕
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // be sure to activate shader when setting uniforms/drawing objects
    glUseProgram(m_ProgramObj);

    /*setVec3(m_ProgramObj, "light.direction", -0.2f, -1.0f, -0.3f);
    setVec3(m_ProgramObj, "light.position", lightPos);
    setVec3(m_ProgramObj, "viewPos", camera.Position);

    // light properties
    setVec3(m_ProgramObj,"light.ambient", 0.2f, 0.2f, 0.2f);
    setVec3(m_ProgramObj,"light.diffuse", 0.5f, 0.5f, 0.5f);
    setVec3(m_ProgramObj,"light.specular", 1.0f, 1.0f, 1.0f);

    // 点光源常数项
    setFloat(m_ProgramObj,"light.constant", 1.0f);
    setFloat(m_ProgramObj,"light.linear", 0.09f);
    setFloat(m_ProgramObj,"light.quadratic", 0.032f);*/

    // 聚光源
    // 手电筒的位置与观察位置相同
    setVec3(m_ProgramObj, "light.direction", cameraUtils.Front);//聚光的指向向量
    // 聚光的方向向量需要光的位置和片段位置相减计算得出
    setVec3(m_ProgramObj, "light.position", cameraUtils.Position);
    //设置切光角的余玄值和θ（光源方向和光的指向的夹角）进行比较可以节省GPU的计算消耗
    setFloat(m_ProgramObj, "light.cutOff", glm::cos(glm::radians(12.5f)));
    setFloat(m_ProgramObj, "light.outerCutOff", glm::cos(glm::radians(25.0f)));
    setVec3(m_ProgramObj, "viewPos", cameraUtils.Position);

    // light properties
    setVec3(m_ProgramObj, "light.ambient", 0.1f, 0.1f, 0.1f);
    // we configure the diffuse intensity slightly higher; the right lighting conditions differ with each lighting method and environment.
    // each environment and lighting type requires some tweaking to get the best out of your environment.
    setVec3(m_ProgramObj, "light.diffuse", 0.8f, 0.8f, 0.8f);
    setVec3(m_ProgramObj, "light.specular", 1.0f, 1.0f, 1.0f);
    setFloat(m_ProgramObj, "light.constant", 1.0f);
    setFloat(m_ProgramObj, "light.linear", 0.09f);
    setFloat(m_ProgramObj, "light.quadratic", 0.032f);

    // material properties
    setFloat(m_ProgramObj, "material.shininess", 32.0f);

    // view/projection transformations
    glm::mat4 projection = glm::perspective(glm::radians(cameraUtils.Zoom), m_Width / m_Height, 0.1f,
                                            100.0f);
    setMat4(m_ProgramObj, "projection", projection);
    setMat4(m_ProgramObj, "view", cameraUtils.GetViewMatrix());

    // world transformation
    glm::mat4 model = glm::mat4(1.0f);
    setMat4(m_ProgramObj, "model", model);

    // bind diffuse map
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    // bind specular map
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularMap);

    // render the cube
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

    // also draw the lamp object
    glUseProgram(m_ProgramObj_Light);
    setMat4(m_ProgramObj_Light, "projection", projection);
    setMat4(m_ProgramObj_Light, "view", cameraUtils.GetViewMatrix());
    model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
    setMat4(m_ProgramObj_Light, "model", model);

    glBindVertexArray(lightCubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    // 计算每一帧绘制的时间，再计算当前帧结束时间
    deltaTime = TimeUtils::currentTimeSeconds() - currentFrame;
}

void LightCastersDirectional::Shutdown() {
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