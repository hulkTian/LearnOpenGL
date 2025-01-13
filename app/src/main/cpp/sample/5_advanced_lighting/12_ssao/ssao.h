//
// Created by ts on 2024/11/19.
//

#ifndef LEARNOPENGL_SSAO_H
#define LEARNOPENGL_SSAO_H

#include "GLBaseSample.h"
#include "3_mesh/Model.h"

class ssao : public GLBaseSample {
public:

    ssao() = default;

    virtual ~ssao() = default;

    virtual void Create();

    virtual void Draw();

    virtual void Shutdown();

private:
    GLuint m_ProgramObj_Geometry_Pass, m_ProgramObj_Lighting_Pass, m_ProgramObj_SSAO, m_ProgramObj_SSAO_Blur;
    Model ourModel;
    GLuint gBuffer, gPosition, gNormal, gAlbedo, rboDepth;
    GLuint ssaoFBO, ssaoBlurFBO;
    GLuint ssaoColorBuffer, ssaoColorBufferBlur;
    std::vector<glm::vec3> ssaoNoise;
    GLuint noiseTexture;
    // lighting info
    // -------------
    glm::vec3 lightPos = glm::vec3(2.0, 4.0, -2.0);
    glm::vec3 lightColor = glm::vec3(0.2, 0.2, 0.7);
    GLuint cubeVAO, cubeVBO;
    GLuint quadVAO, quadVBO;
    std::vector<glm::vec3> ssaoKernel;

    float ourLerp(float a, float b, float f);

    void renderCube();

    void renderQuad();
};


#endif //LEARNOPENGL_SSAO_H
