//
// Created by ts on 2024/9/30.
//

#ifndef LEARNOPENGL_BLENDINGDISCARD_H
#define LEARNOPENGL_BLENDINGDISCARD_H

#include "GLBaseSample.h"

class BlendingDiscard : public GLBaseSample {
public:
    BlendingDiscard() = default;

    virtual ~BlendingDiscard() = default;

    virtual void Create();

    virtual void Draw();

    virtual void Shutdown();

private:
    GLuint cubeVAO, cubeVBO, planeVAO, planeVBO, transparentVAO, transparentVBO;
    GLuint cubeTexture, floorTexture, transparentTexture;

};

#endif //LEARNOPENGL_BLENDINGDISCARD_H
