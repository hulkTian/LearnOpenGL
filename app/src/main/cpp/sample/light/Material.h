//
// Created by ts on 2024/9/10.
//

#ifndef LEARNOPENGL_MATERIAL_H
#define LEARNOPENGL_MATERIAL_H

#include "GLBaseSample.h"
#include "CameraUtils.h"

class Material : public GLBaseSample {

public:
    Material() = default;

    virtual ~Material() = default;

    virtual void Create();

    virtual void Draw();

    virtual void Shutdown();

    virtual void ProcessInput(int i);

    virtual void MoveCallback(double x, double y);
private:
    //定义一个光源的VAO，避免修改物体颜色属性影响光源；光源颜色固定为白色
    GLuint VBO, VAO, VAO_light;
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

};

#endif //LEARNOPENGL_MATERIAL_H
