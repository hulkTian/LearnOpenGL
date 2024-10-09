//
// Created by ts on 2024/10/9.
//

#ifndef LEARNOPENGL_FRAMEBUFFERSEXERCISE_H
#define LEARNOPENGL_FRAMEBUFFERSEXERCISE_H

#include "GLBaseSample.h"

class FrameBuffersExercise : public GLBaseSample {

public:
    FrameBuffersExercise() = default;

    virtual ~FrameBuffersExercise()=default;

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


#endif //LEARNOPENGL_FRAMEBUFFERSEXERCISE_H
