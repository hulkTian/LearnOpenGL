//
// Created by 田浩 on 2024/6/16.
//

#ifndef LEARNOPENGL_TRANSFORM_H
#define LEARNOPENGL_TRANSFORM_H

#include "GLBaseSample.h"

class Transform : public GLBaseSample {
public:
    Transform() = default;

    virtual ~Transform() = default;

    virtual void Create();

    virtual void Draw();

    virtual void Shutdown();

private:
    //声明顶点缓存对象和顶点数组对象
    GLuint VAO;
    GLuint texture1, texture2;
};


#endif //LEARNOPENGL_TRANSFORM_H
