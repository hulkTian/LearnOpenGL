//
// Created by ts on 2024/10/9.
//

/**
 * 缓冲帧：颜色缓冲，深度缓冲，模板缓冲，这些缓冲结合起来叫做帧缓冲(Framebuffer)。
 *
 * 帧缓冲的工作过程：
 * 1.创建帧缓冲；
 * 2.绑定并激活帧缓冲；
 * 3.构建完整的帧缓冲；
 * 4.检查帧缓冲是否完整；
 * 5.使用帧缓冲实现特定效果；
 * 6.帧缓冲目标绑定到默认帧缓冲；
 * 6.删除帧缓冲；
 *
 * 一个完整的帧缓冲需要满足一下的条件：
 * 1.附加至少一个缓冲（颜色、深度、模板）；
 * 2.至少有一个颜色附件（Attachment）；
 * 3.所有的附件都必须是完整的（保留了内存）；
 * 4.每个缓冲都应该有相同的样本数（sample）；
 *
 * 附件：是一个内存位置，它能够作为帧缓冲的一个缓冲。
 *
 * 离屏渲染(Off-screen Rendering)：渲染到一个不同的帧缓冲被叫做离屏渲染。
 *
 * 纹理附件：
 * 为帧缓冲创建一个纹理和创建一个普通的纹理差不多，主要的区别就是，我们将维度设置为了屏幕大小（尽管这不是必须的），并且我们给纹理的data参数传递了NULL。
 *
 * 纹理附件添加到帧缓冲目标：
 * glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
 * target：帧缓冲目标（GL_FRAMEBUFFER、GL_READ_FRAMEBUFFER 或 GL_DRAW_FRAMEBUFFER）；
 * attachment：附件类型；
 *  可选的附件类型有：
 *  GL_COLOR_ATTACHMENT0，颜色附件类型，注意最后的0意味着我们可以附加多个颜色附件；
 *  GL_DEPTH_ATTACHMENT，深度附件类型，注意纹理格式需要是GL_DEPTH_COMPONENT；
 *  GL_STENCIL_ATTACHMENT，模板附件类型，注意纹理格式需要是GL_STENCIL_INDEX；
 *  GL_DEPTH_STENCIL_ATTACHMENT，深度和模板附件类型，注意纹理格式需要是 GL_DEPTH24_STENCIL8，纹理的每32位数值将包含24位的深度信息和8位的模板信息；
 * textarget：附加的纹理类型；
 * texture：要附加的纹理本身；
 * level：多级渐远纹理的级别，我们将它保留为0。
 *
 * 渲染缓冲对象附件(Renderbuffer Object)：
 * 是在纹理之后引入到OpenGL中，作为一个可用的帧缓冲附件类型。
 * 渲染缓冲对象附加的好处是，它会将数据储存为OpenGL原生的渲染格式。因为它的数据已经是原生的格式了，所以交换缓冲这样的操作在使用渲染缓冲对象时会非常快。
 * 由于渲染缓冲对象通常都是只写的，它们会经常用于深度和模板附件，因为大部分时间我们都不需要从深度和模板缓冲中读取值，只关心深度和模板测试。
 * 创建一个渲染缓冲对象和纹理对象类似，不同的是这个对象是专门被设计作为帧缓冲附件使用的，而不是纹理那样的通用数据缓冲(General Purpose Data Buffer)。
 *
 * 渲染缓冲对象附件和纹理附件怎么选择？
 * 如果你不需要从一个缓冲中采样数据，那么对这个缓冲使用渲染缓冲对象会是明智的选择。如果你需要从缓冲中采样颜色或深度值等数据，那么你应该选择纹理附件。
 *
 * 后期效果：
 * 相反：在片段着色器中返回纹理的相反颜色。
 * 灰度：对像素的RGB分量通过加权值计算最后输出的颜色分量。
 * 核效果：对当前像素周围的纹理坐标添加一个小的偏移量，并根据核将结果合并。
 */

#include "FrameBuffers.h"

static float cubeVertices[] = {
        // positions          // texture Coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

static float planeVertices[] = {
        // positions          // texture Coords
        5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

        5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
        5.0f, -0.5f, -5.0f,  2.0f, 2.0f
};

static float quadVertices[] = {
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
        1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
        1.0f, -1.0f,  1.0f, 0.0f,
        1.0f,  1.0f,  1.0f, 1.0f
};

void FrameBuffers::Create() {
    GLUtils::printGLInfo();

    // cube
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *) (3 * sizeof(float)));
    glBindVertexArray(0);
    // plane
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *) (3 * sizeof(float)));
    glBindVertexArray(0);

    // screen quad VAO
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    // load textures
    cubeTexture = GLUtils::loadTgaTexture("textures/container.jpg", false,
                                          GL_REPEAT, GL_REPEAT,
                                          GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    floorTexture = GLUtils::loadTgaTexture("textures/metal.png", false,
                                           GL_REPEAT, GL_REPEAT,
                                           GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    m_ProgramObj = GLUtils::createProgram("shaders/vs_depth_testing.glsl",
                                          "shaders/fs_depth_testing.glsl");
    m_ProgramObj_screen = GLUtils::createProgram("shaders/vs_frame_buffers.glsl",
                                          "shaders/fs_frame_buffers.glsl");

    if (!m_ProgramObj || !m_ProgramObj_screen) {
        LOGD("Could not create program")
        return;
    }

    //指定纹理位置
    glUseProgram(m_ProgramObj);
    setInt(m_ProgramObj, "texture1", 0);
    glUseProgram(m_ProgramObj_screen);
    setInt(m_ProgramObj_screen, "screenTexture", 0);

    // 创建一个帧缓冲
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    // 生成纹理附件
    glGenTextures(1, &texColorBuffer);
    glBindTexture(GL_TEXTURE_2D, texColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 将纹理附件附加到当前绑定的帧缓冲对象
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

    // 生成渲染缓冲对象附件
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);

    // 将渲染缓冲对象附件附加到当前绑定的帧缓冲对象
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    // 检查帧缓冲是否完整
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        LOGE("ERROR::FRAMEBUFFER:: Framebuffer is not complete!")
        return;
    }

    // 帧缓冲目标绑定到默认帧缓冲
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffers::Draw() {
    //计算每一帧绘制的时间：先记录当前在开始时间
    float currentFrame = TimeUtils::currentTimeSeconds();

    // 第一处理阶段(Pass)
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 我们现在不使用模板缓冲
    glEnable(GL_DEPTH_TEST);

    glUseProgram(m_ProgramObj);
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = cameraUtils.GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(cameraUtils.Zoom), SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
    setMat4(m_ProgramObj, "view", view);
    setMat4(m_ProgramObj, "projection", projection);
    // cubes
    glBindVertexArray(cubeVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, cubeTexture);
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
    setMat4(m_ProgramObj,"model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
    setMat4(m_ProgramObj,"model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    // floor
    glBindVertexArray(planeVAO);
    glBindTexture(GL_TEXTURE_2D, floorTexture);
    setMat4(m_ProgramObj,"model", glm::mat4(1.0f));
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    // 第二处理阶段
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // 返回默认
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(m_ProgramObj_screen);
    glBindVertexArray(quadVAO);
    glDisable(GL_DEPTH_TEST);
    glBindTexture(GL_TEXTURE_2D, texColorBuffer);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // 计算每一帧绘制的时间，再计算当前帧结束时间
    deltaTime = TimeUtils::currentTimeSeconds() - currentFrame;
}

void FrameBuffers::Shutdown() {
    glDeleteTextures(1, &texColorBuffer);
    glDeleteTextures(1, &cubeTexture);
    glDeleteTextures(1, &floorTexture);
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &planeVAO);
    glDeleteVertexArrays(1, &quadVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &planeVBO);
    glDeleteBuffers(1, &quadVBO);
    glDeleteRenderbuffers(1, &rbo);
    glDeleteFramebuffers(1, &framebuffer);
    glDeleteProgram(m_ProgramObj_screen);
    GLBaseSample::Shutdown();
}
