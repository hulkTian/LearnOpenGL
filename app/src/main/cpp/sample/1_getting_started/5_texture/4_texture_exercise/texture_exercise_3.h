//
// Created by ts on 2024/12/18.
//

#pragma once


#include "GLBaseSample.h"

class texture_exercise_3 : public GLBaseSample {

public:
    texture_exercise_3() = default;

    virtual ~texture_exercise_3() = default;

    virtual void Create();

    virtual void Draw();

    virtual void Shutdown();

private:
    //声明顶点缓存对象和顶点数组对象
    GLuint VAO;
    GLuint texture1, texture2;
};