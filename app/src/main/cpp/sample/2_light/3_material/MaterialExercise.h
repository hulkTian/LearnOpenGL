//
// Created by tzh on 2025/11/24.
//

#ifndef LEARNOPENGL_MATERIALEXERCISE_H
#define LEARNOPENGL_MATERIALEXERCISE_H


#include "GLBaseSample.h"

class MaterialExercise : public GLBaseSample {

public:
    MaterialExercise() = default;

    virtual ~MaterialExercise() = default;

    virtual void Create();

    virtual void Draw();

    virtual void Shutdown();

private:
    //定义一个光源的VAO，避免修改物体颜色属性影响光源；光源颜色固定为白色
    GLuint VBO, VAO, VAO_light;
    // 光源着色器程序对象
    GLuint m_ProgramObj_Light;
    //光源立方体位置
    glm::vec3 lightPos = glm::vec3(2.2f, 0.0f, 0.0f);
};


#endif //LEARNOPENGL_MATERIALEXERCISE_H
