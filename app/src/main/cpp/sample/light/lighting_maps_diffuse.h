//
// Created by ts on 2024/9/11.
//
#include "GLBaseSample.h"
#include "CameraUtils.h"

class LightingMapsDiffuse : public GLBaseSample {
public:
    LightingMapsDiffuse() = default;

    ~LightingMapsDiffuse() = default;

    virtual void Create();

    virtual void Draw();

    virtual void Shutdown();

private:
    //定义一个光源的VAO，避免修改物体颜色属性影响光源；光源颜色固定为白色
    GLuint VBO, VAO, VAO_light;
    // 光源着色器程序对象
    GLuint m_ProgramObj_Light;
    //光源立方体位置
    glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);

    //漫反射贴图
    GLuint diffuseMap;
    //镜面反射贴图
    GLuint specularMap;
    //放射光贴图
    GLuint emissionMap;
};

