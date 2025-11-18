//
// Created by tzh on 2025/11/18.
//

#ifndef LEARNOPENGL_COORDINATESYSTEMSEXERCISE_3_H
#define LEARNOPENGL_COORDINATESYSTEMSEXERCISE_3_H


#include "GLBaseSample.h"

class CoordinateSystemsExercise_3 : public GLBaseSample {
public:
    CoordinateSystemsExercise_3()  = default;

    virtual ~CoordinateSystemsExercise_3() = default;

    void Create() override;

    void Draw() override;

    void Shutdown() override;

private:
    GLuint VAO;
    GLuint texture1, texture2;
};


#endif //LEARNOPENGL_COORDINATESYSTEMSEXERCISE_3_H
