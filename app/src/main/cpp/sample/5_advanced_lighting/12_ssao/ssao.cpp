//
// Created by ts on 2024/11/19.
//

/**
 * 为了使空间中像墙角或有遮挡地方的光照效果更真实，需要对这些区域显示的环境光进行一定的衰减。
 *
 * 环境光遮蔽(Ambient Occlusion)，它的原理是通过将褶皱、孔洞和非常靠近的墙面变暗的方法近似模拟出间接光照。
 *
 * 屏幕空间环境光遮蔽(Screen-Space Ambient Occlusion, SSAO):
 * 对于铺屏四边形(Screen-filled Quad)上的每一个片段，我们都会根据周边深度值计算一个遮蔽因子(Occlusion Factor)。
 * 这个遮蔽因子之后会被用来减少或者抵消片段的环境光照分量。
 *
 * 获取遮蔽因子：
 * 通过采集片段周围球型核心(Kernel)的多个深度样本，并和当前片段深度值对比，高于片段深度值样本的个数就是我们想要的遮蔽因子。
 *
 * 球形核心样本全采样虽然产生的效果好，但是性能开销大。样本数太低又会导致波纹。
 * 解决方式：使用随机旋转采样核心减少样本数，并对结果进行模糊，降低噪点。最后也不使用球体，而使用法向量半球体对周围采样。
 *
 * SSAO实现步骤：
 * 1. 创建gBuffer用于存储位置向量、法向量、颜色向量
 * 2. 创建半球采样核心，本例中包含64个采样，半球可以避免采样非可是区域的样本
 * 3. 创建随机旋转向量数组，作为噪声纹理，随机旋转可以保证采样质量
 * 4. 创建SSAO帧缓冲，保存遮蔽因子
 * 5. 创建SSAO 模糊帧缓冲，保存模糊后的遮蔽因子
 * 6. 几何处理阶段：渲染场景，保存几何信息到gBuffer
 * 7. 几何信息、采样点、噪声纹理传入SSAO着色器，计算遮蔽因子
 * 8. 遮蔽因子传入模糊着色器，计算模糊后的遮蔽因子
 * 9. 遮蔽因子和几何信息传入光照渲染着色器，渲染光照环境下的每个片段。环境光需要乘上遮蔽因子来计算
 */

#include "ssao.h"
#include <iostream>
#include <random>

void ssao::Create() {
    GLUtils::printGLInfo();

    cameraUtils = CameraUtils(glm::vec3(0.0f, 0.0f, 5.0f));

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    // 这个着色器程序是为了填充 gBuffer 使用
    m_ProgramObj_Geometry_Pass = GLUtils::createProgram("shaders/vs_ssao_geometry.glsl", "shaders/fs_ssao_geometry.glsl");
    // 这个着色器程序是为了计算 SSAO 的数据
    m_ProgramObj_SSAO = GLUtils::createProgram("shaders/vs_ssao.glsl", "shaders/fs_ssao.glsl");
    // 这个着色器程序是为了模糊 SSAO 的数据效果
    m_ProgramObj_SSAO_Blur = GLUtils::createProgram("shaders/vs_ssao.glsl", "shaders/fs_ssao_blur.glsl");
    // 这个着色器程序是为了渲染最后的光照效果场景
    m_ProgramObj_Lighting_Pass = GLUtils::createProgram("shaders/vs_ssao.glsl", "shaders/fs_ssao_lighting.glsl");

    if (!m_ProgramObj_Geometry_Pass || !m_ProgramObj_Lighting_Pass || !m_ProgramObj_SSAO || !m_ProgramObj_SSAO_Blur) {
        LOGD("Could not create program")
        return;
    }

    // load models
    // -----------
    std::string path(DEFAULT_OGL_ASSETS_DIR);
    ourModel = Model(path + "/backpack/backpack.obj");

    // configure g-buffer framebuffer
    // ------------------------------
    glGenFramebuffers(1, &gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
    // 观察空间的片段位置
    glGenTextures(1, &gPosition);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
    // 模型的法线贴图，在切线空间
    glGenTextures(1, &gNormal);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
    // color + specular color buffer
    glGenTextures(1, &gAlbedo);
    glBindTexture(GL_TEXTURE_2D, gAlbedo);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedo, 0);
    // 告诉OpenGL帧缓冲将使用哪些位置的颜色附件作为缓冲
    unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    //添加多个颜色附件到帧缓冲gBuffer中
    glDrawBuffers(3, attachments);
    // 创建渲染对象缓冲来缓存深度信息
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, SCR_WIDTH, SCR_HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
    // 最后，检查帧缓冲是否配置完全
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        LOGE("Framebuffer not complete!")
    }
    // 解绑帧缓冲gBuffer，恢复默认帧缓冲的绑定
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // also create framebuffer to hold SSAO processing stage
    // -----------------------------------------------------
    glGenFramebuffers(1, &ssaoFBO);
    glGenFramebuffers(1, &ssaoBlurFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
    // SSAO color buffer
    glGenTextures(1, &ssaoColorBuffer);
    glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RED, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        LOGE("SSAO Framebuffer not complete!")
    }
    // and blur stage
    glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
    glGenTextures(1, &ssaoColorBufferBlur);
    glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RED, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBufferBlur, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        LOGE("SSAO Blur Framebuffer not complete!")
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // generate sample kernel
    // ----------------------
    std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // generates random floats between 0.0 and 1.0
    std::default_random_engine generator;
    // 创建64个样本采样点
    for (unsigned int i = 0; i < 64; ++i)
    {
        // 在切线空间中以-1.0到1.0为范围变换x和y方向，并以0.0和1.0为范围变换样本的z方向(半球形)
        glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
        // 归一化样本
        sample = glm::normalize(sample);
        // 再次乘以一个随机数，使样本分布更随机
        sample *= randomFloats(generator);
        // 根据索引对样本点进行缩放调整
        float scale = float(i) / 64.0f;

        // scale samples s.t. they're more aligned to center of kernel
        scale = ourLerp(0.1f, 1.0f, scale * scale);
        // 随着索引的变大，样本的缩放会越来越大，使样本进行非线性分布，即靠近核心的样本密度更大。
        sample *= scale;
        ssaoKernel.push_back(sample);
    }

    // generate noise texture
    // ----------------------
    // 创建一个4x4朝向切线空间平面法线的随机旋转向量数组
    for (unsigned int i = 0; i < 16; i++)
    {
        // 在切线空间，绕Z轴旋转的向量
        glm::vec3 noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f);
        ssaoNoise.push_back(noise);
    }
    // 创建一个包含随机旋转向量的4x4纹理
    glGenTextures(1, &noiseTexture);
    glBindTexture(GL_TEXTURE_2D, noiseTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // 环绕方式要设置为 GL_REPEAT ，才能保证合适的平铺在屏幕平面上
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // shader configuration
    // --------------------
    // 光照场景着色器程序获取几何信息和SSAO数据，用于计算片段最后需要显示的片段颜色
    glUseProgram(m_ProgramObj_Lighting_Pass);
    setInt(m_ProgramObj_Lighting_Pass, "gPosition", 0);
    setInt(m_ProgramObj_Lighting_Pass, "gNormal", 1);
    setInt(m_ProgramObj_Lighting_Pass, "gAlbedo", 2);
    setInt(m_ProgramObj_Lighting_Pass, "ssao", 3);
    // SSAO着色器获取片段位置向量、法线向量和噪点纹理用于计算SSAO结果
    glUseProgram(m_ProgramObj_SSAO);
    setInt(m_ProgramObj_SSAO, "gPosition", 0);
    setInt(m_ProgramObj_SSAO, "gNormal", 1);
    setInt(m_ProgramObj_SSAO, "texNoise", 2);
    // SSAO模糊着色器获取SSAO结果用于模糊计算
    glUseProgram(m_ProgramObj_SSAO_Blur);
    setInt(m_ProgramObj_SSAO_Blur, "ssaoInput", 0);
}

void ssao::Draw() {
    //计算每一帧绘制的时间：先记录当前在开始时间
    float currentFrame = TimeUtils::currentTimeSeconds();

    // render
    // ------
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 1. SSAO几何处理阶段：使用帧缓冲保存需要的几何信息
    // -----------------------------------------------------------------
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::mat4 projection = glm::perspective(glm::radians(cameraUtils.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = cameraUtils.GetViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);
    glUseProgram(m_ProgramObj_Geometry_Pass);
    setMat4(m_ProgramObj_Geometry_Pass, "projection", projection);
    setMat4(m_ProgramObj_Geometry_Pass, "view", view);
    // room cube
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0, 7.0f, 0.0f));
    model = glm::scale(model, glm::vec3(7.5f, 7.5f, 7.5f));
    setMat4(m_ProgramObj_Geometry_Pass, "model", model);
    //在立方体内部渲染模型，需要将法线反转
    setInt(m_ProgramObj_Geometry_Pass, "invertedNormals", 1);
    renderCube();
    setInt(m_ProgramObj_Geometry_Pass, "invertedNormals", 0);
    // backpack model on the floor
    model = glm::mat4(1.0f);
    // 沿着Y轴移动5，使背包底部接触立方体底部
    model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0));
    // 绕 X 轴旋转-90度，使背包平台到立方体底部
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
    // 缩小背包，使背包模型大小合适立方体
    model = glm::scale(model, glm::vec3(1.0f));
    setMat4(m_ProgramObj_Geometry_Pass, "model", model);
    ourModel.Draw(m_ProgramObj_Geometry_Pass);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // 2. 生成SSAO纹理：根据缓冲的几何信息、采样核心向量数组和噪声纹理生成SSAO纹理，结果保存在 ssaoColorBuffer
    // ------------------------
    glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(m_ProgramObj_SSAO);
    // 64个采样点发送给着色器
    for (unsigned int i = 0; i < 64; ++i)
    {
        setVec3(m_ProgramObj_SSAO, "samples[" + std::to_string(i) + "]", ssaoKernel[i]);
    }
    // 设置投影矩阵到片段着色器，视图空间的位置向量需要变换到裁剪空间
    setMat4(m_ProgramObj_SSAO, "projection", projection);
    glActiveTexture(GL_TEXTURE0);
    // 视图空间片段位置向量
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glActiveTexture(GL_TEXTURE1);
    // 视图空间法向量
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glActiveTexture(GL_TEXTURE2);
    // 噪声纹理，包含绕切线空间Z轴随机旋转的向量数据，需要使用TBN矩阵转换到视图空间
    glBindTexture(GL_TEXTURE_2D, noiseTexture);
    // 通过渲染四边形，启动SSAO着色器程序，生成SSAO纹理并保存再帧缓冲 ssaoFBO
    renderQuad();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // 3. blur SSAO texture to remove noise
    // ------------------------------------
    glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(m_ProgramObj_SSAO_Blur);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
    renderQuad();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // 4. lighting pass: traditional deferred Blinn-Phong lighting with added screen-space ambient occlusion
    // -----------------------------------------------------------------------------------------------------
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(m_ProgramObj_Lighting_Pass);
    // send light relevant uniforms
    glm::vec3 lightPosView = glm::vec3(cameraUtils.GetViewMatrix() * glm::vec4(lightPos, 1.0));
    setVec3(m_ProgramObj_Lighting_Pass, "light.Position", lightPosView);
    setVec3(m_ProgramObj_Lighting_Pass, "light.Color", lightColor);
    // Update attenuation parameters
    const float linear    = 0.09f;
    const float quadratic = 0.032f;
    setFloat(m_ProgramObj_Lighting_Pass, "light.Linear", linear);
    setFloat(m_ProgramObj_Lighting_Pass, "light.Quadratic", quadratic);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, gAlbedo);
    glActiveTexture(GL_TEXTURE3); // add extra SSAO texture to lighting pass
    glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
    renderQuad();

    // 计算每一帧绘制的时间，再计算当前帧结束时间
    deltaTime = TimeUtils::currentTimeSeconds() - currentFrame;
}

void ssao::renderCube()
{
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

// renderQuad() renders a 1x1 XY quad in NDC
// -----------------------------------------
void ssao::renderQuad()
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {
                // positions        // texture Coords
                -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
                1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void ssao::Shutdown() {
    glDeleteBuffers(1, &gBuffer);
    glDeleteBuffers(1, &gPosition);
    glDeleteBuffers(1, &gNormal);
    glDeleteBuffers(1, &gAlbedo);
    glDeleteBuffers(1, &rboDepth);
    glDeleteBuffers(1, &ssaoFBO);
    glDeleteBuffers(1, &ssaoBlurFBO);
    glDeleteBuffers(1, &ssaoColorBuffer);
    glDeleteBuffers(1, &ssaoColorBufferBlur);
    glDeleteProgram(m_ProgramObj_Geometry_Pass);
    glDeleteProgram(m_ProgramObj_Lighting_Pass);
    glDeleteProgram(m_ProgramObj_SSAO);
    glDeleteProgram(m_ProgramObj_SSAO_Blur);
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteTextures(1, &noiseTexture);
    GLBaseSample::Shutdown();
}

float ssao::ourLerp(float a, float b, float f)
{
    return a + f * (b - a);
}