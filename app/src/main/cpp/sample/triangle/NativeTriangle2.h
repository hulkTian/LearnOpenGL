//
// Created by TS on 2024/2/28.
//

#pragma once

#include <GLBaseSample.h>

class NativeTriangle2 : public GLBaseSample {

public:
    NativeTriangle2() = default;

    virtual ~NativeTriangle2() = default;

    virtual void Create();

    virtual void Draw();

    virtual void Shutdown();

private:
    GLuint vao;
    GLuint vbo;
};