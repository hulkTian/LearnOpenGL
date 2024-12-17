//
// Created by ts on 2024/12/17.
//

#ifndef LEARNOPENGL_VAO_VBO_H
#define LEARNOPENGL_VAO_VBO_H

#include "GLBaseSample.h"

class vao_vbo :public GLBaseSample {
public:
    vao_vbo() = default;
    ~vao_vbo() = default;

    virtual void Create();
    virtual void Draw();
    virtual void Shutdown();

private:
    // 使用OpenGL提供的数据类型可以保证不同平台数据宽度一致
    GLuint VAO;
    GLuint VBO;
};


#endif //LEARNOPENGL_VAO_VBO_H
