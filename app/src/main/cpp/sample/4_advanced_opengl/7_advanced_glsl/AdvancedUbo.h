//
// Created by ts on 2024/10/12.
//

#ifndef LEARNOPENGL_ADVANCEDUBO_H
#define LEARNOPENGL_ADVANCEDUBO_H

#include "GLBaseSample.h"

class AdvancedUbo : public GLBaseSample {
public:
    AdvancedUbo() = default;

    virtual ~AdvancedUbo() = default;

    virtual void Create();

    virtual void Draw();

    virtual void Shutdown();

private:

    GLuint cubeVAO, cubeVBO;
    GLuint m_program_red, m_program_green, m_program_blue,m_program_yellow;
    GLuint uboMatrices;
};


#endif //LEARNOPENGL_ADVANCEDUBO_H
