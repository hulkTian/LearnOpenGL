//
// Created by ts on 2024/12/12.
//

#ifndef LEARNOPENGL_WEIGHTED_BLENDED_H
#define LEARNOPENGL_WEIGHTED_BLENDED_H

#include "GLBaseSample.h"

class weighted_blended : public GLBaseSample {
public:
    weighted_blended() = default;

    ~weighted_blended() {
        glDeleteFramebuffers(1, &opaqueFBO);
        glDeleteFramebuffers(1, &transparentFBO);
        glDeleteTextures(1, &opaqueTexture);
        glDeleteTextures(1, &depthTexture);
        glDeleteTextures(1, &accumTexture);
        glDeleteTextures(1, &revealTexture);
        glDeleteProgram(m_ProgramObj);
        glDeleteProgram(m_ProgramObj_Solid);
        glDeleteProgram(m_ProgramObj_Transparent);
        glDeleteProgram(m_ProgramObj_Composite);
    }

    virtual void Create();

    virtual void Draw();

private:
    GLuint m_ProgramObj_Solid, m_ProgramObj_Transparent, m_ProgramObj_Composite;
    unsigned int opaqueFBO, transparentFBO;
    unsigned int opaqueTexture;
    unsigned int depthTexture;
    unsigned int accumTexture;
    unsigned int revealTexture;

    glm::mat4 redModelMat;
    glm::mat4 greenModelMat;
    glm::mat4 blueModelMat;

    glm::vec4 zeroFillerVec;
    glm::vec4 oneFillerVec;
};


#endif //LEARNOPENGL_WEIGHTED_BLENDED_H
