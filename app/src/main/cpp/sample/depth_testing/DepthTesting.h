//
// Created by ts on 2024/9/29.
//

#ifndef LEARNOPENGL_DEPTHTESTING_H
#define LEARNOPENGL_DEPTHTESTING_H

#include "GLBaseSample.h"

class DepthTesting : public GLBaseSample {
public:
    DepthTesting() = default;

    virtual ~DepthTesting() = default;

    virtual void Create();

    virtual void Draw();

    virtual void Shutdown();

private:
    GLuint cubeVAO, cubeVBO, planeVAO, planeVBO;
    GLuint cubeTexture, floorTexture;
};


#endif //LEARNOPENGL_DEPTHTESTING_H
