//
// Created by TS on 2024/2/28.
//

#pragma once

#include <GLBaseSample.h>

class NativeTriangle : public GLBaseSample {

public:
    NativeTriangle() = default;

    virtual ~NativeTriangle() = default;

    virtual void Create();

    virtual void Draw();

    virtual void Shutdown();

private:
    //声明顶点缓存对象和顶点数组对象
    GLuint VBO;
    GLuint VAO;
};