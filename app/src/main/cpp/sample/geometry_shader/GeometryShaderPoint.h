//
// Created by ts on 2024/10/14.
//

#ifndef LEARNOPENGL_GEOMETRYSHADERPOINT_H
#define LEARNOPENGL_GEOMETRYSHADERPOINT_H

#include "GLBaseSample.h"

class GeometryShaderPoint : public GLBaseSample {

public:

    GeometryShaderPoint() = default;

    ~GeometryShaderPoint() = default;

    virtual void Create();

    virtual void Draw();

    virtual void Shutdown();

private:
    GLuint VAO, VBO;

};


#endif //LEARNOPENGL_GEOMETRYSHADERPOINT_H
