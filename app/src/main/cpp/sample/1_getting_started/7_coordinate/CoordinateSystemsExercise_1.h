//
// Created by tzh on 2025/11/18.
//

#ifndef LEARNOPENGL_COORDINATESYSTEMSEXERCISE_1_H
#define LEARNOPENGL_COORDINATESYSTEMSEXERCISE_1_H

#include "GLBaseSample.h"

class CoordinateSystemsExercise_1 : public GLBaseSample{

public:
    CoordinateSystemsExercise_1() = default;

    virtual ~CoordinateSystemsExercise_1() = default;

    virtual void Create();

    virtual void Draw();

    virtual void Shutdown();

private:
    GLuint VAO;
    GLuint texture1, texture2;
};


#endif //LEARNOPENGL_COORDINATESYSTEMSEXERCISE_1_H
