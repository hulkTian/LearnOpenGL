//
// Created by ts on 2024/10/15.
//

#ifndef LEARNOPENGL_ASTEROIDS_H
#define LEARNOPENGL_ASTEROIDS_H

#include "GLBaseSample.h"
#include "3_mesh/Model.h"

class Asteroids : public GLBaseSample {

public:
    Asteroids() = default;

    virtual ~Asteroids() = default;

    virtual void Create();

    virtual void Draw();

    virtual void Shutdown();

private:
    Model rock, planet;
    unsigned int amount = 10000;
    glm::mat4* modelMatrices;
};


#endif //LEARNOPENGL_ASTEROIDS_H
