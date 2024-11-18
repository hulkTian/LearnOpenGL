//
// Created by ts on 2024/11/18.
//

#ifndef LEARNOPENGL_BLOOM_H
#define LEARNOPENGL_BLOOM_H

#include "GLBaseSample.h"

class bloom : public GLBaseSample {
public:
    bloom() = default;

    virtual ~bloom() = default;

    virtual void Create();

    virtual void Draw();

    virtual void Shutdown();

private:
    float exposure = 0.1f;
    GLuint m_ProgramObj_light, m_ProgramObj_blur, m_ProgramObj_bloom_final;
    GLuint woodTexture, containerTexture;
    GLuint hdrFBO, rboDepth;
    GLuint cubeVAO, cubeVBO;
    GLuint quadVAO, quadVBO;
    std::vector<glm::vec3> lightPositions;
    std::vector<glm::vec3> lightColors;
    GLuint colorBuffers[2];
    GLuint pingpongFBO[2];
    GLuint pingpongColorbuffers[2];
    void renderCube();

    void renderQuad();
};


#endif //LEARNOPENGL_BLOOM_H
