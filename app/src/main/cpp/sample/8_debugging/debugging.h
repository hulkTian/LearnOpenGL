//
// Created by ts on 2024/12/5.
//

#ifndef LEARNOPENGL_DEBUGGING_H
#define LEARNOPENGL_DEBUGGING_H

#include "GLBaseSample.h"

class debugging : public GLBaseSample {
public:
    debugging() = default;

    ~debugging() {
        glDeleteBuffers(1, &cubeVBO);
        glDeleteVertexArrays(1, &cubeVAO);
        glDeleteBuffers(1, &quadVBO);
        glDeleteVertexArrays(1, &quadVAO);
        glDeleteTextures(1, &texture);
    }

    virtual void Create();

    virtual void Draw();

    virtual void Shutdown();

private:
    GLuint cubeVAO, cubeVBO;
    GLuint texture;
};


#endif //LEARNOPENGL_DEBUGGING_H
