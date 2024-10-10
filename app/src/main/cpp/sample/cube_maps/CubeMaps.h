//
// Created by ts on 2024/10/9.
//

#ifndef LEARNOPENGL_CUBEMAPS_H
#define LEARNOPENGL_CUBEMAPS_H

#include "GLBaseSample.h"

class CubeMaps : public GLBaseSample {

public:
    CubeMaps() = default;

    virtual ~CubeMaps() = default;

    virtual void Create();

    virtual void Draw();

    virtual void Shutdown();

private:
    GLuint skyboxVAO, skyboxVBO;
    GLuint cubeVAO, cubeVBO;
    GLuint cubemapTexture, cubeTexture;
    GLuint m_ProgramObj_cube;
};


#endif //LEARNOPENGL_CUBEMAPS_H
