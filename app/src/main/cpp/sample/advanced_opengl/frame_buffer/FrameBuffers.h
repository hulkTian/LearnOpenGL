//
// Created by ts on 2024/10/9.
//

#ifndef LEARNOPENGL_FRAMEBUFFERS_H
#define LEARNOPENGL_FRAMEBUFFERS_H

#include "GLBaseSample.h"

class FrameBuffers : public GLBaseSample {
public:
    FrameBuffers() = default;

    virtual ~FrameBuffers() = default;

    virtual void Create();

    virtual void Draw();

    virtual void Shutdown();

private:
    unsigned int framebuffer;
    unsigned int texColorBuffer;
    unsigned int rbo;

    unsigned int cubeVAO, cubeVBO;
    unsigned int planeVAO, planeVBO;
    unsigned int quadVAO, quadVBO;
    unsigned int cubeTexture;
    unsigned int floorTexture;
    unsigned int m_ProgramObj_screen;
};


#endif //LEARNOPENGL_FRAMEBUFFERS_H
