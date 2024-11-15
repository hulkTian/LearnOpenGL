//
// Created by ts on 2024/11/14.
//

#ifndef LEARNOPENGL_HDR_H
#define LEARNOPENGL_HDR_H

#include "GLBaseSample.h"

class hdr : public GLBaseSample {
public:
    hdr() = default;

    virtual void Create();

    virtual void Draw();

    virtual void Shutdown();

private:
    float exposure = 0.1f;
    GLuint m_ProgramObj_hdr;
    GLuint woodTexture;
    GLuint hdrFBO, colorBuffer,rboDepth;
    GLuint cubeVAO, cubeVBO;
    GLuint quadVAO, quadVBO;
    std::vector<glm::vec3> lightPositions;
    std::vector<glm::vec3> lightColors;

    void renderCube();

    void renderQuad();

};


#endif //LEARNOPENGL_HDR_H
