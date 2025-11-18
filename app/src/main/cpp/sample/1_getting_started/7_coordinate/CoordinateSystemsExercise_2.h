//
// Created by tzh on 2025/11/18.
//

#ifndef LEARNOPENGL_COORDINATESYSTEMSEXERCISE_2_H
#define LEARNOPENGL_COORDINATESYSTEMSEXERCISE_2_H

#include <GLES3/gl3.h>
#include "GLBaseSample.h"

class CoordinateSystemsExercise_2 : public GLBaseSample {
public:
    CoordinateSystemsExercise_2() = default;

    virtual ~CoordinateSystemsExercise_2() = default;

    void Create() override;

    void Draw() override;

    void Shutdown() override;

private:
    GLuint VAO;
    GLuint texture1, texture2;
    float angleY = 0.0f;
};


#endif //LEARNOPENGL_COORDINATESYSTEMSEXERCISE_2_H
