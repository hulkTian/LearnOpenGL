#pragma once

#include "GLBaseSample.h"

class texture_unit : public GLBaseSample {

public:
    texture_unit() = default;

    virtual ~texture_unit() = default;

    virtual void Create();

    virtual void Draw();

    virtual void Shutdown();

private:
    //声明顶点缓存对象和顶点数组对象
    GLuint VAO;
    GLuint texture1, texture2;
};