//
// Created by ts on 2024/7/3.
//

#ifndef LEARNOPENGL_CAMERA_H
#define LEARNOPENGL_CAMERA_H

#include "GLBaseSample.h"

class Camera : public GLBaseSample{
public:
    Camera() = default;

    virtual ~Camera() = default;

    virtual void Create();

    virtual void Draw();

    virtual void Shutdown();

private:
    //声明顶点缓存对象和顶点数组对象
    GLuint VBO, VAO, EBO;
    GLuint texture1, texture2;
};


#endif //LEARNOPENGL_CAMERA_H
