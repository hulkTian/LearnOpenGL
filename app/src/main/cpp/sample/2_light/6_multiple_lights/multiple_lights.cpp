//
// Created by ts on 2024/9/14.
//

/**
 * 多光源合并使用实现特殊的场景效果。
 *
 * 颜色向量(Color Vector)：一个通过红绿蓝(RGB)分量的组合描绘大部分真实颜色的向量。一个物体的颜色实际上是该物体所不能吸收的反射颜色分量。
 *
 * 风氏光照模型(Phong Lighting Model)：一个通过计算环境光，漫反射，和镜面光分量的值来近似真实光照的模型。
 *
 * 环境光照(Ambient Lighting)：通过给每个没有被光照的物体很小的亮度，使其不是完全黑暗的，从而对全局光照进行近似。
 *
 * 漫反射着色(Diffuse Shading)：一个顶点/片段与光线方向越接近，光照会越强。使用了法向量来计算角度。
 *
 * 法向量(Normal Vector)：一个垂直于平面的单位向量。
 *
 * 法线矩阵(Normal Matrix)：一个3x3矩阵，或者说是没有平移的模型（或者模型-观察）矩阵。它也被以某种方式修改（逆转置），
 * 从而在应用非统一缩放时，保持法向量朝向正确的方向。否则法向量会在使用非统一缩放时被扭曲。
 *
 * 镜面光照(Specular Lighting)：当观察者视线靠近光源在表面的反射线时会显示的镜面高光。镜面光照是由观察者的方向，
 * 光源的方向和设定高光分散量的反光度值三个量共同决定的。
 *
 * 风氏着色(Phong Shading)：风氏光照模型应用在片段着色器。
 *
 * Gouraud着色(Gouraud shading)：风氏光照模型应用在顶点着色器上。在使用很少数量的顶点时会产生明显的瑕疵。会得到效率提升但是损失了视觉质量。
 *
 * GLSL结构体(GLSL struct)：一个类似于C的结构体，用作着色器变量的容器。大部分时间用来管理输入/输出/uniform。
 *
 * 材质(Material)：一个物体反射的环境光，漫反射，镜面光颜色。这些东西设定了物体所拥有的颜色。
 *
 * 光照属性(Light(properties))：一个光的环境光，漫反射，镜面光的强度。可以使用任何颜色值，对每一个风氏分量(Phong Component)定义光源发出的颜色/强度。
 *
 * 漫反射贴图(Diffuse Map)：一个设定了每个片段中漫反射颜色的纹理图片。
 *
 * 镜面光贴图(Specular Map)：一个设定了每一个片段的镜面光强度/颜色的纹理贴图。仅在物体的特定区域显示镜面高光。
 *
 * 定向光(Directional Light)：只有方向的光源。它被建模为无限距离，这使得它的所有光线看起来都是平行的，因此它的方向矢量在整个场景中保持不变。
 *
 * 点光源(Point Light)：一个在场景中有位置的，光线逐渐衰减的光源。
 *
 * 衰减(Attenuation)：光随着距离减少强度减小的过程，通常使用在点光源和聚光下。
 *
 * 聚光(Spotlight)：一个被定义为在某一个方向上的锥形的光源。
 *
 * 手电筒(Flashlight)：一个摆放在观察者视角的聚光。
 *
 * GLSL Uniform数组(GLSL Uniform Array)：一个uniform值数组。它的工作原理和C语言数组大致一样，只是不能动态分配内存。
 */

#include "multiple_lights.h"

REGISTER_SAMPLE(SAMPLE_TYPE_LIGHTING_MERGE, MultipleLights)

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

// 提取设置点光源的通用函数
void SetPointLights(GLuint program, const glm::vec3 &ambient,
                    const glm::vec3 &diffuse, const glm::vec3 &specular,
                    float constant, float linear, float quadratic) {
    int pointLightCount = sizeof(pointLightPositions) / sizeof(pointLightPositions[0]);
    for (int i = 0; i < pointLightCount; ++i) {
        std::string idx = "pointLights[" + std::to_string(i) + "]";
        setVec3(program, idx + ".position", pointLightPositions[i]);
        setVec3(program, idx + ".ambient", ambient);
        setVec3(program, idx + ".diffuse", diffuse);
        setVec3(program, idx + ".specular", specular);
        setFloat(program, idx + ".constant", constant);
        setFloat(program, idx + ".linear", linear);
        setFloat(program, idx + ".quadratic", quadratic);
    }
}

// 提取设置点光源的通用函数
void SetPointLights(GLuint program, const glm::vec3 *colors, float constant,
                    float linear, float quadratic) {
    int pointLightCount = sizeof(pointLightPositions) / sizeof(pointLightPositions[0]);
    for (int i = 0; i < pointLightCount; ++i) {
        std::string idx = "pointLights[" + std::to_string(i) + "]";
        setVec3(program, idx + ".position", pointLightPositions[i]);
        setVec3(program, idx + ".ambient", colors[i] * 0.1f);
        setVec3(program, idx + ".diffuse", colors[i]);
        setVec3(program, idx + ".specular", colors[i]);
        setFloat(program, idx + ".constant", constant);
        setFloat(program, idx + ".linear", linear);
        setFloat(program, idx + ".quadratic", quadratic);
    }
}

// 设置定向光
void SetDirLight(GLuint program, const glm::vec3 &direction, const glm::vec3 &ambient,
                 const glm::vec3 &diffuse, const glm::vec3 &specular) {
    setVec3(program, "dirLight.direction", direction);
    setVec3(program, "dirLight.ambient", ambient);
    setVec3(program, "dirLight.diffuse", diffuse);
    setVec3(program, "dirLight.specular", specular);
}

// 设置聚光灯
void SetSpotLight(GLuint program, const glm::vec3 &position, const glm::vec3 &direction,
                  const glm::vec3 &ambient, const glm::vec3 &diffuse, const glm::vec3 &specular,
                  float constant, float linear, float quadratic, float cutOff, float outerCutOff) {
    setVec3(program, "spotLight.position", position);
    setVec3(program, "spotLight.direction", direction);
    setVec3(program, "spotLight.ambient", ambient);
    setVec3(program, "spotLight.diffuse", diffuse);
    setVec3(program, "spotLight.specular", specular);
    setFloat(program, "spotLight.constant", constant);
    setFloat(program, "spotLight.linear", linear);
    setFloat(program, "spotLight.quadratic", quadratic);
    setFloat(program, "spotLight.cutOff", cutOff);
    setFloat(program, "spotLight.outerCutOff", outerCutOff);
}

void DrawSceneCommon(const GLuint program, const GLuint program_light, CameraUtils &cameraUtils,
                     const GLuint cubeVAO, const GLuint lightCubeVAO, const glm::vec3 *colors,
                     const GLuint diffuseMap, const GLuint specularMap,
                     float SCR_WIDTH, float SCR_HEIGHT) {
    setVec3(program, "viewPos", cameraUtils.Position);
    setFloat(program, "material.shininess", 32.0f);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularMap);

    glm::mat4 projection = glm::perspective(glm::radians(cameraUtils.Zoom),
                                            SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = cameraUtils.GetViewMatrix();
    setMat4(program, "projection", projection);
    setMat4(program, "view", view);

    // world transformation
    glm::mat4 model = glm::mat4(1.0f);
    setMat4(program, "model", model);

    // render containers
    glBindVertexArray(cubeVAO);
    for (unsigned int i = 0; i < 10; i++) {
        // calculate the model matrix for each object and pass it to shader before drawing
        model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        setMat4(program, "model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    // also draw the lamp object(s)
    glUseProgram(program_light);
    setMat4(program_light, "projection", projection);
    setMat4(program_light, "view", view);

    glBindVertexArray(lightCubeVAO);
    for (unsigned int i = 0; i < 4; i++) {
        model = glm::mat4(1.0f);
        model = glm::translate(model, pointLightPositions[i]);
        model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
        setMat4(program_light, "model", model);
        // 为每个光源立方体设置颜色
        setVec3(program_light, "light.ambient", colors[i]);
        setVec3(program_light, "light.diffuse", colors[i]);
        setVec3(program_light, "light.specular", colors[i]);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

void MultipleLights::Create() {
    GLUtils::printGLInfo();

    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          8 * sizeof(float), (void *) nullptr);
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          8 * sizeof(float), (void *) nullptr);
    glEnableVertexAttribArray(0);

    //创建着色器程序,并编译着色器代码
    m_ProgramObj = GLUtils::createProgram("shaders/vs_lighting_maps.glsl",
                                          "shaders/fs_light_merge.glsl");

    //创建光源的着色器程序
    m_ProgramObj_Light = GLUtils::createProgram("shaders/vs_colors.glsl",
                                                "shaders/fs_colors_light_material.glsl");

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

void MultipleLights::Draw() {
    //计算每一帧绘制的时间
    float currentFrame = TimeUtils::currentTimeSeconds();

    // 根据时间变化场景类型，间隔3秒切换一次场景
    int sceneCount = 5; // 场景总数
    int newType = static_cast<int>(currentFrame * 10 / 3.0f) % sceneCount;
    type = newType;
    LOGD("MultipleLights::Draw type=%d  currentFrame = %f", type, currentFrame)

    if (type == 0) {
        DrawDefault();
    }

    if (type == 1) {
        DrawDesert();
    }

    if (type == 2) {
        DrawFactory();
    }

    if (type == 3) {
        DrawHorror();
    }

    if (type == 4) {
        DrawBiochemicalLab();
    }

    // 计算每一帧绘制的时间，再计算当前帧结束时间
    deltaTime = TimeUtils::currentTimeSeconds() - currentFrame;
}

void MultipleLights::DrawDefault() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(m_ProgramObj);

    // 设置定向光
    SetDirLight(m_ProgramObj,
                glm::vec3(-0.2f, -1.0f, -0.3f),
                glm::vec3(0.05f, 0.05f, 0.05f),
                glm::vec3(0.4f, 0.4f, 0.4f),
                glm::vec3(0.5f, 0.5f, 0.5f)
    );

    // 设置点光源
    SetPointLights(m_ProgramObj,
                   glm::vec3(0.05f, 0.05f, 0.05f),
                   glm::vec3(0.8f, 0.8f, 0.8f),
                   glm::vec3(1.0f, 1.0f, 1.0f),
                   1.0f, 0.09f, 0.032f);

    // 设置聚光灯
    SetSpotLight(m_ProgramObj,
                 cameraUtils.Position,
                 cameraUtils.Front,
                 glm::vec3(0.0f, 0.0f, 0.0f),
                 glm::vec3(1.0f, 1.0f, 1.0f),
                 glm::vec3(1.0f, 1.0f, 1.0f),
                 1.0f, 0.09f, 0.032f,
                 glm::cos(glm::radians(12.5f)),
                 glm::cos(glm::radians(15.0f))
    );

    glm::vec3 pointLightColors[] = {
            glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3(1.0f, 1.0f, 1.0f),
    };

    DrawSceneCommon(m_ProgramObj, m_ProgramObj_Light, cameraUtils,
                    cubeVAO, lightCubeVAO, pointLightColors,
                    diffuseMap, specularMap,
                    SCR_WIDTH, SCR_HEIGHT);
}

// 沙漠效果
void MultipleLights::DrawDesert() {
    glClearColor(0.75f, 0.52f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(m_ProgramObj);

    // 设置定向光
    SetDirLight(m_ProgramObj,
                glm::vec3(-0.2f, -1.0f, -0.3f),
                glm::vec3(0.3f, 0.24f, 0.14f),
                glm::vec3(0.7f, 0.42f, 0.26f),
                glm::vec3(0.5f, 0.5f, 0.5f)
    );

    glm::vec3 pointLightColors[] = {
            glm::vec3(1.0f, 0.6f, 0.0f),
            glm::vec3(1.0f, 0.9f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.2f, 0.2f, 1.0f)
    };

    // 设置点光源
    SetPointLights(m_ProgramObj, pointLightColors, 1.0f, 0.09f, 0.032f);

    // 设置聚光灯
    SetSpotLight(m_ProgramObj,
                 cameraUtils.Position,
                 cameraUtils.Front,
                 glm::vec3(0.0f, 0.0f, 0.0f),
                 glm::vec3(0.8f, 0.8f, 0.0f),
                 glm::vec3(0.8f, 0.8f, 0.0f),
                 1.0f, 0.09f, 0.032f,
                 glm::cos(glm::radians(12.5f)),
                 glm::cos(glm::radians(13.0f))
    );

    // 设置共同渲染逻辑
    DrawSceneCommon(m_ProgramObj, m_ProgramObj_Light, cameraUtils,
                    cubeVAO, lightCubeVAO, pointLightColors,
                    diffuseMap, specularMap,
                    SCR_WIDTH, SCR_HEIGHT);
}

// 工厂效果
void MultipleLights::DrawFactory() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // be sure to activate shader when setting uniforms/drawing objects
    glUseProgram(m_ProgramObj);

    // 设置定向光
    SetDirLight(m_ProgramObj,
                glm::vec3(-0.2f, -1.0f, -0.3f),
                glm::vec3(0.05f, 0.05f, 0.1f),
                glm::vec3(0.2f, 0.2f, 0.7f),
                glm::vec3(0.7f, 0.7f, 0.7f)
    );

    // 设置点光源
    glm::vec3 pointLightColors[] = {
            glm::vec3(0.2f, 0.2f, 0.6f),
            glm::vec3(0.3f, 0.3f, 0.7f),
            glm::vec3(0.0f, 0.0f, 0.3f),
            glm::vec3(0.4f, 0.4f, 0.4f)
    };
    SetPointLights(m_ProgramObj, pointLightColors, 1.0f, 0.09f, 0.032f);

    // 设置聚光灯
    SetSpotLight(m_ProgramObj, cameraUtils.Position, cameraUtils.Front,
                 glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f),
                 glm::vec3(0.0f, 0.0f, 1.0f), 1.0f, 0.09f, 0.032f,
                 glm::cos(glm::radians(10.0f)),
                 glm::cos(glm::radians(15.0f))
    );

    // 设置共同渲染逻辑
    DrawSceneCommon(m_ProgramObj, m_ProgramObj_Light, cameraUtils,
                    cubeVAO, lightCubeVAO, pointLightColors, diffuseMap, specularMap,
                    SCR_WIDTH, SCR_HEIGHT);
}

// 恐怖效果
void MultipleLights::DrawHorror() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(m_ProgramObj);

    // 设置定向光
    SetDirLight(m_ProgramObj,
                glm::vec3(-0.2f, -1.0f, -0.3f),
                glm::vec3(0.0f, 0.0f, 0.0f),
                glm::vec3(0.05f, 0.05f, 0.05f),
                glm::vec3(0.2f, 0.2f, 0.2f)
    );

    // 设置点光源
    glm::vec3 pointLightColors[] = {
            glm::vec3(0.1f, 0.1f, 0.1f),
            glm::vec3(0.1f, 0.1f, 0.1f),
            glm::vec3(0.1f, 0.1f, 0.1f),
            glm::vec3(0.3f, 0.1f, 0.1f)
    };
    SetPointLights(m_ProgramObj, pointLightColors, 1.0f, 0.14f, 0.07f);

    // 设置聚光灯
    SetSpotLight(m_ProgramObj,
                 cameraUtils.Position,
                 cameraUtils.Front,
                 glm::vec3(0.0f, 0.0f, 0.0f),
                 glm::vec3(1.0f, 1.0f, 1.0f),
                 glm::vec3(1.0f, 1.0f, 1.0f),
                 1.0f, 0.09f, 0.032f,
                 glm::cos(glm::radians(10.0f)),
                 glm::cos(glm::radians(15.0f))
    );

    // 设置共同渲染逻辑
    DrawSceneCommon(m_ProgramObj, m_ProgramObj_Light, cameraUtils,
                    cubeVAO, lightCubeVAO, pointLightColors,
                    diffuseMap, specularMap,
                    SCR_WIDTH, SCR_HEIGHT);
}

// 生物化学实验室效果
void MultipleLights::DrawBiochemicalLab() {
    glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // be sure to activate shader when setting uniforms/drawing objects
    glUseProgram(m_ProgramObj);

    // 设置定向光
    SetDirLight(m_ProgramObj,
                glm::vec3(-0.2f, -1.0f, -0.3f),
                glm::vec3(0.5f, 0.5f, 0.5f),
                glm::vec3(1.0f, 1.0f, 1.0f),
                glm::vec3(1.0f, 1.0f, 1.0f)
    );

    // 设置点光源
    glm::vec3 pointLightColors[] = {
            glm::vec3(0.4f, 0.7f, 0.1f),
            glm::vec3(0.4f, 0.7f, 0.1f),
            glm::vec3(0.4f, 0.7f, 0.1f),
            glm::vec3(0.4f, 0.7f, 0.1f)
    };
    SetPointLights(m_ProgramObj, pointLightColors, 1.0f, 0.07f, 0.017f);

    // 设置聚光灯
    SetSpotLight(m_ProgramObj,
                 cameraUtils.Position,
                 cameraUtils.Front,
                 glm::vec3(0.0f, 0.0f, 0.0f),
                 glm::vec3(0.0f, 1.0f, 0.0f),
                 glm::vec3(0.0f, 1.0f, 0.0f),
                 1.0f, 0.07f, 0.017f,
                 glm::cos(glm::radians(7.0f)),
                 glm::cos(glm::radians(10.0f))
    );

    // 设置共同渲染逻辑
    DrawSceneCommon(m_ProgramObj, m_ProgramObj_Light, cameraUtils,
                    cubeVAO, lightCubeVAO, pointLightColors,
                    diffuseMap, specularMap,
                    SCR_WIDTH, SCR_HEIGHT);
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
