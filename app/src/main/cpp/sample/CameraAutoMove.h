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

    virtual void ProcessInput(int i);

    virtual void MoveCallback(double x, double y);

    virtual void Draw();

    virtual void Shutdown();

private:
    //声明顶点缓存对象和顶点数组对象
    GLuint VBO, VAO;
    GLuint texture1, texture2;
    CameraUtils cameraUtils = CameraUtils(glm::vec3(0.0f, 0.0f,  3.0f));
    float deltaTime = 0.0f; // 当前帧与上一帧的时间差
    float lastFrame = 0.0f; // 上一帧的时间
    bool firstMouse = true;
    float lastX = m_Width / 2.0f;
    float lastY = m_Height / 2.0f;
};


#endif //LEARNOPENGL_CAMERAAUTOMOVE_H
