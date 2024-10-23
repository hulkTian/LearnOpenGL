//
// Created by ts on 2024/10/23.
//

#ifndef LEARNOPENGL_GAMMA_CORRECTION_H
#define LEARNOPENGL_GAMMA_CORRECTION_H

#include "GLBaseSample.h"

class gamma_correction : public GLBaseSample {
public:
    gamma_correction() = default;

    virtual ~gamma_correction() = default;

    virtual void Create();
    virtual void Draw();
    virtual void Shutdown();

private:
    GLuint floorTexture, floorTextureGammaCorrected;
    GLuint planeVAO, planeVBO;
};


#endif //LEARNOPENGL_GAMMA_CORRECTION_H
