//
// Created by ts on 2024/10/10.
//

#ifndef LEARNOPENGL_CUBEMAPSREFLECTION_H
#define LEARNOPENGL_CUBEMAPSREFLECTION_H

#include "GLBaseSample.h"

class CubeMapsReflection  : public GLBaseSample{

public:
    CubeMapsReflection() = default;

    virtual ~CubeMapsReflection() = default;

    virtual void Create();

    virtual void Draw();

    virtual void Shutdown();

private:
    GLuint skyboxVAO, skyboxVBO;
    GLuint cubeVAO, cubeVBO;
    GLuint cubemapTexture, cubeTexture;
    GLuint m_ProgramObj_cube;
};


#endif //LEARNOPENGL_CUBEMAPSREFLECTION_H
