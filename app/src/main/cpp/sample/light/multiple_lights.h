//
// Created by ts on 2024/9/14.
//

#ifndef LEARNOPENGL_MULTIPLE_LIGHTS_H
#define LEARNOPENGL_MULTIPLE_LIGHTS_H

#include "GLBaseSample.h"
#include "CameraUtils.h"

class MultipleLights : public GLBaseSample {
public:
    MultipleLights() = default;

    virtual ~MultipleLights() = default;

    virtual void Create();

    virtual void MoveCallback(double xposIn, double yposIn);

    virtual void ProcessInput(int i);

    virtual void Draw();

    virtual void Shutdown();

    virtual void DrawDefault();

    virtual void DrawDesert();

    virtual void DrawFactory();

    virtual void DrawHorror();

    virtual void DrawBiochemicalLab();

private:
    GLuint VBO, cubeVAO, lightCubeVAO;
    // 光源着色器程序对象
    GLuint m_ProgramObj_Light;
    //光源立方体位置
    glm::vec3 lightPos = glm::vec3(1.2f, 0.0f, 0.0f);
    // camera
    CameraUtils camera = CameraUtils(glm::vec3(0.0f, 0.0f, 3.0f));
    float lastX = m_Width / 2.0f;
    float lastY = m_Height / 2.0f;
    bool firstMouse = true;

    // timing
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    //漫反射贴图
    GLuint diffuseMap;
    //镜面反射贴图
    GLuint specularMap;

    //场景类型：0 default；1 desert；2 factory；3 horror；4 BIOCHEMICAL LAB
    int type = 1;
};

#endif //LEARNOPENGL_MULTIPLE_LIGHTS_H
