//
// Created by ts on 2024/10/17.
//

#ifndef LEARNOPENGL_ADVANCEDLIGHTING_H
#define LEARNOPENGL_ADVANCEDLIGHTING_H

#include "GLBaseSample.h"

class AdvancedLighting : public GLBaseSample {
public:
    AdvancedLighting() = default;

    virtual ~AdvancedLighting() = default;

    virtual void Create();

    virtual void Draw();

    virtual void Shutdown();

private:
    unsigned int planeVAO, planeVBO;
    unsigned int floorTexture;
    glm::vec3 lightPos = glm::vec3 (0.0f, 0.0f, 0.0f);
};


#endif //LEARNOPENGL_ADVANCEDLIGHTING_H
