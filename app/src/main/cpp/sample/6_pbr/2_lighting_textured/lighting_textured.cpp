//
// Created by ts on 2024/11/21.
//

/**
 * 使用纹理作为PBR材质
 */

#include "lighting_textured.h"

REGISTER_SAMPLE(SAMPLE_TYPE_PBR_LIGHTING_TEXTURED, lighting_textured)

void lighting_textured::Create() {
    GLUtils::printGLInfo();

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    m_ProgramObj = GLUtils::createProgram("shaders/vs_pbr_lighting.glsl",
                                          "shaders/fs_pbr_textured.glsl");

    if (!m_ProgramObj) {
        LOGD("Could not create program")
        return;
    }

    glUseProgram(m_ProgramObj);
    setInt(m_ProgramObj, "albedoMap", 0);
    setInt(m_ProgramObj, "normalMap", 1);
    setInt(m_ProgramObj, "metallicMap", 2);
    setInt(m_ProgramObj, "roughnessMap", 3);
    setInt(m_ProgramObj, "aoMap", 4);

    /*albedo = GLUtils::loadTexture("textures/pbr/rusted_iron/albedo.png", false);
    normal = GLUtils::loadTexture("textures/pbr/rusted_iron/normal.png", false);
    metallic = GLUtils::loadTexture("textures/pbr/rusted_iron/metallic.png", false);
    roughness = GLUtils::loadTexture("textures/pbr/rusted_iron/roughness.png", false);
    ao = GLUtils::loadTexture("textures/pbr/rusted_iron/ao.png", false);*/
    albedo = GLUtils::loadTexture("textures/pbr/gold/albedo.png", false);
    normal = GLUtils::loadTexture("textures/pbr/gold/normal.png", false);
    metallic = GLUtils::loadTexture("textures/pbr/gold/metallic.png", false);
    roughness = GLUtils::loadTexture("textures/pbr/gold/roughness.png", false);
    ao = GLUtils::loadTexture("textures/pbr/gold/ao.png", false);
    /*albedo = GLUtils::loadTexture("textures/pbr/grass/albedo.png", false);
    normal = GLUtils::loadTexture("textures/pbr/grass/normal.png", false);
    metallic = GLUtils::loadTexture("textures/pbr/grass/metallic.png", false);
    roughness = GLUtils::loadTexture("textures/pbr/grass/roughness.png", false);
    ao = GLUtils::loadTexture("textures/pbr/grass/ao.png", false);*/

    // lights
    // ------
    lightPositions[0] = glm::vec3(0.0f, 0.0f, 10.0f);
    lightColors[0] = glm::vec3(150.0f, 150.0f, 150.0f);

    projection = glm::perspective(glm::radians(cameraUtils.Zoom), (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f, 100.0f);
    setMat4(m_ProgramObj, "projection", projection);
}

void lighting_textured::Draw() {
    //计算每一帧绘制的时间：先记录当前在开始时间
    float currentFrame = TimeUtils::currentTimeSeconds();

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(m_ProgramObj);
    glm::mat4 view = cameraUtils.GetViewMatrix();
    setMat4(m_ProgramObj, "view", view);
    setVec3(m_ProgramObj, "camPos", cameraUtils.Position);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, albedo);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, normal);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, metallic);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, roughness);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, ao);

    glm::mat4 model = glm::mat4(1.0f);
    for (int row = 0; row < nrRows; ++row) {
        for (int col = 0; col < nrColumns; ++col) {
            // 从左下向右上网格排列球体
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(
                    (col - (nrColumns / 2)) * spacing,
                    (row - (nrRows / 2)) * spacing,
                    0.0f
            ));
            setMat4(m_ProgramObj, "model", model);
            // 法线转换到世界空间的矩阵，CPU计算后传给GPU
            setMat3(m_ProgramObj, "normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
            renderSphere();
        }
    }

    // 使用相同的着色器渲染四个球形点光源，这里是为了减少代码亮，渲染效果也不会有问题
    for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i) {
        glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(currentFrame * 5.0) * 5.0, 0.0, 0.0);
        newPos = lightPositions[i];
        setVec3(m_ProgramObj, "lightPositions[" + std::to_string(i) + "]", newPos);
        setVec3(m_ProgramObj, "lightColors[" + std::to_string(i) + "]", lightColors[i]);

        model = glm::mat4(1.0f);
        model = glm::translate(model, newPos);
        model = glm::scale(model, glm::vec3(0.5f));
        setMat4(m_ProgramObj, "model", model);
        setMat3(m_ProgramObj, "normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
        renderSphere();
    }

    // 计算每一帧绘制的时间，再计算当前帧结束时间
    deltaTime = TimeUtils::currentTimeSeconds() - currentFrame;
}

void lighting_textured::renderSphere() {
    if (sphereVAO == 0) {
        glGenVertexArrays(1, &sphereVAO);

        unsigned int vbo, ebo;
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        std::vector<glm::vec3> positions;
        std::vector<glm::vec2> uv;
        std::vector<glm::vec3> normals;
        std::vector<unsigned int> indices;

        const unsigned int X_SEGMENTS = 64;
        const unsigned int Y_SEGMENTS = 64;
        const float PI = 3.14159265359f;
        // 生成顶点位置、纹理坐标和法线向量数据
        for (unsigned int x = 0; x <= X_SEGMENTS; ++x) {
            for (unsigned int y = 0; y <= Y_SEGMENTS; ++y) {
                // 根据细分点计算出水平和垂直方向上的相对位置比例
                float xSegment = (float) x / (float) X_SEGMENTS;
                float ySegment = (float) y / (float) Y_SEGMENTS;
                // 使用球体参数方程计算每个点的坐标位置
                float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
                float yPos = std::cos(ySegment * PI);
                float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

                // 保存顶点位置
                positions.push_back(glm::vec3(xPos, yPos, zPos));
                // 相对位置的比例作为纹理坐标保存
                uv.push_back(glm::vec2(xSegment, ySegment));
                // 球形中法线向量与顶点向量方向相同，所以顶点坐标也可以作为法线
                normals.push_back(glm::vec3(xPos, yPos, zPos));
            }
        }

        bool oddRow = false;
        for (unsigned int y = 0; y < Y_SEGMENTS; ++y) {
            if (!oddRow) // even rows: y == 0, y == 2; and so on
            {
                //偶数行，按照从左到右的顺序依次添加两个索引，分别对应当前行和下一行的顶点，以确保正确构建三角形面片
                for (unsigned int x = 0; x <= X_SEGMENTS; ++x) {
                    //y=0: 0*65+0, 0*65+1, 0*65+2,... 0*65+64;
                    //y=2: 2*65+0, 2*65+1, 2*65+2,... 2*65+64;
                    indices.push_back(y * (X_SEGMENTS + 1) + x);
                    //y=0: 1*65+0, 1*65+1, 1*65+2,... 1*65+64;
                    //y=2: 3*65+0, 3*65+1, 3*65+2,... 3*65+64;
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                }
            } else {
                // 奇数行，则按照从右到左的顺序依次添加两个索引，同样对应当前行和下一行的顶点，以确保正确构建三角形面片
                for (int x = X_SEGMENTS; x >= 0; --x) {
                    //y=1: 2*65+64, 2*65+63, 2*65+62,...2*65+0;
                    //y=3: 4*65+64, 4*65+63, 4*65+62,...4*65+0;
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                    //y=1: 1*65+64, 1*65+63, 1*65+62,... 1*65+0;
                    //y=3: 3*65+64, 3*65+63, 3*65+62,... 3*65+0;
                    indices.push_back(y * (X_SEGMENTS + 1) + x);
                }
            }
            // 每处理完一行后，通过oddRow =!oddRow切换行的奇偶性，以便在下一次循环中使用正确的索引生成方式
            oddRow = !oddRow;
        }
        // 将生成的索引数据的数量存储在indexCount变量中
        indexCount = static_cast<unsigned int>(indices.size());

        // 整合顶点数据到一个连续的数组
        std::vector<float> data;
        for (unsigned int i = 0; i < positions.size(); ++i) {
            data.push_back(positions[i].x);
            data.push_back(positions[i].y);
            data.push_back(positions[i].z);
            if (normals.size() > 0) {
                data.push_back(normals[i].x);
                data.push_back(normals[i].y);
                data.push_back(normals[i].z);
            }
            if (uv.size() > 0) {
                data.push_back(uv[i].x);
                data.push_back(uv[i].y);
            }
        }
        // 将数据绑定到 OpenGL 缓冲对象并配置顶点属性
        glBindVertexArray(sphereVAO);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0],
                     GL_STATIC_DRAW);
        unsigned int stride = (3 + 2 + 3) * sizeof(float);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void *) 0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void *) (3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void *) (6 * sizeof(float)));
    }

    // 三角形条带（GL_TRIANGLE_STRIP）的方式绘制球体
    glBindVertexArray(sphereVAO);
    glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
}

void lighting_textured::Shutdown() {
    GLBaseSample::Shutdown();
}