#pragma once

#include "GLBaseSample.h"

class NativeTriangle6 : public GLBaseSample {

public:
    NativeTriangle6() = default;

    virtual ~NativeTriangle6() = default;

    virtual void Create();

    virtual void Draw();

    virtual void Shutdown();

private:
    //声明顶点缓存对象和顶点数组对象
    GLuint VBO, VAO, EBO;
};