//
// Created by ts on 2024/11/26.
//

/**
 * 基于图像的光照(Image based lighting, IBL)是一类光照技术的集合。IBL 通常使用环境立方体贴图 (Cubemap) ，
 * 将立方体贴图的每个像素视为光源，在渲染方程中直接使用它。
 *
 * PBR的积分方程可以分为漫反射和镜面反射部分，这里主要讨论漫反射部分。
 * Lo(p,ωo)=Kdc/π∫ΩLi(p,ωi)n ⋅ ωidωi
 *
 * 辐照度图：使用任何一个向量对立方体贴图进行采样，就可以获取该方向上的场景辐照度。
 *
 * PBR 的大部分输入基于实际物理属性和测量，因此为入射光值找到其物理等效值是很重要的。在 PBR 渲染管线中考虑高动态范围(High Dynamic Range, HDR)的场景光照非常重要。
 * 辐射度的 HDR 文件格式：辐射度文件的格式（扩展名为 .hdr）存储了一张完整的立方体贴图，所有六个面数据都是浮点数，允许指定 0.0 到 1.0 范围之外的颜色值，以使光线具有正确的颜色强度。
 * 这个文件格式使用了一个聪明的技巧来存储每个浮点值：它并非直接存储每个通道的 32 位数据，而是每个通道存储 8 位，再以 alpha 通道存放指数——虽然确实会导致精度损失，但是非常有效率，不过需要解析程序将每种颜色重新转换为它们的浮点数等效值。
 *
 * 辐照度HDR环境贴图是从球体投影到平面上，让我们可以将环境信息存储到一张等距柱状体投影图（Equirectangular Map）中。
 *
 * 使用stb_image.h加载HDR文件：将辐射度 HDR 图像直接加载为一个浮点数数组。
 *
 * 为了提高采样性能，需要将等距柱状体投影图转换到立方体贴图。
 */

#include "ibl_irradiance_conversion.h"

REGISTER_SAMPLE(SAMPLE_TYPE_PBR_IBL_IRRADIANCE_CONVERSION, ibl_irradiance_conversion)

void ibl_irradiance_conversion::Create() {
    GLUtils::printGLInfo();

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    // 设置深度测试函数为小于等于
    glDepthFunc(GL_LEQUAL);

    m_ProgramObj = GLUtils::createProgram("shaders/vs_pbr_ibl_irradiance_conversion.glsl",
                                          "shaders/fs_pbr_ibl_irradiance_conversion.glsl");

    m_ProgramObj_Equirectangular_To_Cubemap = GLUtils::createProgram("shaders/vs_pbr_cubemap.glsl",
                                                                     "shaders/fs_pbr_equirectangular_to_cubemap.glsl");
    m_ProgramObj_Background = GLUtils::createProgram("shaders/vs_pbr_background.glsl",
                                                     "shaders/fs_pbr_background.glsl");

    if (!m_ProgramObj) {
        LOGD("Could not create program")
        return;
    }

    glUseProgram(m_ProgramObj);
    setVec3(m_ProgramObj, "albedo", 0.5f, 0.0f, 0.0f);
    setFloat(m_ProgramObj, "ao", 1.0f);

    glUseProgram(m_ProgramObj_Background);
    setInt(m_ProgramObj_Background, "environmentMap", 0);

    // lights
    // ------
    lightPositions[0] = glm::vec3(-10.0f, 10.0f, 10.0f);
    lightPositions[1] = glm::vec3(10.0f, 10.0f, 10.0f);
    lightPositions[2] = glm::vec3(-10.0f, -10.0f, 10.0f);
    lightPositions[3] = glm::vec3(10.0f, -10.0f, 10.0f);
    lightColors[0] = glm::vec3(300.0f, 300.0f, 300.0f);
    lightColors[1] = glm::vec3(300.0f, 300.0f, 300.0f);
    lightColors[2] = glm::vec3(300.0f, 300.0f, 300.0f);
    lightColors[3] = glm::vec3(300.0f, 300.0f, 300.0f);

    // pbr: setup framebuffer
    // ----------------------
    glGenFramebuffers(1, &captureFBO);
    glGenRenderbuffers(1, &captureRBO);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

    // pbr: setup cubemap to render to and attach to framebuffer
    // ---------------------------------------------------------
    glGenTextures(1, &envCubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
    for (unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // pbr: set up projection and view matrices for capturing data onto the 6 cubemap face directions
    // ----------------------------------------------------------------------------------------------
    captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    captureViews[0] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
    captureViews[1] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
    captureViews[2] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
    captureViews[3] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
    captureViews[4] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
    captureViews[5] = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f));

    // pbr: load the HDR environment map
    // ---------------------------------
    hdrTexture = GLUtils::loadHDRTexture("textures/hdr/newport_loft.hdr");

    // pbr: HDR 等距柱状环境贴图转换为等效的立方体贴图
    // ----------------------------------------------------------------------
    glUseProgram(m_ProgramObj_Equirectangular_To_Cubemap);
    setInt(m_ProgramObj_Equirectangular_To_Cubemap, "equirectangularMap", 0);
    setMat4(m_ProgramObj_Equirectangular_To_Cubemap, "projection", captureProjection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, hdrTexture);

    // 渲染的视口尺寸需要和缓冲帧设置的纹理附件尺寸一致
    glViewport(0, 0, 512, 512);
    // 绑定缓冲帧，准备存储渲染的立方体贴图
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    // 将等距柱状投影图转换为立方体贴图，需要渲染一个立方体，并从内部将等距柱状投影图投影到立方体的每个面，并将立方体的六个面的图像构造成立方体贴图。
    for (unsigned int i = 0; i < 6; ++i)
    {
        setMat4(m_ProgramObj_Equirectangular_To_Cubemap, "view", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderCube();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // initialize static shader uniforms before rendering
    // --------------------------------------------------
    projection = glm::perspective(glm::radians(cameraUtils.Zoom),
                                  (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f, 100.0f);
    glUseProgram(m_ProgramObj);
    setMat4(m_ProgramObj, "projection", projection);

    glUseProgram(m_ProgramObj_Background);
    setMat4(m_ProgramObj_Background, "projection", projection);
}

void ibl_irradiance_conversion::Draw() {
    //计算每一帧绘制的时间：先记录当前在开始时间
    float currentFrame = TimeUtils::currentTimeSeconds();

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(m_ProgramObj);
    glm::mat4 view = cameraUtils.GetViewMatrix();
    setMat4(m_ProgramObj, "view", view);
    setVec3(m_ProgramObj, "camPos", cameraUtils.Position);

    glm::mat4 model = glm::mat4(1.0f);
    for (int row = 0; row < nrRows; ++row) {
        // 从下往上金属度从0.0到1.0
        setFloat(m_ProgramObj, "metallic", (float) row / (float) nrRows);
        for (int col = 0; col < nrColumns; ++col) {
            // 从左往右粗糙度从0.05到1.0
            setFloat(m_ProgramObj, "roughness",
                     glm::clamp((float) col / (float) nrColumns, 0.05f, 1.0f));

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

    // 使用相同的着色器渲染四个球形点光源，这里是为了减少代码，渲染效果也不会有问题
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

    glUseProgram(m_ProgramObj_Background);
    setMat4(m_ProgramObj_Background, "view", view);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
    renderCube();


    // 计算每一帧绘制的时间，再计算当前帧结束时间
    deltaTime = TimeUtils::currentTimeSeconds() - currentFrame;
}

void ibl_irradiance_conversion::renderCube() {
    // initialize (if necessary)
    if (cubeVAO == 0)
    {
        float vertices[] = {
                // back face
                -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
                1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
                1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right
                1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
                -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
                -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
                // front face
                -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
                1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
                1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
                1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
                -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
                -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
                // left face
                -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
                -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
                -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
                -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
                -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
                -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
                // right face
                1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
                1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
                1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right
                1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
                1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
                1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left
                // bottom face
                -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
                1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
                1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
                1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
                -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
                -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
                // top face
                -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
                1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
                1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right
                1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
                -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
                -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left
        };
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        // fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // link vertex attributes
        glBindVertexArray(cubeVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    // render Cube
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void ibl_irradiance_conversion::renderSphere() {
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

void ibl_irradiance_conversion::Shutdown() {
    glDeleteProgram(m_ProgramObj_Equirectangular_To_Cubemap);
    glDeleteProgram(m_ProgramObj_Background);
    glDeleteTextures(1, &hdrTexture);
    glDeleteTextures(1, &envCubemap);
    glDeleteBuffers(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &captureFBO);
    glDeleteBuffers(1, &captureRBO);
    GLBaseSample::Shutdown();
}
