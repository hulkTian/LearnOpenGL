//
// Created by ts on 2024/10/8.
//

#ifndef LEARNOPENGL_CULLFACE_H
#define LEARNOPENGL_CULLFACE_H

#include "GLBaseSample.h"

class CullFace : public GLBaseSample {

public:
    CullFace() = default;

    virtual ~CullFace()=default;

    virtual void Create();

    virtual void Draw();

    virtual void Shutdown();
private:
    GLuint cubeVAO, cubeVBO, planeVAO, planeVBO;
    GLuint cubeTexture, floorTexture;
};


#endif //LEARNOPENGL_CULLFACE_H
