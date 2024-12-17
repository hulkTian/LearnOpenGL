//
// Created by ts on 2024/12/17.
//

#ifndef LEARNOPENGL_EBO_H
#define LEARNOPENGL_EBO_H

#include "GLBaseSample.h"

class ebo :public GLBaseSample{
public:
    ebo() = default;
    ~ebo() = default;

    virtual void Create();
    virtual void Draw();
    virtual void Shutdown();

private:
    // 使用OpenGL提供的数据类型可以保证不同平台数据宽度一致
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
};


#endif //LEARNOPENGL_EBO_H
