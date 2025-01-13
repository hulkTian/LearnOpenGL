//
// Created by 15094 on 2025/1/9.
//

#ifndef LEARNOPENGL_DEPTH_DISPLAY_H
#define LEARNOPENGL_DEPTH_DISPLAY_H

#include "GLBaseSample.h"

class depth_display : public GLBaseSample {
public:
    depth_display() = default;

    virtual ~depth_display() = default;

    virtual void Create();

    virtual void Draw();

    virtual void Shutdown();

private:
    GLuint cubeVAO, cubeVBO, planeVAO, planeVBO;
    GLuint cubeTexture, floorTexture;
};


#endif //LEARNOPENGL_DEPTH_DISPLAY_H
