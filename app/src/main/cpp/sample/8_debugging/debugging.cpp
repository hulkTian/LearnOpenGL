//
// Created by ts on 2024/12/5.
//
/**
 * 调试技巧
 * glGetError函数可以获取错误代码；
 * glDebugMessageCallback函数开启调试模式后，可以定义回调函数来打印具体错误信息；
 * 帧缓冲调试可以使用一个debug着色器程序来渲染纹理缓冲附件，看纹理缓冲附件是否正确获取；
 * 可以把需要调试的变量输入到片段着色器，作为颜色数据，来调试变量的值是否符合预期；
 * 调试工具，例如 AGI，可以捕获应用中的OpenGL渲染流程进行调试
 */

#include "debugging.h"
#include "stb_image.h"

REGISTER_SAMPLE(SAMPLE_TYPE_DEBUGGING, debugging)

void GL_APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity,
                                        GLsizei length, const GLchar *message, void *userParam)
{
    // 忽略一些不重要的错误/警告代码
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    LOGD("---------------")
    LOGD("Debug message (\"%s\"): ", message)

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             LOGD("Source: API") break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   LOGD("Source: Window System") break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: LOGD("Source: Shader Compiler") break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     LOGD("Source: Third Party") break;
        case GL_DEBUG_SOURCE_APPLICATION:     LOGD("Source: Application") break;
        case GL_DEBUG_SOURCE_OTHER:           LOGD("Source: Other") break;
    }

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               LOGD("Type: Error") break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: LOGD("Type: Deprecated Behaviour") break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  LOGD("Type: Undefined Behaviour") break;
        case GL_DEBUG_TYPE_PORTABILITY:         LOGD("Type: Portability") break;
        case GL_DEBUG_TYPE_PERFORMANCE:         LOGD("Type: Performance") break;
        case GL_DEBUG_TYPE_MARKER:              LOGD("Type: Marker") break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          LOGD("Type: Push Group") break;
        case GL_DEBUG_TYPE_POP_GROUP:           LOGD("Type: Pop Group") break;
        case GL_DEBUG_TYPE_OTHER:               LOGD("Type: Other") break;
    }

    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         LOGD("Severity: high") break;
        case GL_DEBUG_SEVERITY_MEDIUM:       LOGD("Severity: medium") break;
        case GL_DEBUG_SEVERITY_LOW:          LOGD("Severity: low") break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: LOGD("Severity: notification") break;
    }
}

void debugging::Create()
{
    GLint flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    LOGD("==========flags========== %d", flags)
    // todo 这里获取flags 一直是0，不确定哪里没有正确吃实话调试上下文
    // 但 Android 系统的OpenGL会自己检查错误，并打印
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(reinterpret_cast<GLDEBUGPROC>(glDebugOutput), nullptr);
        glDebugMessageControl(GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_ERROR, GL_DEBUG_SEVERITY_HIGH, 0, nullptr, GL_TRUE);
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    m_ProgramObj = GLUtils::createProgram("shaders/vs_debugging.glsl", "shaders/fs_debugging.glsl");

    if (!m_ProgramObj)
    {
        return;
    }

    float vertices[] = {
            // back face
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, // bottom-left
            0.5f,  0.5f, -0.5f,  1.0f,  1.0f, // top-right
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f, // bottom-right
            0.5f,  0.5f, -0.5f,  1.0f,  1.0f, // top-right
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, // bottom-left
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f, // top-left
            // front face
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, // bottom-left
            0.5f, -0.5f,  0.5f,  1.0f,  0.0f, // bottom-right
            0.5f,  0.5f,  0.5f,  1.0f,  1.0f, // top-right
            0.5f,  0.5f,  0.5f,  1.0f,  1.0f, // top-right
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f, // top-left
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, // bottom-left
            // left face
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f, // top-right
            -0.5f,  0.5f, -0.5f, -1.0f,  1.0f, // top-left
            -0.5f, -0.5f, -0.5f, -0.0f,  1.0f, // bottom-left
            -0.5f, -0.5f, -0.5f, -0.0f,  1.0f, // bottom-left
            -0.5f, -0.5f,  0.5f, -0.0f,  0.0f, // bottom-right
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f, // top-right
            // right face
            0.5f,  0.5f,  0.5f,  1.0f,  0.0f, // top-left
            0.5f, -0.5f, -0.5f,  0.0f,  1.0f, // bottom-right
            0.5f,  0.5f, -0.5f,  1.0f,  1.0f, // top-right
            0.5f, -0.5f, -0.5f,  0.0f,  1.0f, // bottom-right
            0.5f,  0.5f,  0.5f,  1.0f,  0.0f, // top-left
            0.5f, -0.5f,  0.5f,  0.0f,  0.0f, // bottom-left
            // bottom face
            -0.5f, -0.5f, -0.5f,  0.0f,  1.0f, // top-right
            0.5f, -0.5f, -0.5f,  1.0f,  1.0f, // top-left
            0.5f, -0.5f,  0.5f,  1.0f,  0.0f, // bottom-left
            0.5f, -0.5f,  0.5f,  1.0f,  0.0f, // bottom-left
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, // bottom-right
            -0.5f, -0.5f, -0.5f,  0.0f,  1.0f, // top-right
            // top face
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f, // top-left
            0.5f,  0.5f,  0.5f,  1.0f,  0.0f, // bottom-right
            0.5f,  0.5f, -0.5f,  1.0f,  1.0f, // top-right
            0.5f,  0.5f,  0.5f,  1.0f,  0.0f, // bottom-right
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f, // top-left
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f  // bottom-left
    };
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindVertexArray(cubeVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    int width, height, nrComponents;
    unsigned char *data = stbi_load("/data/data/com.example.learnopengl/textures/wood.png", &width, &height, &nrComponents, 0);
    if (data)
    {
        glTexImage2D(GL_FRAMEBUFFER, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        LOGE("Failed to load texture")
    }
    stbi_image_free(data);

    // set up projection matrix
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10.0f);
    glUniformMatrix4fv(glGetUniformLocation(m_ProgramObj, "projection"), 1, GL_FALSE, &projection[0][0]);
    glUniform1i(glGetUniformLocation(m_ProgramObj, "tex"), 0);
}

void debugging::Draw()
{
    // render
    // ------
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(m_ProgramObj);
    float rotationSpeed = 10.0f;
    float angle = (float)TimeUtils::currentTimeSeconds() * rotationSpeed;
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0, 0.0f, -2.5));
    model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 1.0f, 1.0f));
    setMat4(m_ProgramObj, "model", model);

    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void debugging::Shutdown()
{
    GLBaseSample::Shutdown();
}