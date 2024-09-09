//
// Created by ts on 2024/9/9.
//

#ifndef LEARNOPENGL_COLORSATVIEW_H
#define LEARNOPENGL_COLORSATVIEW_H


#include "GLBaseSample.h"
#include "CameraUtils.h"

class ColorsAtView : public GLBaseSample{
public:
    ColorsAtView() = default;

    virtual ~ColorsAtView() = default;

    virtual void Create();

    virtual void Draw();

    virtual void Shutdown();

private:
    //定义一个光源的VAO，避免修改物体颜色属性影响光源；光源颜色固定为白色
    GLuint VBO, VAO, VAO_light, EBO;
    // 光源着色器程序对象
    GLuint m_ProgramObj_Light;
    //光源立方体位置
    glm::vec3 lightPos = glm::vec3(0.0f, 0.7f, -1.5f);
    CameraUtils camera = CameraUtils(glm::vec3(0.0f, 0.5f, 2.0f));
};


#endif //LEARNOPENGL_COLORSATVIEW_H
