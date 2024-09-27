//
// Created by ts on 2024/9/25.
//

#ifndef LEARNOPENGL_MODELLOADING_H
#define LEARNOPENGL_MODELLOADING_H

#include "GLBaseSample.h"
#include "CameraUtils.h"
#include "Model.h"
#include <android/asset_manager_jni.h>

class ModelLoading : public GLBaseSample{
public:
    ModelLoading() = default;

    virtual ~ModelLoading() = default;

    virtual void Create();

    virtual void ProcessInput(int i);

    virtual void MoveCallback(double x, double y);

    virtual void Draw();

    virtual void Shutdown();
private:
    CameraUtils cameraUtils = CameraUtils(glm::vec3(0.0f, 0.0f,  3.0f));
    float deltaTime = 0.0f; // 当前帧与上一帧的时间差
    float lastFrame = 0.0f; // 上一帧的时间
    bool firstMouse = true;
    float lastX = m_Width / 2.0f;
    float lastY = m_Height / 2.0f;
    Model ourModel;
};


#endif //LEARNOPENGL_MODELLOADING_H
