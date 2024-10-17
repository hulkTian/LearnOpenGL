//
// Created by ts on 2024/10/16.
//

#ifndef LEARNOPENGL_ANITALIASING_H
#define LEARNOPENGL_ANITALIASING_H

#include "GLBaseSample.h"

class AnitAliasing : public GLBaseSample {

public:
    AnitAliasing() = default;

    virtual ~AnitAliasing() = default;

    virtual void Create();

    virtual void Draw();

    virtual void Shutdown();

private:
    GLuint cubeVAO, cubeVBO, quadVAO, quadVBO, screenTexture;
    GLuint framebuffer, textureColorBufferMultiSampled, rbo, intermediateFBO;
    GLuint m_ProgramObj_screen;
};


#endif //LEARNOPENGL_ANITALIASING_H
