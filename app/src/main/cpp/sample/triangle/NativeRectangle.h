//
// Created by TS on 2024/2/28.
//

#ifndef LEARNOPENGL_NATIVERECTANGLE_H
#define LEARNOPENGL_NATIVERECTANGLE_H

#include <GLBaseSample.h>

class NativeRectangle : public GLBaseSample {
public:
    NativeRectangle() = default;

    virtual ~NativeRectangle() = default;

    virtual void Create();

    virtual void Draw();

    virtual void Shutdown();

private:
    unsigned int VBO, VAO, EBO;
};


#endif //LEARNOPENGL_NATIVERECTANGLE_H
