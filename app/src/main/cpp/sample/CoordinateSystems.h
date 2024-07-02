//
// Created by ts on 2024/7/1.
//

#ifndef LEARNOPENGL_COORDINATESYSTEMS_H
#define LEARNOPENGL_COORDINATESYSTEMS_H


#include <GLES3/gl3.h>
#include "GLBaseSample.h"

class CoordinateSystems : public GLBaseSample {
public:
    CoordinateSystems() = default;

    virtual ~CoordinateSystems() = default;

    virtual void Create();

    virtual void Draw();

    virtual void Shutdown();

private:
    //声明顶点缓存对象和顶点数组对象
    GLuint VBO, VAO, EBO;
    GLuint texture1, texture2;
};


#endif //LEARNOPENGL_COORDINATESYSTEMS_H
