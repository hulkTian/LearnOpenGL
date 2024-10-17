//
// Created by ts on 2024/9/4.
//

#ifndef LEARNOPENGL_CAMERAAUTOMOVE_H
#define LEARNOPENGL_CAMERAAUTOMOVE_H


#include "GLBaseSample.h"
#include "CameraUtils.h"

class CameraAutoMove : public GLBaseSample{
public:
    CameraAutoMove() = default;

    virtual ~CameraAutoMove() = default;

    virtual void Create();

    virtual void Draw();

    virtual void Shutdown();

private:
    //声明顶点缓存对象和顶点数组对象
    GLuint VBO, VAO;
    GLuint texture1, texture2;
};


#endif //LEARNOPENGL_CAMERAAUTOMOVE_H
