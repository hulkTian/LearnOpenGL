//
// Created by ts on 2024/10/15.
//

#ifndef LEARNOPENGL_INSTANCING_H
#define LEARNOPENGL_INSTANCING_H

#include "GLBaseSample.h"

class Instancing : public GLBaseSample {
public:
    Instancing() = default;

    virtual ~Instancing() = default;

    virtual void Create();

    virtual void Draw();

    virtual void Shutdown();

private:
    GLuint VAO, VBO, instanceVBO;
};


#endif //LEARNOPENGL_INSTANCING_H
