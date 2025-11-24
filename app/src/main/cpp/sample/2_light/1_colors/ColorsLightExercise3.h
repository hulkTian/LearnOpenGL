//
// Created by tzh on 2025/11/21.
//

#ifndef LEARNOPENGL_COLORSLIGHTEXERCISE3_H
#define LEARNOPENGL_COLORSLIGHTEXERCISE3_H

#include "GLBaseSample.h"

class ColorsLightExercise3 : public GLBaseSample {
public:
    ColorsLightExercise3() =default;

    virtual ~ColorsLightExercise3() = default;

    virtual void Create();

    virtual void Draw();

    virtual void Shutdown();

private:
    //定义一个光源的VAO，避免修改物体颜色属性影响光源；光源颜色固定为白色
    GLuint VBO, VAO, VAO_light, EBO;
    // 光源着色器程序对象
    GLuint m_ProgramObj_Light;
    //世界空间的光源立方体位置
    glm::vec3 lightPos = glm::vec3(0.2f, 0.0f, 2.0f);
};


#endif //LEARNOPENGL_COLORSLIGHTEXERCISE3_H
