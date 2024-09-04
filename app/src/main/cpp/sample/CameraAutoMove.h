//
// Created by ts on 2024/9/4.
//

#ifndef LEARNOPENGL_CAMERAAUTOMOVE_H
#define LEARNOPENGL_CAMERAAUTOMOVE_H


#include "GLBaseSample.h"

class CameraAutoMove : public GLBaseSample{
public:
    CameraAutoMove() = default;

    virtual ~CameraAutoMove() = default;

    virtual void Create();

    virtual void ProcessInput(int i);

    virtual void MoveCallback(double x, double y, double z);

    virtual void Draw();

    virtual void Shutdown();

private:
    //声明顶点缓存对象和顶点数组对象
    GLuint VBO, VAO, EBO;
    GLuint texture1, texture2;
    glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);
    float deltaTime = 0.0f; // 当前帧与上一帧的时间差
    float lastFrame = 0.0f; // 上一帧的时间
};


#endif //LEARNOPENGL_CAMERAAUTOMOVE_H
