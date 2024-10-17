//
// Created by ts on 2024/9/29.
//

#ifndef LEARNOPENGL_STENCILTESTING_H
#define LEARNOPENGL_STENCILTESTING_H

#include "GLBaseSample.h"

class StencilTesting : public GLBaseSample {
public:
    StencilTesting() = default;

    ~StencilTesting() = default;

    virtual void Create();

    virtual void Draw();

    virtual void Shutdown();

private:
    GLuint cubeVAO, cubeVBO, planeVAO, planeVBO;
    GLuint cubeTexture, floorTexture;
    GLuint m_ProgramObj_border;
};


#endif //LEARNOPENGL_STENCILTESTING_H
