#pragma once

#include <GLBaseSample.h>

class NativeTriangle7 : public GLBaseSample {

public:
    NativeTriangle7() = default;

    virtual ~NativeTriangle7() = default;

    virtual void Create();

    virtual void Draw();

    virtual void Shutdown();

private:
    //声明顶点缓存对象和顶点数组对象
    GLuint VBO, VAO, EBO;
    GLuint texture1, texture2;
};