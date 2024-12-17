//
// Created by TS on 2024/3/1.
//

#pragma once

#include "GLBaseSample.h"

class uniform : public GLBaseSample{
public:
    uniform() = default;

    ~uniform() = default;

    virtual void Create();

    virtual void Draw();

    virtual void Shutdown();
private:
    //声明VBO和VAO
    GLuint VBO, VAO;
};