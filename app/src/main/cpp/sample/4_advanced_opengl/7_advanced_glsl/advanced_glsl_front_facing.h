//
// Created by tzh on 2025/12/9.
//

#ifndef LEARNOPENGL_ADVANCED_GLSL_FRONT_FACING_H
#define LEARNOPENGL_ADVANCED_GLSL_FRONT_FACING_H

#include "GLBaseSample.h"

class advanced_glsl_front_facing : public GLBaseSample {
public:
     advanced_glsl_front_facing()=default;

    virtual ~advanced_glsl_front_facing()=default;

    virtual void Create();

    virtual void Draw();

    virtual void Shutdown();

private:
    GLuint m_programObj;
    GLuint VAO, VBO;
    GLuint cubeTexture;
};


#endif //LEARNOPENGL_ADVANCED_GLSL_FRONT_FACING_H
