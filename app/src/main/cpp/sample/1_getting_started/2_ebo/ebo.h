//
// Created by ts on 2024/12/17.
//

#ifndef LEARNOPENGL_EBO_H
#define LEARNOPENGL_EBO_H

#include "GLBaseSample.h"

class ebo :public GLBaseSample{
public:
    ebo() = default;
    virtual ~ebo() = default;

    virtual void Create() override;
    virtual void Draw() override;
    virtual void Shutdown() override;

private:
    // 使用OpenGL提供的数据类型可以保证不同平台数据宽度一致
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
};


#endif //LEARNOPENGL_EBO_H
