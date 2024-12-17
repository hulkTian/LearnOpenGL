//
// Created by TS on 2024/2/29.
//

#pragma once

#include "GLBaseSample.h"

class triangle_exercise_1 : public GLBaseSample {

public:
    triangle_exercise_1() = default;

    virtual ~triangle_exercise_1() = default;

    virtual void Create();

    virtual void Draw();

    virtual void Shutdown();
private:
    unsigned int VBO, VAO;
};