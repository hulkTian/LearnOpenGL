//
// Created by ts on 2024/10/10.
//

#ifndef LEARNOPENGL_REFLECTIONMODE_H
#define LEARNOPENGL_REFLECTIONMODE_H

#include "GLBaseSample.h"
#include "3_mesh/Model.h"

class ReflectionMode : public GLBaseSample {
public:
    ReflectionMode() = default;

    virtual ~ReflectionMode() = default;

    virtual void Create();

    virtual void Draw();

    virtual void Shutdown();

private:
    GLuint skyboxVAO, skyboxVBO;
    GLuint cubeVAO, cubeVBO;
    GLuint cubemapTexture;
    Model ourModel;
    GLuint m_ProgramObj_mode;
};


#endif //LEARNOPENGL_REFLECTIONMODE_H
