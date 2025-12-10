//
// Created by tzh on 2025/12/9.
//

#ifndef LEARNOPENGL_ADVANCED_GLSL_POINT_SIZE_H
#define LEARNOPENGL_ADVANCED_GLSL_POINT_SIZE_H

#include "GLBaseSample.h"

class advanced_glsl_point_size : public GLBaseSample {
public:
     advanced_glsl_point_size()=default;

    virtual ~advanced_glsl_point_size()=default;

    virtual void Create();

    virtual void Draw();

    virtual void Shutdown();

private:
    GLuint m_programObj;
    GLuint VAO, VBO;
};


#endif //LEARNOPENGL_ADVANCED_GLSL_POINT_SIZE_H
