//
// Created by tzh on 2025/11/26.
//

#ifndef LEARNOPENGL_LIGHT_CASTERS_POINT_H
#define LEARNOPENGL_LIGHT_CASTERS_POINT_H


#include "GLBaseSample.h"

class light_casters_point : public GLBaseSample {
public:
    light_casters_point() = default;

    virtual ~light_casters_point() = default;

    virtual void Create();

    virtual void Draw();

    virtual void Shutdown();

private:
    //定义一个光源的VAO，避免修改物体颜色属性影响光源；光源颜色固定为白色
    GLuint VBO, cubeVAO, lightCubeVAO;
    // 光源着色器程序对象
    GLuint m_ProgramObj_Light;
    //光源立方体位置
    glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);
    //漫反射贴图
    GLuint diffuseMap;
    //镜面反射贴图
    GLuint specularMap;
};


#endif //LEARNOPENGL_LIGHT_CASTERS_POINT_H
