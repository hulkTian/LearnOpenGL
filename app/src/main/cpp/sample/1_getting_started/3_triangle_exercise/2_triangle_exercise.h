//
// Created by TS on 2024/2/29.
//
#pragma once

#include "GLBaseSample.h"

class triangle_exercise_2 : public GLBaseSample{

public:
    triangle_exercise_2() = default;

    virtual ~triangle_exercise_2() = default;

    virtual void Create();

    virtual void Draw();

    virtual void Shutdown();
private:
    GLuint VBOs[2], VAOs[2];
};