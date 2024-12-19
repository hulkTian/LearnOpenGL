//
// Created by ts on 2024/12/18.
//

#ifndef LEARNOPENGL_TEXTURE_WARP_H
#define LEARNOPENGL_TEXTURE_WARP_H

#include "GLBaseSample.h"

class texture_warp : public GLBaseSample {
public:
    texture_warp() = default;
    ~texture_warp() = default;

    virtual void Create();
    virtual void Draw();
    virtual void Shutdown();

private:
    GLuint VAO;
    GLuint ourTexture;
};


#endif //LEARNOPENGL_TEXTURE_WARP_H
